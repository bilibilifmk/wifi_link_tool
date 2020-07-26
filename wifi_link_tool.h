/*
wifi link tool 配网库
by:发明控 
版本v1.0.7
测试环境 sdk版本：2.7.1 arduino版本1.8.8
项目地址：https://github.com/bilibilifmk/wifi_link_tool 项目成员：发明控 狗腿 
*/





#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <FS.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>
void  ICACHE_RAM_ATTR blink();
const char *AP_name = "wifi_link_tool"; //修改后即不支持微信配网



/////////////////////////////////////////////////////不建议修改部分//////////////////////////////////////////////////////////////////
String Hostname = "ESP8266";     
String url = Hostname + ".com";   
int Signal_filtering = -200;  
const byte DNS_PORT = 53;
String WiFi_State;   
#define WiFi_State_Addr 0   
bool wxscan=true;
int rstb=0;
int stateled=2;
IPAddress apIP(6, 6, 6, 6);
DNSServer dnsServer;
ESP8266WebServer webServer(80);
String responseHTML =" <html><head><title>正在跳转</title></head><body><script language='javascript'>document.location = '/'</script></body></html>";


//心跳服务
void pant()
{
   MDNS.update();
   dnsServer.processNextRequest();
   webServer.handleClient();
}


void torest()
{
    WiFi.disconnect(true);
    delay(100);
    EEPROM.write(WiFi_State_Addr, 0);
    EEPROM.commit();
    delay(300);
    Serial.print("重置成功！正在重启。");
    ESP.restart();
}

int link()
{
  int a=0;
  if(WiFi_State=="1")
  {
    a=1;
  }
  return a; 
}

void wwwroot()
{
  
  if (WiFi_State == "1")
  {
    File file = SPIFFS.open("/index.html", "r");
    webServer.streamFile(file, "text/html");
    file.close();
  }
  else if (WiFi_State == "0")
  {
    File file = SPIFFS.open("/config.html", "r");
    webServer.streamFile(file, "text/html");
    file.close();
  }
}


void wifiConfig()
{
 
  if (webServer.hasArg("ssid") && webServer.hasArg("password") && WiFi_State == "0")
  {
    int ssid_len = webServer.arg("ssid").length();
    int password_len = webServer.arg("password").length();
  
    if ((ssid_len > 0) && (ssid_len < 33) && (password_len > 7) && (password_len < 65))
    {
      
      String ssid_str = webServer.arg("ssid");
      String password_str = webServer.arg("password");
      const char *ssid = ssid_str.c_str();
      const char *password = password_str.c_str();
      Serial.print("SSID: ");
      Serial.println(ssid);
      Serial.print("Password: ");
      Serial.println(password);
    
      WiFi.begin(ssid, password);
      Serial.print("Connenting");
    
      unsigned long millis_time = millis();
      while ((WiFi.status() != WL_CONNECTED) && (millis() - millis_time < 8000))
      {
        delay(500);
        Serial.print(".");
      }
 
      if (WiFi.status() == WL_CONNECTED)
      {
       
        digitalWrite(stateled, HIGH);
        Serial.println("");
        Serial.println("Connected successfully!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        Serial.print("http://");
        Serial.println(Hostname);
        webServer.send(200, "text/plain", "1");
        delay(300);
        WiFi_State = "1";
      
        EEPROM.write(WiFi_State_Addr, 1);
        EEPROM.commit();
        delay(50);
    
        WiFi.softAPdisconnect();
        delay(1000);
        ESP.restart();
      }
      else
      {
        
        Serial.println("Connenting failed!");
        webServer.send(200, "text/plain", "0");
      }
    }
    else
    {
     
      Serial.println("Password format error");
      webServer.send(200, "text/plain", "Password format error");
    }
  }
  else
  {
   
    Serial.println("Request parameter error");
    webServer.send(200, "text/plain", "Request parameter error");
  }
}


String wifi_type(int typecode)
{
  if (typecode == ENC_TYPE_NONE) return "Open";
  if (typecode == ENC_TYPE_WEP) return "WEP ";
  if (typecode == ENC_TYPE_TKIP) return "WPA ";
  if (typecode == ENC_TYPE_CCMP) return "WPA2";
  if (typecode == ENC_TYPE_AUTO) return "WPA*";
}


void wifiScan()
{
  String req_json = "";
  Serial.println("Scan WiFi");
  int n = WiFi.scanNetworks();
  if (n > 0)
  {
    int m = 0;
    req_json = "{\"req\":[";
    for (int i = 0; i < n; i++)
    {
    if ((int)WiFi.RSSI(i) >= Signal_filtering)
     //  if (1)
      {
        m++;
        String a="{\"ssid\":\"" + (String)WiFi.SSID(i) + "\"," + "\"encryptionType\":\"" + wifi_type(WiFi.encryptionType(i)) + "\"," + "\"rssi\":" + (int)WiFi.RSSI(i) + "},";
        if(a.length()>15)
        req_json += a;  
        }
    }
    req_json.remove(req_json.length() - 1);
  
    req_json += "]}";
    webServer.send(200, "text/json;charset=UTF-8", req_json);
    Serial.print("Found ");
    Serial.print(m);
    Serial.print(" WiFi!  >");
    Serial.print(Signal_filtering);
    Serial.println("dB");
  }
}

void opera() {
  if(webServer.arg("opera") == "sb"){
    
  webServer.send(200, "text/plain", Hostname);
    
  }
  
}



void blink() {
  Serial.println("Keep 3 seconds to reset!");
  bool res_state = true;
  unsigned int res_time = millis();
  
  while (millis() - res_time < 3000)
  {
      ESP.wdtFeed();//喂狗
    if (digitalRead(rstb) != LOW)
    {
      res_state = false;
      Serial.println("Cancel reset!");
      break;
    }
  }
  if (res_state == true)
  {
     EEPROM.write(WiFi_State_Addr, 0);
     EEPROM.commit();
    if (WiFi.status() == WL_CONNECTED)
     {
     WiFi.disconnect(true);
     }

   
    delay(300);
    Serial.println("Press the 'rst' button!");
    while (1) {
      digitalWrite(stateled, LOW);
      delay(250);
      digitalWrite(stateled, HIGH);
      delay(250);
    }
     Serial.println("重置!");
     ESP.restart();
  }
}


//加载部分
void load(){ 
 WiFi.softAP("wif_link_tool", "00000000", 3, 1);
 attachInterrupt(digitalPinToInterrupt(rstb), blink, FALLING);
  Serial.println("");
  EEPROM.begin(4096);
  SPIFFS.begin();
  WiFi.hostname(Hostname);
  pinMode(stateled, OUTPUT);
  WiFi_State = EEPROM.read(WiFi_State_Addr);
  delay(300);
  if (WiFi_State == "1")  
  {
    WiFi.mode(WIFI_STA);
    Serial.println("找到配置!");
    Serial.print("链接网络");
      delay(500);
    unsigned millis_time = millis();
    while ((WiFi.status() != WL_CONNECTED) && (millis() - millis_time < 5000))
    {
      delay(250);
      ESP.wdtFeed();//喂狗
      Serial.print(".");
    }
    if (wxscan){
      if (MDNS.begin(Hostname)) { Serial.println("mDNS以启动"); }
      MDNS.addService("http", "tcp", 80);
                }

    Serial.println("");
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.print("IP 地址: ");
      Serial.println(WiFi.localIP());

      Serial.print("http://");
      Serial.println(Hostname);
      digitalWrite(stateled, HIGH);
    }
    else
    {
      Serial.println("链接失败!");
      Serial.println("请尝试重置系统!");
      digitalWrite(stateled, LOW);
      delay(5000);
    }
  }
  else if (WiFi_State == "0")   
  {
   
    digitalWrite(stateled, LOW);
    WiFi.disconnect(true);
    Serial.println("");
    Serial.print("启动WiFi配置 \n建立AP 名称 -->  ");
    Serial.println(AP_name);
    Serial.print("http://");
    Serial.println(url);
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(AP_name);
  }
  else
  {
    Serial.println("初次启动");
    EEPROM.write(WiFi_State_Addr, 0);
    EEPROM.commit();
    delay(300);
   
    Serial.println("请重置系统！");
    while (1) {
      digitalWrite(stateled, LOW);
      delay(250);
      digitalWrite(stateled, HIGH);
      delay(250);
    }
  }



  Serial.println("启动http服务");
  webServer.on("/", wwwroot);
  webServer.on("/wificonfig", wifiConfig);
  webServer.on("/wifiscan", wifiScan);
  webServer.on("/opera", opera);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  dnsServer.start(DNS_PORT, "*", apIP);
  webServer.onNotFound([]() {
      webServer.send(200, "text/html", responseHTML);
    });
    webServer.begin();
  }
