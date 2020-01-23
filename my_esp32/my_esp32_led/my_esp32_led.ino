/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */
#include <Arduino.h>
#include <WiFi.h>
#include <Ticker.h>
#include <ArduinoJson.h>


int value = 0;
int led_sta = 0;

Ticker tickerLedrun;

//以下三个定义为调试定义
#define DebugBegin(baud_rate)    Serial.begin(baud_rate)
#define DebugPrintln(message)    Serial.println(message)
#define DebugPrint(message)    Serial.print(message)
 
const char* ssid     = "Tigerots";        // XXXXXX -- 使用时请修改为当前你的 wifi ssid
const char* password = "9955995599";      // XXXXXX -- 使用时请修改为当前你的 wifi 密码
const char* host = "api.seniverse.com";
const char* APIKEY = "wcmquevztdy1jpca";  //API KEY
const char* city = "beijing";
const char* language = "zh-Hans";         //zh-Hans 简体中文  会显示乱码
  
const unsigned long BAUD_RATE = 115200;   // serial connection speed
const unsigned long HTTP_TIMEOUT = 5000;  // max respone time from server
const size_t MAX_CONTENT_SIZE = 1000;     // max size of the HTTP response
 
// 我们要从此网页中提取的数据的类型
struct WeatherData 
{
    char city[16];//城市名称
    char weather[32];//天气介绍（多云...）
    char temp[16];//温度
    char udate[32];//更新时间
};
  
WiFiClient client;
char response[MAX_CONTENT_SIZE];
char endOfHeaders[] = "\r\n\r\n";

 
void setup()
{
    // 每隔xs调用一次
    led_sta = 0;
    tickerLedrun.attach_ms(100, my_led_sta);
    delay(10);

    WiFi.mode(WIFI_STA);     //设置esp8266 工作模式
    DebugBegin(BAUD_RATE);
    DebugPrint("Connecting to "); //写几句提示，哈哈
    DebugPrintln(ssid);
    WiFi.begin(ssid, password);   //连接wifi
    WiFi.setAutoConnect(true);
    while (WiFi.status() != WL_CONNECTED) 
    {
      //这个函数是wifi连接状态，返回wifi链接状态
      delay(500);
      DebugPrint(".");
    }
    DebugPrintln("");
    DebugPrintln("WiFi connected");
    delay(500);
    DebugPrintln("IP address: ");
    DebugPrintln(WiFi.localIP()); //WiFi.localIP()返回8266获得的ip地址
    client.setTimeout(HTTP_TIMEOUT);
}

void loop()
{
    //判断tcp client是否处于连接状态，不是就建立连接
    while (!client.connected())
    {
        DebugPrintln("step 1");
       if (!client.connect(host, 80))
       {
            led_sta = 1;
            DebugPrintln("connection....");
            delay(500);
       }
    }
    DebugPrintln("step 2");
    led_sta = 2;
    //发送http请求 并且跳过响应头 直接获取响应body
    if (sendRequest(host, city, APIKEY) && skipResponseHeaders()) 
    {
        //清除缓冲
        clrEsp8266ResponseBuffer();
        //读取响应数据
        readReponseContent(response, sizeof(response));
        WeatherData weatherData;
        if (parseUserData(response, &weatherData)) 
        {
            printUserData(&weatherData);
        }
    }
    delay(5000);//每5s调用一次
}


void my_led_sta(void)
{
    my_led_run(led_sta);
}

/********************************************************
 * LED指示灯闪烁函数
*********************************************************/
#define PIN_LED 2
#define my_led_off  digitalWrite(PIN_LED,LOW)
#define my_led_on   digitalWrite(PIN_LED,HIGH)
int lastTime = 0, currTime = 0;
void my_led_run(int sta)
{
    pinMode(PIN_LED, OUTPUT);// LED引脚初始化
    currTime = millis();
    switch(sta)
    {
        case 0:
            if((currTime - lastTime) > 1000)
            {
                lastTime = currTime;
                my_led_off;//关闭LED
            }
            else if((currTime - lastTime) > 900)
            {
                my_led_on;//点亮LED
            }
            else
            {
                my_led_off;//关闭LED
            }
        break;
        case 1:
            if((currTime - lastTime) > 1000)
            {
                lastTime = currTime;
                my_led_off;//关闭LED
            }
            else if((currTime - lastTime) > 500)
            {
                my_led_on;//点亮LED
            }
            else
            {
                my_led_off;//关闭LED
            }
        break;
        case 2:
            if((currTime - lastTime) > 2000)
            {
                lastTime = currTime;
                my_led_off;
            }
            else if((currTime - lastTime) > 1900)
            {
                my_led_on;
            }
            else if((currTime - lastTime) > 1800)
            {
                my_led_off;
            }
            else if((currTime - lastTime) > 1700)
            {
                my_led_on;
            }
            else
            {
                my_led_off;
            }
        break;
        default:
          my_led_on;
        break;
    }
}
//串口接收函数
void my_serial_read(void)
{
    //如果缓冲区中有数据,则读取并输出
    if(Serial.available()>0)
    {
        char ch = Serial.read();
        Serial.print(ch);
        if((ch<0x33)&&(ch>=0x30))
        {
            led_sta = ch-0x30;
        }
    }
}





/**
* @发送http请求指令
*/
bool sendRequest(const char* host, const char* cityid, const char* apiKey) {
  // We now create a URI for the request
  //心知天气  发送http请求
  String GetUrl = "/v3/weather/now.json?key=";
  GetUrl += apiKey;
  GetUrl += "&location=";
  GetUrl += city;
  GetUrl += "&language=";
  GetUrl += language;
  // This will send the request to the server
  client.print(String("GET ") + GetUrl + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  DebugPrintln("create a request:");
  DebugPrintln(String("GET ") + GetUrl + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n");
  delay(1000);
  return true;
}
  
/**
* @Desc 跳过 HTTP 头，使我们在响应正文的开头
*/
bool skipResponseHeaders() {
  // HTTP headers end with an empty line
  bool ok = client.find(endOfHeaders);
  if (!ok) {
    DebugPrintln("No response or invalid response!");
  }
  return ok;
}
  
/**
* @Desc 从HTTP服务器响应中读取正文
*/
void readReponseContent(char* content, size_t maxSize) {
  size_t length = client.readBytes(content, maxSize);
  delay(100);
  DebugPrintln("Get the data from Internet!");
  content[length] = 0;
  DebugPrintln(content);
  DebugPrintln("Read data Over!");
  client.flush();//清除一下缓冲
}
  
/**
 * @Desc 解析数据 Json解析
 * 数据格式如下：
 * {
 *    "results": [
 *        {
 *            "location": {
 *                "id": "WX4FBXXFKE4F",
 *                "name": "北京",
 *                "country": "CN",
 *                "path": "北京,北京,中国",
 *                "timezone": "Asia/Shanghai",
 *                "timezone_offset": "+08:00"
 *            },
 *            "now": {
 *                "text": "多云",
 *                "code": "4",
 *                "temperature": "23"
 *            },
 *            "last_update": "2017-09-13T09:51:00+08:00"
 *        }
 *    ]
 *}
 */
bool parseUserData(char* content, struct WeatherData* weatherData) {
//    -- 根据我们需要解析的数据来计算JSON缓冲区最佳大小
//   如果你使用StaticJsonBuffer时才需要
//    const size_t BUFFER_SIZE = 1024;
//   在堆栈上分配一个临时内存池
//    StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
//    -- 如果堆栈的内存池太大，使用 DynamicJsonBuffer jsonBuffer 代替
  DynamicJsonBuffer jsonBuffer;
   
  JsonObject& root = jsonBuffer.parseObject(content);
   
  if (!root.success()) {
    DebugPrintln("JSON parsing failed!");
    return false;
  }
    
  //复制我们感兴趣的字符串
  strcpy(weatherData->city, root["results"][0]["location"]["name"]);
  strcpy(weatherData->weather, root["results"][0]["now"]["text"]);
  strcpy(weatherData->temp, root["results"][0]["now"]["temperature"]);
  strcpy(weatherData->udate, root["results"][0]["last_update"]);
  //  -- 这不是强制复制，你可以使用指针，因为他们是指向“内容”缓冲区内，所以你需要确保
  //   当你读取字符串时它仍在内存中
  return true;
}
   
// 打印从JSON中提取的数据
void printUserData(const struct WeatherData* weatherData) {
  DebugPrintln("Print parsed data :");
  DebugPrint("City : ");
  DebugPrint(weatherData->city);
  DebugPrint(", \t");
  DebugPrint("Weather : ");
  DebugPrint(weatherData->weather);
  DebugPrint(",\t");
  DebugPrint("Temp : ");
  DebugPrint(weatherData->temp);
  DebugPrint(" C");
  DebugPrint(",\t");
  DebugPrint("Last Updata : ");
  DebugPrint(weatherData->udate);
  DebugPrintln("\r\n");
}
   
// 关闭与HTTP服务器连接
void stopConnect() 
{
  DebugPrintln("Disconnect");
  client.stop();
}
  
void clrEsp8266ResponseBuffer(void)
{
    memset(response, 0, MAX_CONTENT_SIZE);      //清空
}
