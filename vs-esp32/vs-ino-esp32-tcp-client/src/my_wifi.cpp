
/**
 * Demo：
 *    演示web Server功能
 *    打开PC浏览器 输入IP地址。请求web server
 * @author Tigerots
 * @date 2020/01/29
 */

#include <WiFi.h>
#include <Arduino.h>

#include <my_led.h>
#include <my_serial.h>

// Wifi 连接配置
const char* ssid = "Tigerots";//wifi账号 这里需要修改
const char* password = "9955995599";//wifi密码 这里需要修改

// TCP 目标地址
const uint16_t port = 9000;
const char * host = "192.168.31.138"; 

//创建一个tcp client连接
WiFiClient client;


void my_web_sever_loop(void)
{
	Serial.print("connecting to ");
	Serial.printf("%s: %d\r", host, port);

	if (!client.connect(host, port)) 
	{
		Serial.println("connection failed");
		Serial.println("wait 5 sec...");
		delay(5000);
		return;
	}

	// 发送数据到Tcp server
	Serial.println("Send this data to server");
	client.println(String("Send this data to server"));

	//读取从server返回到响应数据
	String line = client.readStringUntil('\r');
	Serial.println(line);

	Serial.println("closing connection");
	client.stop();

	Serial.println("wait 5 sec...");
	delay(5000);
}


void my_wifi_init(void)
{
	Serial.println();
	Serial.printf("Connecting to %s ", ssid);
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	Serial.println();
	Serial.println("Wifi is connected !");
	Serial.print("localIP address: ");
	Serial.println(WiFi.localIP());
	Serial.println();
}



