
#include <Arduino.h>
#include <Ticker.h>
#include <my_led.h>


// LED 引脚配置
#define PIN_LED 2
#define my_led_off  digitalWrite(PIN_LED,LOW)
#define my_led_on   digitalWrite(PIN_LED,HIGH)

int led_sta = 0;
Ticker tickerLedrun;

void my_led_sta(void)
{
    my_led_run(led_sta);
}

void my_led_init(void)
{
    // LED引脚初始化
	pinMode(PIN_LED, OUTPUT);

	// 定时执行函数, 感觉相当于多线程了
	tickerLedrun.attach_ms(20, my_led_sta);
}


/********************************************************
 * LED指示灯闪烁函数
*********************************************************/
int lastTime = 0, currTime = 0;
void my_led_run(int sta)
{
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


