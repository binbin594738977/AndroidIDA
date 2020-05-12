
/**
 * 是否已经引用了test.h这个文件了,防止多次引用
 * 一般是
 * #ifndef XXX_H
 * #define XXX_H
 *
 * ...
 *
 * #endif
 *
 */
#ifndef TEST_H
#define TEST_H

#include <jni.h>//引用jni.h

/**
 * 声明方法,具体在同名.cpp里面实现
 */
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_test_JniMainActivity_javaCallNative(JNIEnv *env, jobject p0, jstring p1);


#endif


