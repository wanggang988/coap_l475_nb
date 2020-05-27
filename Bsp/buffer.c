#include "buffer.h"

//lpuart1 ���ջ�����
char  recvlp1_buf[LPUART1_BUFFER_SIZE];
char * recvlp1_ptr;
short  recvlp1_num;

//lpuart1 ���ͻ�����
char   sendlp1_buf[LPUART1_BUFFER_SIZE];
char * sendlp1_ptr;
short  sendlp1_num;

//uart2 ���ջ�����
char   recv2_buf[UART2_BUFFER_SIZE];
char * recv2_ptr;
short  recv2_num;

//uart2 ���ͻ�����
char   send2_buf[UART2_BUFFER_SIZE];
char * send2_ptr;
short  send2_num;


/*******************************************************************
* ������ : Buffer_Lpuart1RecvClear
* ����   : ����LPUART1���ջ�����
* ����   : ��
* ���   : ��
* ����   : ��
* ע��   : ��������СΪLPUART1_BUFFER_SIZE�ֽڣ��ͻ����޸�����
********************************************************************/
void Buffer_Lpuart1RecvClear(void)
{
	unsigned short i = 0;

	for(i = 0; i < LPUART1_BUFFER_SIZE; i++)
	{
		recvlp1_buf[i] = 0x00;
	}

	recvlp1_ptr = recvlp1_buf;
	recvlp1_num = 0;
}

/*******************************************************************
* ������ : Buffer_Lpuart1SendClear
* ����   : ����LPUART1���ͻ�����
* ����   : ��
* ���   : ��
* ����   : ��
* ע��   : ��������СΪLPUART1_BUFFER_SIZE�ֽڣ��ͻ����޸�����
********************************************************************/
void Buffer_Lpuart1SendClear(void)
{
	unsigned short i = 0;

	for(i = 0; i < LPUART1_BUFFER_SIZE; i++)
	{
		sendlp1_buf[i] = 0x00;
	}

	sendlp1_ptr = sendlp1_buf;
	sendlp1_num = 0;
}

/*******************************************************************
* ������ : Buffer_Uart2RecvClear
* ����   : ����UART2���ջ�����
* ����   : ��
* ���   : ��
* ����   : ��
* ע��   : ��������СΪUART2_BUFFER_SIZE�ֽڣ��ͻ����޸�����
********************************************************************/
void Buffer_Uart2RecvClear(void)
{
    unsigned short i = 0;

    for(i = 0; i < UART2_BUFFER_SIZE; i++)
    {
        recv2_buf[i] = 0x00;
    }

    recv2_ptr = recv2_buf;
    recv2_num = 0;
}

/*******************************************************************
* ������ : Buffer_Uart2SendClear
* ����   : ����UART2���ͻ�����
* ����   : ��
* ���   : ��
* ����   : ��
* ע��   : ��������СΪUART2_BUFFER_SIZE�ֽڣ��ͻ����޸�����
********************************************************************/
void Buffer_Uart2SendClear(void)
{
    unsigned short i = 0;

    for(i = 0; i < UART2_BUFFER_SIZE; i++)
    {
        send2_buf[i] = 0x00;
    }

    send2_ptr = send2_buf;
    send2_num = 0;
}
