目前发现很多人 将改配网库修改 进行商业项目 或不注明来源 强制做为自己项目在网络中分享。 请尊重开源协议，谢谢！  

# wifi_link_tool
这是一个为esp8266设计的通用配网库   
该库免费提供 使用请注明出处 允许毕业设计使用本库   
请在发行页面下载稳定的发布版本  
## 当前版本v1.1.0 开源协议：GPL3.0
* 如果是旧版库项目安装目前最新版请在项目第一行加入 #define FS_CONFIG 来启动fs模式
# v1.1.x版本视频：https://www.bilibili.com/video/BV1Ek4y1r75J?p=2
# 操作视频：https://www.bilibili.com/video/BV1Ek4y1r75J/

****
![img](./img/index.jpg)
![img](./img/index2.jpg)
****
## 普通配网流程 1.链接WiFi 2.自动弹出配网或打开浏览器访问6.6.6.6 3.输入WiFi密码   
## 微信小程序配网 使用微信扫描二维码 打开小程序 点击通用设备配网  输入WiFi密码
****
# 关于配网操作
* 默认情况下 下拉gpio0(nodemcu D3可以按flash按键)5秒 重置已有配网 第一次使用该库请重置一次或用官方工具擦除flash  
* 默认情况下 状态指示灯gpio02（nodemcu D4） 也就是12f的板载led  
* 指示灯常亮 未链接网络 处于未配置 或 无法链接网络  
* 指示灯闪烁 正在抹除配置  
* 指示灯熄灭 联网正常  

# 微信小程序配网
## 使用微信扫一扫进行加载小程序

![image](./img/wxxcx.jpg)

## 小程序页面
![image](./img/wx1.jpg)  

![image](./img/wx2.jpg)  
点击通用设备配网 即可弹出配网 如未弹出请检查文件系统是否上传成功！  
点击设备列表可搜索局域网中可被搜索的设备

# 普通配网
## 扫描二维码直接链接或手动链接 SSID：wifi_link_tool
### 该二维码支持国内大部分手机相机 （例如使用MIUI系统相机）
![image](/img/ptlink.png)  
链接成功后 会弹出配网web 如为弹出 请检查设备支持性（安卓4.4.4以上大部分支持 ios 11+）和文件系统是否上传成功！

# 安装库wif link tool 库
## 安装库本身
* 请保证已经安装了 arduino esp8266 sdk （已在2.7.1测试）
* 在github上下载本库
* 打开arduino  项目—>加载库—>添加.zip库 —>选择刚才下载的zip导入 (手动库安装目录Arduino\libraries\WiFi_link_tool)

# 二进制固化模式
* 二进制固化模式为默认模式 不过还是推荐使用文件系统方式 固化方式是将web页直接放入程序空间  
## 测试
* 打开测试demo 文件—>示例—>wifi_link_tool—>hex_demo
* 选择串口及波特率
* 点击验证 验证库是否能正常编译

# 文件系统模式
* 使用文件系统模式请在项目第一行加入 #define FS_CONFIG 来激活文件系统模式
## 安装文件系统
* 下载文件系统上传工具 [工具地址](https://github.com/esp8266/arduino-esp8266fs-plugin/releases)
* 导入工具参考目录 ******/tools/ESP8266FS/tool/esp8266fs.jar  
* 重启arduino

## 测试
* 打开测试demo 文件—>示例—>wifi_link_tool—>fs_demo
* 选择串口及波特率
* 选择合适的flash size
![image](./img/flash.jpg)  
* 点击验证 验证库是否能正常编译
* 检查文件系统上传工具  工具下会显示 ESP8266 sketch Data Upload 证明安装成功
* 上传文件系统 工具 —>ESP8266 sketch Data Upload 

# 库基础配置及api
* 配置设备  
在setup中配置设备io名称  
    rstb=D3;  
    //重置io  
    stateled=D4;  
    //指示灯io  
    Hostname = "ESP8266";  
    //设备名称 允许中文名称 不建议太长  
    wxscan=true;  
    //是否被小程序发现设备 开启意味该设备具有后台 true开启 false关闭  
* load()函数 加载启动等 必须存在

* pant()函数 维持系统正常运行 请保证在loop 函数的第一行 保证系统稳定性

* link()函数 (有值发回)获取当前WiFi配置状态   
 注意请在load()函数之后或在loop区块中使用  
 返回类型 int 返回内容 0 当前未配置WiFi 1 以配置WiFi  
 作用 识别当前是否已经配网实现数码管或oled屏幕内容的显示与切换  
 示例：   if(link()){/*设备以配置网络触发内容*/}else{/*设备未配置网络触发内容*/}
 
 * torest() 函数 无返回值   
  作用软重置配网！

* 局域网中获得设备名称API(用来配合网关或设备通信)    
该api以集成进库中  
作用 获得设备设备名称 方便设备联动  
请求方法 http get 返回类型 String  
地址 http://x.x.x.x/opera?opera=sb

* 自定义http get请求   
语法： webServer.on("/<请求地址>", <处理函数>);   
请写在setup函数中 load函数之后  
处理函数请自定义一个新函数  
请参考demo中get请求处理

* 自定义添加web
文件系统模式下 库自动添加 config.html(配网web) index.html(设备主页)  
将自定义的html 放入Data目录下（将demo另存为 在另存目录就会出现data目录）  
语法： webServer.on("/<请求地址>", <加载函数>);  
请写在setup函数中 load函数之后  
加载函数 语法 void <加载函数>(){File file = SPIFFS.open("/<自定义html文件名>", "r"); webServer.streamFile(file, "text/html");file.close(); }  
请参考demo中加载多个web  
二进制固化模式下  
将自定义的html 使用tool目录中所提供的工具进行二进制转换， 如果页面中存在中文请先转换成UTF-8编码后转换
语法： webServer.on("/<请求地址>", <加载函数>);  
请写在setup函数中 load函数之后  
加载函数 语法 void <加载函数>() { wifi_link_tool_hex(200, "text/html", <二进制数组>, sizeof(二进制数组));} //该函数必须在固化模式下才可使用
请参考示例中命名规则  


# 版本更新说明
****
* v1.0.1：基础库发布！
* v1.0.2：更新函数link() //获取当前WiFi配置状态
* v1.0.3：更新函数torest()//软重置配网
* v1.0.4：更新兼容性，以适配最新sdk（2.7.1），修复了一些语法问题
* v1.0.5：修复严重配网BUG（在链接WiFi中重置会导致无限重启） ，优化了结构。
* v1.0.6：修复了臃肿ui(例如miui)和ios扫描出WiFi点击不弹出密码框。
* v1.0.7：修复配网后正常联网某些手机会出现一个乱码WiFi，可能sdk的bug 解决思路是首先建立隐藏网络在识别配网状态。
* v1.1.0：双模式配网，全新的配网页面，增加wifi_link_tool_hex()函数 修复一下语法错误  如果是旧项目升级请在第一行加入  #define FS_CONFIG 
****



## 测试环境  sdk版本：2.7.1 arduino版本1.8.8

