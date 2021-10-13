#LOCAL_PATH定义本地路径,  $(call my-dir)是一个函数,意思是获取当前函数
LOCAL_PATH := $(call my-dir)
#CLEAR_VARS由编译系统提供，指定让GNU MAKEFILE为你清除许多LOCAL_XXX变量（例如 LOCAL_MODULE, LOCAL_SRC_FILES, LOCAL_STATIC_LIBRARIES, 等等...),除LOCAL_PATH 。这是必要的，因为所有的编译控制文件都在同一个GNU MAKE执行环境中，所有的变量都是全局的。
include $(CLEAR_VARS)
#LOCAL_MODULE变量必须定义，以标识你在Android.mk文件中描述的每个模块。名称必须是唯一的，而且不包含任何空格。注意编译系统会自动产生合适的前缀和后缀，换句话说，一个被命名为'foo'的共享库模块，将会生成'libfoo.so'文件。
LOCAL_MODULE := test
#LOCAL_SRC_FILES变量必须包含将要编译打包进模块中的C或C++源代码文件。注意，你不用在这里列出头文件和包含文件，因为编译系统将会自动为你找出依赖型的文件；仅仅列出直接传递给编译器的源代码文件就好。
LOCAL_SRC_FILES := test.cpp util.cpp
#BUILD_SHARED_LIBRARY 由编译系统提供,表示编译一个分享库(动态库.so)
include $(BUILD_SHARED_LIBRARY)



