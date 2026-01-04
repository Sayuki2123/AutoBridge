#include <cstring>

#include "module_helper.h"
#include "nativebridge/native_bridge.h"

android::NativeBridgeCallbacks NativeBridgeItf;

extern "C" bool native_bridge_initialize(const android::NativeBridgeRuntimeCallbacks* art_cbs, const char* app_code_cache_dir, const char* isa) {
    auto callbacks = get_callbacks(app_code_cache_dir);

    if (callbacks == nullptr) {
        return false;
    }

    std::memcpy(&NativeBridgeItf, callbacks, sizeof(android::NativeBridgeCallbacks));

    return NativeBridgeItf.initialize(art_cbs, app_code_cache_dir, isa);
}

__attribute__((constructor)) static void on_init() {
    load_native_bridge();

    NativeBridgeItf = {
        .version = 1,
        .initialize = &native_bridge_initialize};
}

__attribute__((destructor)) static void on_exit() {
    unload_native_bridge();
}
