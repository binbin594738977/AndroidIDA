#include <string.h>
#include "jniMethods.h"


JavaVM *g_vm;
static jclass gJniHookClass;


u32 test() {
    return reverseByte(100);
}

jstring callNativeMethod(JNIEnv *env, jclass jobj, jstring p1) {
    const char *nativeString1 = env->GetStringUTFChars(p1, JNI_FALSE);
    env->ReleaseStringUTFChars(p1, nativeString1);
    u32 a = test();
    if (a == 100) {
        return nullptr;
    }
    return p1;
}

jstring callNative2(JNIEnv *env, jclass jobj, jstring p1) {
    const char *nativeString1 = env->GetStringUTFChars(p1, JNI_FALSE);
    env->ReleaseStringUTFChars(p1, nativeString1);
    u32 a = test();
    if (a == 100) {
        return nullptr;
    }
    return p1;
}

//结构体: 分别是java层的函数名称，签名，对应的函数指针
static JNINativeMethod gMethods[] = {
        {"callNativeMethod", "(Ljava/lang/String;)Ljava/lang/String;", (void *) callNativeMethod},
        {"callNative2",      "(Ljava/lang/String;)Ljava/lang/String;", (void *) callNative2},
};

//每次java层加载System.loadLibrary之后，自动会查找改库一个叫JNI_OnLoad的函数
//JNI_OnLoad函数实现
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    g_vm = vm;
    JNIEnv *env;

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    jclass javaClass = env->FindClass(JAVA_CLASS);
    if (javaClass == nullptr) {
        return JNI_ERR;
    }

    gJniHookClass = static_cast<jclass>(env->NewGlobalRef(javaClass));

    env->UnregisterNatives(javaClass);
    if (env->RegisterNatives(javaClass, gMethods, sizeof(gMethods) / sizeof(JNINativeMethod)) < 0) {
        return JNI_ERR;
    }

    env->DeleteLocalRef(javaClass);
    return JNI_VERSION_1_6;
}

