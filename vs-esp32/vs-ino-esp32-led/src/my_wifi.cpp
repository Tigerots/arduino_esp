

#include <Arduino.h>

#include <WiFi.h>
#include <my_led.h>
#include <my_serial.h>

const char* ssid = "Tigerots";//wifi账号 这里需要修改
const char* password = "9955995599";//wifi密码 这里需要修改


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



