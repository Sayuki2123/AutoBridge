#ifndef AUTOBRIDGE_MODULE_HELPER_H
#define AUTOBRIDGE_MODULE_HELPER_H

void load_native_bridge();
void unload_native_bridge();

void* get_callbacks(const char* app_code_cache_dir);

#endif // AUTOBRIDGE_MODULE_HELPER_H
