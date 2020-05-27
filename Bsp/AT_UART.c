/* Includes ------------------------------------------------------------------*/
#include "AT_UART.h"
/*******************************************************************
* ������ : void Mcu_UartxSendByte(UART_HandleTypeDef huart,char dat)
* ����   : ���ʹ�������
* ����   : ��
* ���   : ��
* ����   : ��
* ע��   :
********************************************************************/
void Mcu_UartxSendByte(UART_HandleTypeDef huart,char dat)
{
    /* ����һ���ֽ����ݵ�huart.Instance */
    HAL_UART_Transmit(&huart, (uint8_t *)&dat, 1, 0xFFFF);	
}
/*******************************************************************
* ������ : Mcu_UartxSendString(UART_HandleTypeDef huart,char * str)
* ����   : �����ַ���
* ����   : ��
* ���   : ��
* ����   : ��
* ע��   :
********************************************************************/
void Mcu_UartxSendString(UART_HandleTypeDef huart,char * str)
{
    while(*str)//����ַ���������
	{
		Mcu_UartxSendByte(huart,*str++);      //���͵�ǰ�ַ�
	}
}

/*******************************************************************
* ������ : Mcu_UartxSendLR(UART_HandleTypeDef huart)
* ����   : ���ͻس���0x0D �� ���з�0x0A
* ����   : ��
* ���   : ��
* ����   : ��
* ע��   :
********************************************************************/
void Mcu_UartxSendLR(UART_HandleTypeDef huart)
{
    Mcu_UartxSendByte(huart,0x0D);  //�س���
    Mcu_UartxSendByte(huart,0x0A);  //���з�
	
}
/*******************************************************************************
* ������ :  Mcu_UartxSendAtCommand(UART_HandleTypeDef huart,char * cmd)
* ����   : ����ATָ���
* ����   :
* ���   :
* ����   :
* ע��   :
*******************************************************************************/
void Mcu_UartxSendAtCommand(UART_HandleTypeDef huart,char * cmd)
{
    Mcu_UartxSendString(huart,cmd);
	Mcu_UartxSendLR(huart);
}

/*******************************************************************************
* ������ : Mcu_UartxSendDatabyLength(UART_HandleTypeDef huart,char *data, unsigned short len)
* ����   : ����ָ�������ݳ��ȷ�������
* ����   :
* ���   :
* ����   :
* ע��   :
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

