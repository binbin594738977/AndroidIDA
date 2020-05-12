#include "test.h"

/**
 * 定义c++函数  注意函数名规范 -->Java_对应java方法的绝对路径(JNIEnv *env , jobject p0 , 对应java方法的参数)
 */
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_androidida_JniTest_javaCallNative(JNIEnv *env, jobject jobj, jstring p1) {
    return p1;
}

