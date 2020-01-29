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
//#include <ArduinoJson.h>


int value = 0;
int led_sta = 0;

const char* ssid = "Tigerots";//wifi账号 这里需要修改
const char* password = "9955995599";//wifi密码 这里需要修改

//创建 tcp server 端口号是80
WiFiServer server(80);

void setup()
{
    Serial.begin(115200);
    Serial.println();

    Serial.printf("Connecting to %s ", ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" connected");
    //启动TCP 连接
    server.begin();
    //打印TCP server IP地址
    Serial.printf("Web server started, open %s in a web browser\n", WiFi.localIP().toString().c_str());
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

void my_led_sta(void)
{
    my_led_run(led_sta);
}


void loop(void)
{
	my_led_run(1);
	delay(50);
}
