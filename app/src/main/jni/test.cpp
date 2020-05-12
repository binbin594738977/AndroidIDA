#include <string.h>
#include "test.h"

/**
 * 定义c++函数  注意函数名规范 -->Java_对应java方法的绝对路径(JNIEnv *env , jobject p0 , 对应java方法的参数)
 */
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_androidida_JniTest_javaCallNative(JNIEnv *env, jobject jobj, jstring p1) {
    return p1;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_example_androidida_JniTest_hasString(JNIEnv *env, jobject jobj, jstring p1) {
    const char *nativeString1 = env->GetStringUTFChars(p1, JNI_FALSE);
    int res = strncmp(nativeString1, "password", strlen(nativeString1));
    env->ReleaseStringUTFChars(p1, nativeString1);
    return res == 0 ? true : false;
}


