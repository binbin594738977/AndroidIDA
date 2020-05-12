package com.example.androidida;

public class JniTest {
    //加载动态库
    static {
        System.loadLibrary("test");
    }

    /**
     * 定义本地函数
     */
    public native String javaCallNative(String str);

}
