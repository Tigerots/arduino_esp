/*
  To upload through terminal you can use: curl -F "image=@firmware.bin" esp8266-webupdate.local/update
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>

const char* host = "esp32-webupdate";

const char* ssid = "Tigerots";
const char* password = "9955995599";

WebServer server(80);
const char* serverIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";

void setup(void) 
{
    static int rxCnt = 0;
    Serial.begin(115200);
    Serial.println();
    Serial.println("Booting Sketch...");
    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() == WL_CONNECTED) 
    {
        MDNS.begin(host);
        server.on("/", HTTP_GET, 
                                []() 
                                {
                                  server.sendHeader("Connection", "close");
                                  server.send(200, "text/html", serverIndex);
                                });
        
        /**
        * 配置uri对应的handler，handler也就是处理方法
        * @param  uri  const String (uri路径)
        * @param  method  HTTPMethod（Http请求方法）
        *         可选参数：HTTP_ANY, HTTP_GET, HTTP_POST, HTTP_PUT, 
        *                   HTTP_PATCH, HTTP_DELETE, HTTP_OPTIONS
        * @param  fn  THandlerFunction  （对应uri处理函数）
        * @param  ufn THandlerFunction  （文件上传处理函数）
        */
        server.on("/update", HTTP_POST, 
                                []() 
                                {
                                    server.sendHeader("Connection", "close");
                                    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");// 升级结果
                                    ESP.restart();// 重启
                                }, 
                                []() 
                                {
                                    HTTPUpload & upload = server.upload();
                                    if (upload.status == UPLOAD_FILE_START) 
                                    {
                                        Serial.setDebugOutput(true);
                                        Serial.printf("Update: %s\n", upload.filename.c_str());
                                        if (!Update.begin()) 
                                        { //start with max available size
                                            Update.printError(Serial);
                                        }
                                        rxCnt = 0;
                                    } 
                                    else if (upload.status == UPLOAD_FILE_WRITE) 
                                    {
                                        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) 
                                        {
                                            Update.printError(Serial);
                                        }
                                        rxCnt += upload.currentSize;
                                        Serial.printf("Update size: %d\n", rxCnt);
                                    } 
                                    else if (upload.status == UPLOAD_FILE_END) 
                                    {
                                        if (Update.end(true)) 
                                        { //true to set the size to the current progress
                                            Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
                                        } 
                                        else 
                                        {
                                            Update.printError(Serial);
                                        }
                                        Serial.setDebugOutput(false);
                                    } 
                                    else 
                                    {
                                        Serial.printf("Update Failed Unexpectedly (likely broken connection): status=%d\n", upload.status);
                                    }
                                });
                                
        server.begin();
        MDNS.addService("http", "tcp", 80);
    
        Serial.printf("Ready! Open http://%s.local in your browser\n", host);
    } 
    else 
    {
        Serial.println("WiFi Failed");
    }
}

void loop(void) 
{
    server.handleClient();
    delay(1);
}
