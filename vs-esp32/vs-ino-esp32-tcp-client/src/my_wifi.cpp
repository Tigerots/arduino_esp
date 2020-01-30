
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

/*  Tcp status:
 *			CLOSED      = 0,
 *          LISTEN      = 1,
 *          SYN_SENT    = 2,
 *          SYN_RCVD    = 3,
 *          ESTABLISHED = 4,
 *          FIN_WAIT_1  = 5,
 *          FIN_WAIT_2  = 6,
 *          CLOSE_WAIT  = 7,
 *          CLOSING     = 8,
 *          LAST_ACK    = 9,
 *          TIME_WAIT   = 10
*/
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

/*  wifi  status: 
	255：WL_NO_SHIELD不用在意（兼容WiFi Shield而设计）
	0：WL_IDLE_STATUS正在WiFi工作模式间切换；
	1：WL_NO_SSID_AVAIL无法访问设置的SSID网络；
	2：WL_SCAN_COMPLETED扫描完成；
	3：WL_CONNECTED连接成功；
	4：WL_CONNECT_FAILED连接失败；
	5：WL_CONNECTION_LOST丢失连接；
	6：WL_DISCONNECTED断开连接；
*/
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
	delay(1000);
	Serial.println();
	Serial.println("=== Wifi is connected : ===");
	Serial.print("wifi  status: ");
	Serial.println(WiFi.status());
	Serial.print("localIP address: ");
	Serial.println(WiFi.localIP());
	Serial.print("gatewayIP  address: ");
	Serial.println(WiFi.gatewayIP());
	Serial.print("subnetMask  address: ");
	Serial.println(WiFi.subnetMask());
	Serial.print("dnsIP  address: ");
	Serial.println(WiFi.dnsIP());
	Serial.printf("RSSI: %d dBm\n", WiFi.RSSI());
	Serial.println();

}



