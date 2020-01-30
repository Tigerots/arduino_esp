
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

const char* ssid = "Tigerots";//wifi账号 这里需要修改
const char* password = "9955995599";//wifi密码 这里需要修改

//创建 tcp server 端口号是80, 最大4个客户端
WiFiServer server(80);
WiFiServer serverClients[4];

/**
 * 模拟web server 返回http web响应内容
 * 这里是手动拼接HTTP响应内容
 * 后面楼主会继续讲解另外两个专用于http请求的库
 */
String prepareHtmlPage()
{
	String htmlPage =
		String("HTTP/1.1 200 OK\r\n") +
			"Content-Type: text/html\r\n" +
			"Connection: close\r\n" +  // the connection will be closed after completion of the response
			"Refresh: 5\r\n" +  // refresh the page automatically every 5 sec
			"\r\n" +
			"<!DOCTYPE HTML>" +
			"<html>" +
			"Analog input:  " + String(analogRead(A0)) +
			"</html>" +
			"\r\n";
	return htmlPage;
}

void my_web_sever_loop(void)
{
	WiFiClient client = server.available();
	// wait for a client (web browser) to connect
	if (client)
	{
		Serial.println("\n[Client connected]");
		while (client.connected())
		{
			// 不断读取请求内容
			if (client.available())
			{
				String line = client.readStringUntil('\r');
				Serial.print(line);
				// wait for end of client's request, that is marked with an empty line
				if (line.length() == 1 && line[0] == '\n')
				{
					//返回响应内容
					client.println(prepareHtmlPage());
					break;
				}
			}
		//由于我们设置了 Connection: close  当我们响应数据之后就会自动断开连接
		}
		delay(100); // give the web browser time to receive the data

		// close the connection:
		client.stop();
		Serial.println("[Client disonnected]");
	}
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
	//启动TCP 连接
	server.begin();
	//关闭小包合并包功能，不会延时发送数据
	server.setNoDelay(true);
	//打印TCP server IP地址
	Serial.printf("Web server started, open %s in a web browser\n", WiFi.localIP().toString().c_str());
}



