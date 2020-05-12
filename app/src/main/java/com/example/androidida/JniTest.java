package com.example.androidida;

public class JniTest {
    //加载动态库
    static {
        System.loadLibrary("test");
    }

    /**
     * 调用本地函数,返回当前字符串
     */
    public native String javaCallNative(String str);

    /**
     * 调用本地函数,判断String
     */
    public native boolean hasString(String str);


}
