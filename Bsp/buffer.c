#include "buffer.h"

//lpuart1 接收缓存区
char  recvlp1_buf[LPUART1_BUFFER_SIZE];
char * recvlp1_ptr;
short  recvlp1_num;

//lpuart1 发送缓存区
char   sendlp1_buf[LPUART1_BUFFER_SIZE];
char * sendlp1_ptr;
short  sendlp1_num;

//uart2 接收缓存区
char   recv2_buf[UART2_BUFFER_SIZE];
char * recv2_ptr;
short  recv2_num;

//uart2 发送缓存区
char   send2_buf[UART2_BUFFER_SIZE];
char * send2_ptr;
short  send2_num;


/*******************************************************************
* 函数名 : Buffer_Lpuart1RecvClear
* 描述   : 清零LPUART1接收缓存区
* 输入   : 无
* 输出   : 无
* 返回   : 无
* 注意   : 缓存区大小为LPUART1_BUFFER_SIZE字节，客户可修改扩大
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
* 函数名 : Buffer_Lpuart1SendClear
* 描述   : 清零LPUART1发送缓存区
* 输入   : 无
* 输出   : 无
* 返回   : 无
* 注意   : 缓存区大小为LPUART1_BUFFER_SIZE字节，客户可修改扩大
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
* 函数名 : Buffer_Uart2RecvClear
* 描述   : 清零UART2接收缓存区
* 输入   : 无
* 输出   : 无
* 返回   : 无
* 注意   : 缓存区大小为UART2_BUFFER_SIZE字节，客户可修改扩大
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
* 函数名 : Buffer_Uart2SendClear
* 描述   : 清零UART2发送缓存区
* 输入   : 无
* 输出   : 无
* 返回   : 无
* 注意   : 缓存区大小为UART2_BUFFER_SIZE字节，客户可修改扩大
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
