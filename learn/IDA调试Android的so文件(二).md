本文是记录自己学习IDA的一些总结与归纳,并将其体现成具体功能,便于复习与交流,有不对之处望大家包容并指出,欢迎大家留言收藏

本篇对比[IDA动态调试android的so文件]()第一篇大体相同,但是也有不同之处,具体可以根据具体需求选择不同的方式

# 环境
# 环境
- 小米5 - root - android7.0
- IDA pro 7.0
- so包是32位
- **android运行demo并进入主界面这时候不会加载so包,点按钮运行加载so包并执行native函数**
- 如果出现其他问题可以参考[IDA常见问题](https://blog.csdn.net/binbin594738977/article/details/106058742)
- [附加GitHub地址下载apk](https://github.com/binbin594738977/AndroidIDA)

# 配置
1. 复制IDA 安装目录下dbgsrv 文件里面的android_server文件到 手机内存储的 /data/local/tmp 目录下
2. 控制台输入 `adb shell su` 命令获取管理员权限
3. 控制台输入 `setenforce 0` 命令 关闭保护(每次关机后开机都要重新设置)
4. 控制台输入:`cd /data/local/tmp` 进入android_server目录 ，输入:`chmod 777 android_server`命令获取执行权限


# 开始
1. 控制台输入 `adb shell su` 命令获取管理员权限
2. 输入: `./data/local/tmp/android_server`启动android_server
3. 新开一个命令窗口 不用获取管理员权限
4. 直接输入 `adb forward tcp:23946 tcp:23946` 命令 端口转发
5. adb shell am start -D -n 包名/activity路径 启动应用,例如:`adb shell am start -D -n com.example.androidida/com.example.androidida.MainActivity`
6. 打开IDA -> 不要选择文件
7. 然后选择debugger -> Attach -> Remote ARM linux/Android debugger
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200512101534592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JpbmJpbjU5NDczODk3Nw==)
8. hostname = 127.0.0.1   post = 23946  然后点击debug options , 设置如下图 , 设置完成后都点击ok
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200512101733398.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JpbmJpbjU5NDczODk3Nw==,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200512101858522.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JpbmJpbjU5NDczODk3Nw==,size_16,color_FFFFFF,t_70)

9. 这时候到选择进程的弹框 ,选择自己apk的进程,点ok
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200512114658291.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JpbmJpbjU5NDczODk3Nw==,size_16,color_FFFFFF,t_70)
10. 出现如下图界面,点击开始按钮(F9) ,如果有反调试的话，一定要先下断点，不然一运行就崩溃了,这里运行自己的就没事
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200512102219322.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JpbmJpbjU5NDczODk3Nw==,size_16,color_FFFFFF,t_70)

11. 打开Android Device Minitor , 查看端口,这里是8612
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200512114735115.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JpbmJpbjU5NDczODk3Nw==,size_16,color_FFFFFF,t_70)
12. 控制台输入:`jdb -connect com.sun.jdi.SocketAttach:hostname=127.0.0.1,port=8612` 注意:我这里是8612,自己的是多少就填多少, 输完这时候Android Device Minitor的小瓢虫会变成绿色,并且IDA程序会开始运行



13. **IDA一直按运行按钮(F9) , 直到加载完 , 有以下几点注意:之后会有弹框,有ok按ok , 有yes按yes , 由于本次app在启动MainActivity不会加载so包,所以一直按到运行按钮不能按**


14. 点击IDA的菜单栏的Debugger菜单中的**Debugger options**项,按照如下弹框选择(这是上面已经做过的,但是这里还是需要重新打开一下)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200511162229985.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JpbmJpbjU5NDczODk3Nw==,size_16,color_FFFFFF,t_70)

15. 测试apk点击按钮进入加载so的页面,这时候会断自动断点
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200512122644527.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JpbmJpbjU5NDczODk3Nw==,size_16,color_FFFFFF,t_70)
16. 这里要慢点按运行按钮 , 每次运行查看so包是否加载了 (control + s键可以查看已经加载的so包)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200512115438773.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JpbmJpbjU5NDczODk3Nw==,size_16,color_FFFFFF,t_70)


17. 加载so包之后, 在general registers查看so包的函数(ctrl +F可以搜索) , 双击so包
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200511165248936.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JpbmJpbjU5NDczODk3Nw==,size_16,color_FFFFFF,t_70)
18. 搜索自己定义的函数名称,双击函数名
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200511165554621.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JpbmJpbjU5NDczODk3Nw==,size_16,color_FFFFFF,t_70)
19. 双击函数名后,在Debug View试图会定位到函数位置,按F2 可以给选中行下断点,如下:
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200511165937928.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JpbmJpbjU5NDczODk3Nw==,size_16,color_FFFFFF,t_70)

20. 下好断点之后,按钮IDA的运行按钮(F9) , 一直到指示线条变色,说明已经断点成功(提示: F8运行下一个断点)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200512103919448.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JpbmJpbjU5NDczODk3Nw==,size_16,color_FFFFFF,t_70)

# 总结
这个动态调试的第二种方案 , 并没有结合静态so包 ,这种方式的优点相比动态和静态结合 , 这个方式更加快速简单 , 缺点就是这样调试 ,有很多功能是不能用的