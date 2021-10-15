
#ifndef JNI_HOOK_H
#define JNI_HOOK_H


#include <jni.h>
#include <android/log.h>
#define TAG "xxxJNI_HOOK"
#define JAVA_CLASS "com/example/androidida/JniHook"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,  TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,  TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,  TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

__BEGIN_DECLS
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved);
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved);
__END_DECLS


#endif //JNIHOOK_H
