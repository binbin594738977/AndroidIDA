
#include <jniHook.h>
#include <dlfcn.h>
#include <string.h>

JavaVM *g_vm;
static jclass gJniHookClass;

//定义一个类型 FuncItem
typedef struct {
    const char *method_name;
    void *origin_func;
    void *new_func;
} FuncItem;

//native函数指针位于结构的偏移量
static int gNativeOffset;

FuncItem *getFuncItemFromKey(const char *key);

typedef JNIEXPORT jstring JNICALL (*callNativeMethod)(JNIEnv *env, jclass cls, jstring name);


jstring new_callNativeMethod(JNIEnv *env, jclass clazz, jstring str) {
    const char *key = "callNativeMethod";
    LOGD("invoke %s", key);
    FuncItem *funcItem = getFuncItemFromKey(key);
    callNativeMethod origin_func = (callNativeMethod) funcItem->origin_func;
    LOGE("new_callNativeMethod %p", &origin_func);
    jstring ret = origin_func(env, clazz, str);
    LOGE("new_callNativeMethod");
    return env->NewStringUTF("new_callNativeMethod");
}

FuncItem gFuncItems[] = {
        {"callNativeMethod", NULL, (void *) new_callNativeMethod},
};

FuncItem *getFuncItemFromKey(const char *key) {
    int i;
    int count = sizeof(gFuncItems) / sizeof(FuncItem);
    for (i = 0; i < count; i++) {
        if (strcmp(gFuncItems[i].method_name, key) == 0) {
            return &gFuncItems[i];
        }
    }
    return NULL;
}

void mark(JNIEnv *env, jclass clazz) {
    // Do nothing
    LOGE("mark");
};

static JNINativeMethod gMarkMethods[] = {
        {"nativeMark", "()V", (void *) mark}
};

/**
 * 计算native函数指针位于结构的偏移量
 */
void searchJniOffset(JNIEnv *env) {
    jclass g_class = env->FindClass(JAVA_CLASS);
    jmethodID mtd_nativeHook = env->GetStaticMethodID(g_class, "nativeMark", "()V");


    size_t startAddress = (size_t) mtd_nativeHook;
    size_t targetAddress = (size_t) mark;


    int offset = 0;
    bool found = false;
    while (true) {
        if (*((size_t *) (startAddress + offset)) == targetAddress) {
            found = true;
            break;
        }
        offset += 4;
        if (offset >= 100) {
            LOGE("Ops: Unable to find the jni function.");
            break;
        }
    }
    if (found) {
        gNativeOffset = offset;
        LOGD("Get the offset : %d.", gNativeOffset);
    }
}

/**
 * 替换指针,取代执行
 */
void replaceImplementation(JNIEnv *env, jobjectArray methods) {
    int length = env->GetArrayLength(methods);
    jclass cls = env->FindClass("java/lang/reflect/Method");
    jmethodID getNameMethod = env->GetMethodID(cls, "getName", "()Ljava/lang/String;");
    jmethodID getParameterTypesMethod = env->GetMethodID(cls, "getParameterTypes",
                                                         "()[Ljava/lang/Class;");
    jclass stringClass = env->FindClass("java/lang/String");
    jstring keyString;
    const char *key;
    int i;
    bool replace;
    for (i = 0; i < length; ++i) {
        jobject method = env->GetObjectArrayElement(methods, i);
        keyString = (jstring) env->CallObjectMethod(method, getNameMethod);
        key = env->GetStringUTFChars(keyString, NULL);
        replace = false;
        if (strcmp(key, "open") == 0) {
            env->ReleaseStringUTFChars(keyString, key);
            key = "openFileInputStream";
            replace = true;
        } else if (strcmp(key, "getUserInfo") == 0) {
            env->ReleaseStringUTFChars(keyString, key);
            jobjectArray classArray = (jobjectArray) env->CallObjectMethod(method,
                                                                           getParameterTypesMethod);
//            public static native String getUserInfo(int i, String str, String[] strArr);
//            public static native String getUserInfo(int i, String str, String[] strArr, String str2);
//            public static native String getUserInfo(int i, String[] strArr, String[] strArr2, String str);
            if (env->GetArrayLength(classArray) == 3) {
                key = "getUserInfo1";
            } else if (env->GetObjectArrayElement(classArray, 1) == stringClass) {
                key = "getUserInfo2";
            } else {
                key = "getUserInfo3";
            }
            replace = true;
        }

        size_t mtd_openDexNative = (size_t) env->FromReflectedMethod(method);
        int nativeFuncOffset = gNativeOffset;
        void **jniFuncPtr = (void **) (mtd_openDexNative + nativeFuncOffset);

//        char vmSoName[4] = {0};
//        __system_property_get("ro.build.version.sdk", vmSoName);
//        int sdk;
//        sscanf(vmSoName, "%d", &sdk);
//        LOGD("The vm is art and the sdk int is %d", sdk);

        FuncItem *funcItem = getFuncItemFromKey(key);
        funcItem->origin_func = *jniFuncPtr;
        *jniFuncPtr = funcItem->new_func;

        if (!replace) {
            env->ReleaseStringUTFChars(keyString, key);
        }
    }
}

/**
 * hook本地函数
 */
void hookNativeMethods(JNIEnv *env, jclass clazz, jobjectArray methods) {
    g_vm->GetEnv((void **) &env, JNI_VERSION_1_6);
    g_vm->AttachCurrentThread(&env, NULL);

    jclass g_class = env->FindClass(JAVA_CLASS);
    if (env->RegisterNatives(g_class, gMarkMethods, 1) < 0) {
        return;
    }

    char vmSoName[15] = {0};
    __system_property_get("persist.sys.dalvik.vm.lib", vmSoName);
    LOGD("Find the so name : %s.", strlen(vmSoName) == 0 ? "<EMPTY>" : vmSoName);

    void *vmHandle = dlopen(vmSoName, 0);
    if (!vmHandle) {
        LOGE("Unable to open the %s.", vmSoName);
        vmHandle = RTLD_DEFAULT;
    }
    searchJniOffset(env);
    replaceImplementation(env, methods);
}


//结构体: 分别是java层的函数名称，签名，对应的函数指针
static JNINativeMethod gMethods[] = {
        {"hookNativeMethods", "([Ljava/lang/Object;)V", (void *) hookNativeMethods},
};

//java层加载System.loadLibrary之后，自动会查找该库一个叫JNI_OnLoad的函数
//JNI_OnLoad函数只会走一次
//JNI_OnLoad函数实现
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    g_vm = vm;
    JNIEnv *env;

    LOGE("JNI_Onload start");
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        LOGE("GetEnv() FAILED!!!");
        return JNI_ERR;
    }

    jclass javaClass = env->FindClass(JAVA_CLASS);
    LOGE("we have found the class: %s", JAVA_CLASS);
    if (javaClass == NULL) {
        LOGE("unable to find class: %s", JAVA_CLASS);
        return JNI_ERR;
    }

    gJniHookClass = static_cast<jclass>(env->NewGlobalRef(javaClass));

    env->UnregisterNatives(javaClass);
    if (env->RegisterNatives(javaClass, gMethods, sizeof(gMethods) / sizeof(JNINativeMethod)) < 0) {
        LOGE("register methods FAILED!!!");
        return JNI_ERR;
    }

    env->DeleteLocalRef(javaClass);
    LOGI("JavaVM::GetEnv() SUCCESS!");
    return JNI_VERSION_1_6;
}





