#ifndef AUTOBRIDGE_LOG_H
#define AUTOBRIDGE_LOG_H

#include "android/log.h"

#ifdef __LP64__
#define LOG_TAG "AutoBridge64"
#else
#define LOG_TAG "AutoBridge32"
#endif // __LP64__

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)

#endif // AUTOBRIDGE_LOG_H
