#include "module_helper.h"

#include <dlfcn.h>
#include <unistd.h>
#include <cerrno>
#include <cstdio>
#include <cstring>

#include "log.h"

struct NativeBridge {
    const char* name;
    const char* file_name;
    const char* lib_path;
    void* handle{nullptr};
    void* callbacks{nullptr};
};

enum LibNativeBridge {
    LibHoudini,
#ifdef __LP64__
    LibNDK,
#endif // __LP64__
    LibCount
};

static NativeBridge native_bridges[LibCount];
static const LibNativeBridge default_native_bridge = LibHoudini;

void load_native_bridge() {
    native_bridges[LibHoudini] = {
        "Houdini",
        "libhoudini.so",
        "/vendor/lib64/arm64"};

#ifdef __LP64__
    native_bridges[LibNDK] = {
        "NDK_translation",
        "libndk_translation.so",
        "/system/lib64/arm64_ndk"};
#endif // __LP64__

    for (auto& nb : native_bridges) {
        nb.handle = dlopen(nb.file_name, RTLD_LAZY);

        if (nb.handle == nullptr) {
            LOGE("Failed to load %s: %s", nb.name, dlerror());

            continue;
        }

        nb.callbacks = dlsym(nb.handle, "NativeBridgeItf");

        if (nb.callbacks == nullptr) {
            LOGE("Failed to load %s: NativeBridgeItf not found", nb.name);

            dlclose(nb.handle);
            nb.handle = nullptr;

            continue;
        }

        LOGD("Native bridge loaded: %s", nb.name);
    }
}

void unload_native_bridge() {
    for (auto& nb : native_bridges) {
        if (nb.handle != nullptr) {
            dlclose(nb.handle);
        }
    }
}

#ifdef __LP64__
static constexpr const char* path_symlink = "/system/lib64/arm64";
static constexpr const char* file_applist = "/data/local/tmp/autobridge/ndk-apps.txt";
static constexpr const char* file_request = "/data/local/tmp/autobridge/redirect.req";

static LibNativeBridge get_required_native_bridge(const char* app_code_cache_dir) {
    auto file = std::fopen(file_applist, "r");

    if (file == nullptr) {
        LOGE("Failed to get required infomation: %s", std::strerror(errno));
        LOGE("Using the default native bridge: %s", native_bridges[default_native_bridge].name);

        return default_native_bridge;
    }

    char package_name[151];
    auto required_native_bridge = default_native_bridge;

    while (std::fgets(package_name, sizeof(package_name), file) != nullptr) {
        package_name[std::strcspn(package_name, "\n")] = 0;

        if (std::strstr(app_code_cache_dir, package_name) != nullptr) {
            required_native_bridge = LibNDK;

            break;
        }
    }

    std::fclose(file);

    LOGD("Required native bridge: %s", native_bridges[required_native_bridge].name);

    return required_native_bridge;
}

static bool is_redirect_required(LibNativeBridge required_native_bridge) {
    char buffer[32];
    auto len = readlink(path_symlink, buffer, sizeof(buffer) - 1);

    if (len < 0) {
        LOGW("Unable to get current native bridge, skipping redirect check");

        return false;
    }

    buffer[len] = '\0';
    auto result = std::strcmp(buffer, native_bridges[required_native_bridge].lib_path) != 0;

    LOGD("Redirect required: %s", result ? "Yes" : "No");

    return result;
}

static void send_redirect_request() {
    auto file = std::fopen(file_request, "w+");

    if (file == nullptr) {
        LOGE("Failed to send redirect request to module service: %s", std::strerror(errno));

        return;
    }

    std::fputc(33, file);

    LOGD("Send redirect request to module service, waiting for redirect complete...");

    auto count = 20;

    do {
        usleep(250000);
        std::fseek(file, 0, SEEK_END);
    } while (std::ftell(file) > 0 && --count > 0);

    std::fclose(file);

    if (count == 0) {
        LOGE("Native bridge redirect timed out, check module service.sh");
    }
}
#endif // __LP64__

void* get_callbacks(const char* app_code_cache_dir) {
    LOGD("Initializing native bridge at %s", app_code_cache_dir);

#ifndef __LP64__
    auto require_native_bridge = default_native_bridge;
#else
    auto require_native_bridge = get_required_native_bridge(app_code_cache_dir);

    if (is_redirect_required(require_native_bridge)) {
        send_redirect_request();
    }
#endif // __LP64__

    if (native_bridges[require_native_bridge].callbacks == nullptr) {
        LOGE("%s is not available", native_bridges[require_native_bridge].name);
    }

    return native_bridges[require_native_bridge].callbacks;
}
