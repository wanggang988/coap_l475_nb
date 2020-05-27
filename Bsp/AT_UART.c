/* Includes ------------------------------------------------------------------*/
#include "AT_UART.h"
/*******************************************************************
* 函数名 : void Mcu_UartxSendByte(UART_HandleTypeDef huart,char dat)
* 描述   : 发送串口数据
* 输入   : 无
* 输出   : 无
* 返回   : 无
* 注意   :
********************************************************************/
void Mcu_UartxSendByte(UART_HandleTypeDef huart,char dat)
{
    /* 发送一个字节数据到huart.Instance */
    HAL_UART_Transmit(&huart, (uint8_t *)&dat, 1, 0xFFFF);	
}
/*******************************************************************
* 函数名 : Mcu_UartxSendString(UART_HandleTypeDef huart,char * str)
* 描述   : 发送字符串
* 输入   : 无
* 输出   : 无
* 返回   : 无
* 注意   :
********************************************************************/
void Mcu_UartxSendString(UART_HandleTypeDef huart,char * str)
{
    while(*str)//检测字符串结束符
	{
		Mcu_UartxSendByte(huart,*str++);      //发送当前字符
	}
}

/*******************************************************************
* 函数名 : Mcu_UartxSendLR(UART_HandleTypeDef huart)
* 描述   : 发送回车符0x0D 和 换行符0x0A
* 输入   : 无
* 输出   : 无
* 返回   : 无
* 注意   :
********************************************************************/
void Mcu_UartxSendLR(UART_HandleTypeDef huart)
{
    Mcu_UartxSendByte(huart,0x0D);  //回车符
    Mcu_UartxSendByte(huart,0x0A);  //换行符
	
}
/*******************************************************************************
* 函数名 :  Mcu_UartxSendAtCommand(UART_HandleTypeDef huart,char * cmd)
* 描述   : 发送AT指令函数
* 输入   :
* 输出   :
* 返回   :
* 注意   :
*******************************************************************************/
void Mcu_UartxSendAtCommand(UART_HandleTypeDef huart,char * cmd)
{
    Mcu_UartxSendString(huart,cmd);
	Mcu_UartxSendLR(huart);
}

/*******************************************************************************
* 函数名 : Mcu_UartxSendDatabyLength(UART_HandleTypeDef huart,char *data, unsigned short len)
* 描述   : 按照指定的数据长度发送数据
* 输入   :
* 输出   :
* 返回   :
* 注意   :
*******************************************************************************/
void Mcu_UartxSendDatabyLength(UART_HandleTypeDef huart,char *data, unsigned short len)
{
    unsigned int k = 0;
    do 
    {
        Mcu_UartxSendByte(huart,*(data + k));
        k++;
    } while(k < len);
}

