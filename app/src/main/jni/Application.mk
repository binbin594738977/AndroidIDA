#(Application Binary Interface)编译CPU架构如:
APP_ABI := armeabi-v7a
#指定目标Android API版本
APP_PLATFORM := android-14
#指定要使用的stl库，默认为system
APP_STL := system
#优化选项，默认值为release。可选值为debug和release。release模式下文件体积更小，debug模式下提供更多的调试信息
APP_OPTIM := release