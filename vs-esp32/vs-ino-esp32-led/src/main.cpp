/*
 *  
 *
 */

#include <Arduino.h>
#include <ArduinoJson.h>

#include <my_led.h>
#include <my_serial.h>
#include <my_wifi.h>

void setup()
{
	my_led_init();//LED 指示灯初始化
	my_seial_init(115200);//串口初始化
	my_wifi_init();//连接到wifi
}


void loop(void)
{
	led_sta = 0;
	delay(5000);
	led_sta = 1;
	delay(5000);
	led_sta = 2;
	delay(5000);
	led_sta = 3;
	delay(5000);
}
