
/**************************** (C) COPYRIGHT 2020 ****************************
* 设计师:   Tigerots
* 创建时间: 2020.02
* 功能描述: 
*       
*****************************************************************************
* ********************************修改历史记录********************************
* 修改时间: 
* 版本号:
* 修改内容:
*****************************************************************************/
#define	 __MY_WIFI_APP

#include <WiFi.h>
#include <Arduino.h>

#include <my_led.h>
#include <my_serial.h>
#include <my_html.h>


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
	String  index_page = read_page();

	String htmlPage =
		String("HTTP/1.1 200 OK\r\n") +
			"Content-Type: text/html\r\n" +
			"Connection: close\r\n" + 
			"Refresh: 20\r\n" +  
			"\r\n" +
			index_page +
			"\r\n";
	
	return htmlPage;
}

/****************************************************************************
* 编写作者: Tigerots
* 创建时间: 2020.02
* 功能描述: web 服务器循环执行函数
*****************************************************************************/
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


/****************************************************************************
* 编写作者: Tigerots
* 创建时间: 2020.02
* 功能描述: 
			1. 初始化串口(打印数据)
			2. 初始化wifi
			3. 初始化TCP,监听端口
* 入口参数:
* 出口参数:
		wifi  status 说明: 
		255：WL_NO_SHIELD不用在意（兼容WiFi Shield而设计）
		0：WL_IDLE_STATUS正在WiFi工作模式间切换；
		1：WL_NO_SSID_AVAIL无法访问设置的SSID网络；
		2：WL_SCAN_COMPLETED扫描完成；
		3：WL_CONNECTED连接成功；
		4：WL_CONNECT_FAILED连接失败；
		5：WL_CONNECTION_LOST丢失连接；
		6：WL_DISCONNECTED断开连接；
* 修改记录:
*****************************************************************************/
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
	Serial.println("=== Wifi is connected ! ===");
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
	Serial.printf("=== Web server started, open %s in a web browser ! ===\r\n\r\n", WiFi.localIP().toString().c_str());
	delay(1000);
}
/****************************************************************************
* 编写作者: Tigerots
* 创建时间: 2020.02
* 功能描述: 
			1. 初始化串口(打印数据)
			2. 使用微信或者APP自动配网
*****************************************************************************/
void my_smartconfig(void) 
{
	Serial.begin(115200);

	//Init WiFi as Station, start SmartConfig
	WiFi.mode(WIFI_AP_STA);
	WiFi.beginSmartConfig();

	//Wait for SmartConfig packet from mobile
	Serial.println("Waiting for SmartConfig.");
	while (!WiFi.smartConfigDone()) 
	{
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.println("SmartConfig received.");

	//Wait for WiFi to connect to AP
	Serial.println("Waiting for WiFi");
	while (WiFi.status() != WL_CONNECTED) 
	{
		delay(500);
		Serial.print(".");
	}

	Serial.println("WiFi Connected.");

	Serial.print("IP Address: ");
	Serial.println(WiFi.localIP());
}


