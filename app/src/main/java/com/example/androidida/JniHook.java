package com.example.androidida;

public class JniHook {
    static {
        System.loadLibrary("jniHook");
    }
    private static native void nativeMark();

    public static native void hookNativeMethods(Object[] methods);
}
