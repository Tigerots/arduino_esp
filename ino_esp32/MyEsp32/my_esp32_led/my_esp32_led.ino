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


int value = 0;
int led_sta = 0;

Ticker tickerSetLow,tickerSetHigh;

const char* ssid     = "Tigerots";
const char* password = "9955995599";

const char* host = "data.sparkfun.com";
const char* streamId   = "....................";
const char* privateKey = "....................";

void set_led_sta(int sta)
{
    led_sta = sta;
}

void setup()
{
    Serial.begin(115200);//打开串口
    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // 每隔xs调用一次
    tickerSetLow.attach(20, set_led_sta, 0);
    tickerSetHigh.attach(9, set_led_sta, 2);
}

void loop()
{
      my_led_run(led_sta);
      my_serial_read();
    
//    delay(5000);
//    ++value;
//
//    Serial.print("connecting to ");
//    Serial.println(host);
//
//    // Use WiFiClient class to create TCP connections
//    WiFiClient client;
//    const int httpPort = 80;
//    if (!client.connect(host, httpPort)) {
//        Serial.println("connection failed");
//        return;
//    }
//
//    // We now create a URI for the request
//    String url = "/input/";
//    url += streamId;
//    url += "?private_key=";
//    url += privateKey;
//    url += "&value=";
//    url += value;
//
//    Serial.print("Requesting URL: ");
//    Serial.println(url);
//
//    // This will send the request to the server
//    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
//                 "Host: " + host + "\r\n" +
//                 "Connection: close\r\n\r\n");
//    unsigned long timeout = millis();
//    while (client.available() == 0) {
//        if (millis() - timeout > 5000) {
//            Serial.println(">>> Client Timeout !");
//            client.stop();
//            return;
//        }
//    }
//
//    // Read all the lines of the reply from server and print them to Serial
//    while(client.available()) {
//        String line = client.readStringUntil('\r');
//        Serial.print(line);
//    }
//
//    Serial.println();
//    Serial.println("closing connection");
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
