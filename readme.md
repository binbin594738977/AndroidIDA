本项目是结合IDA学习演示的apk项目
# 准备指令
```shell script
ps -ef|grep android_
./data/local/tmp/android_server
adb shell am start -D -n com.example.androidida/com.example.androidida.MainActivity
jdb -connect com.sun.jdi.SocketAttach:hostname=127.0.0.1,port=11116
adb forward tcp:23946 tcp:23946

```



[专栏地址](https://blog.csdn.net/binbin594738977/category_9995640.html)
1. [IDA常见问题](https://blog.csdn.net/binbin594738977/article/details/106058742)
2. [android逆向-通过IDA工具dump指定的dex](https://blog.csdn.net/binbin594738977/article/details/103068017)
3. [IDA静态分析so文件](https://blog.csdn.net/binbin594738977/article/details/106076621)
4. [IDA调试Android的so文件](https://blog.csdn.net/binbin594738977/article/details/106055975)
5. [IDA调试Android的so文件(二)](https://blog.csdn.net/binbin594738977/article/details/106070388)