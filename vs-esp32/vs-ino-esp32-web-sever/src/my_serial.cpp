
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

#define	 __MY_SERIAL_APP

#include <Arduino.h>
#include <my_serial.h>


/****************************************************************************
* 编写作者: Tigerots
* 创建时间: 2020.02
* 功能描述: 
* 入口参数: 
*****************************************************************************/
void my_seial_init(int band)
{
    Serial.begin(band);
}


