
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
#define	 __MY_SPIFFS_APP

#include <my_spiffs.h>
#include <spiffs_config.h>

/****************************************************************************
* 编写作者: Tigerots
* 创建时间: 2020.02
* 功能描述: 显示目录下文件或目录
* 入口参数: SPIFFS, 文件名, 是否遍历下一级(0:否)
*****************************************************************************/
void listDir(fs::FS &fs, const char * dirname, uint8_t levels)
{
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}
/****************************************************************************
* 编写作者: Tigerots
* 创建时间: 2020.02
* 功能描述: 读文件,打印到串口
* 入口参数: SPIFFS, 路径
*****************************************************************************/
void readFile(fs::FS &fs, const char * path)
{
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return;
    }

    Serial.println("- read from file:");
    while(file.available()){
        Serial.write(file.read());
    }
}
/****************************************************************************
* 编写作者: Tigerots
* 创建时间: 2020.02
* 功能描述: 写文件
* 入口参数: SPIFFS, 路径, 写入内容
*****************************************************************************/
void writeFile(fs::FS &fs, const char * path, const char * message)
{
    Serial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("- file written");
    } else {
        Serial.println("- frite failed");
    }
}
/****************************************************************************
* 编写作者: Tigerots
* 创建时间: 2020.02
* 功能描述: 追加写入文件
* 入口参数: SPIFFS, 路径, 写入内容
*****************************************************************************/
void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\r\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("- failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("- message appended");
    } else {
        Serial.println("- append failed");
    }
}
/****************************************************************************
* 编写作者: Tigerots
* 创建时间: 2020.02
* 功能描述: 文件重命名
* 入口参数: SPIFFS, 原路径文件名, 目标路径文件名
*****************************************************************************/
void renameFile(fs::FS &fs, const char * path1, const char * path2){
    Serial.printf("Renaming file %s to %s\r\n", path1, path2);
    if (fs.rename(path1, path2)) {
        Serial.println("- file renamed");
    } else {
        Serial.println("- rename failed");
    }
}
/****************************************************************************
* 编写作者: Tigerots
* 创建时间: 2020.02
* 功能描述: 删除文件
* 入口参数: SPIFFS, 路径
*****************************************************************************/
void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\r\n", path);
    if(fs.remove(path)){
        Serial.println("- file deleted");
    } else {
        Serial.println("- delete failed");
    }
}
/****************************************************************************
* 编写作者: Tigerots
* 创建时间: 2020.02
* 功能描述: 读写文件速度测试
* 入口参数: SPIFFS, 路径
*****************************************************************************/
void testFileIO(fs::FS &fs, const char * path){
    Serial.printf("Testing file I/O with %s\r\n", path);

    static uint8_t buf[512];
    size_t len = 0;
    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }

    size_t i;
    Serial.print("- writing" );
    uint32_t start = millis();
    for(i=0; i<2048; i++){
        if ((i & 0x001F) == 0x001F){
            Serial.print(".");
        }
        file.write(buf, 512);
    }
    Serial.println("");
    uint32_t end = millis() - start;
    Serial.printf(" - %u bytes written in %u ms\r\n", 2048 * 512, end);
    file.close();

    file = fs.open(path);
    start = millis();
    end = start;
    i = 0;
    if(file && !file.isDirectory()){
        len = file.size();
        size_t flen = len;
        start = millis();
        Serial.print("- reading" );
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            if ((i++ & 0x001F) == 0x001F){
                Serial.print(".");
            }
            len -= toRead;
        }
        Serial.println("");
        end = millis() - start;
        Serial.printf("- %u bytes read in %u ms\r\n", flen, end);
        file.close();
    } else {
        Serial.println("- failed to open file for reading");
    }
}

/****************************************************************************
* 编写作者: Tigerots
* 创建时间: 2020.02
* 功能描述: SPIFS测试函数
* 入口参数:
* 出口参数:
* 修改记录:
*****************************************************************************/
void my_spiffs_sample(void)
{
    listDir(SPIFFS, "/", 0);
    writeFile(SPIFFS, "/hello.txt", "Hello ");
    appendFile(SPIFFS, "/hello.txt", "World!\r\n");
    readFile(SPIFFS, "/hello.txt");
    renameFile(SPIFFS, "/hello.txt", "/foo.txt");
    readFile(SPIFFS, "/foo.txt");
    deleteFile(SPIFFS, "/foo.txt");
    //testFileIO(SPIFFS, "/test.txt");
    //deleteFile(SPIFFS, "/test.txt");
    Serial.println( "Test complete" );
}


/****************************************************************************
* 编写作者: Tigerots
* 创建时间: 2020.02
* 功能描述: 
			1. 初始化串口（用于打印输出）
			2. 启动SPIFS组件,挂载文件系统
			3. 挂载失败是否格式化在.h中定义
            4. 格式化时间比较长
* 入口参数:
* 出口参数:
* 修改记录:
*****************************************************************************/
// 挂载失败是否格式化
#define FORMAT_SPIFFS_IF_FAILED true
void my_spiffs_init(void)
{
    Serial.begin(115200);
    if(!SPIFFS.begin(false))
    {
        Serial.println("SPIFFS Mount Failed, famating, please waiting...");
    }
    if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))
    {
        Serial.println("SPIFFS Mount Failed");
        return;
    }
}