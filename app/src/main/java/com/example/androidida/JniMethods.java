package com.example.androidida;

public class JniMethods {
    //加载动态库
    static {
        System.loadLibrary("jniMethods");
    }

    /**
     * 调用本地函数,返回当前字符串
     */
    public static native String callNativeMethod(String str);


    /**
     * 调用本地函数,返回当前字符串
     */
    public static native String callNative2(String str);

}
