/*
项目地址：https://github.com/bilibilifmk/wifi_link_tool  by:发明控 
测试环境sdk版本：2.5.0 arduino版本1.8.8
默认重置 D3（gpio 0） 默认状态led gpio2  
D3---------/ --------gnd

fsdemo  该实例为文件系统模式 

*/
//普通配网流程 1.连接WiFi 2.自动弹出配网或打开浏览器访问6.6.6.6 3.输入WiFi密码
//微信小程序配网 使用微信扫描二维码 打开小程序 点击通用设备配网  输入WiFi密码
#define FS_CONFIG
//激活文件系统模式配网
#include <wifi_link_tool.h>
//引入wifilinktool头文件
void setup() {
  Serial.begin(115200);
  //开启串口
 /////////////////////////////////////////////////////////基础设置//////////////////////////////////////////////////////////
    rstb=0;
    //重置io
    stateled=2;
    //指示灯io
    Hostname = "ESP8266";
    //设备名称 允许中文名称 不建议太长
    wxscan=true;
    //是否被小程序发现设备 开启意味该设备具有后台 true开启 false关闭
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //此处请勿添加代码
  load();
  //初始化WiFi link tool 
  //在下面添加代码
  //get请求示例（配合index.html）
  webServer.on("/gettool", gettool);
  //对应请求方法就是 ip/gettool?gettool=xxx
  //加载多个web 按照这种写法 以此类推
  //webServer.on("/lodweb", lodweb);
}
////加载多个web 按照这种写法 以此类推
//void lodweb(){File file = SPIFFS.open("/xxxx.html", "r"); webServer.streamFile(file, "text/html");file.close(); }

//get请求识别处理 
void gettool() {
  if(webServer.arg("gettool") == "call") {
    //当有请求gettool参数是call激活请求返回ojbk
    Serial.println("get");
    //串口输出
    webServer.send(200, "text/plain", "ojbk");
  }
}


void loop() {
  //请勿此处添加代码
  pant();
  //WiFi link tool 服务维持函数  请勿修改位置 
  //请在下面添加自己的代码
}
