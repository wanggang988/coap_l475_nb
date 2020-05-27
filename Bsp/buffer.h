#ifndef __buffer_H
#define __buffer_H
#include "stm32l4xx_hal.h"

////uart����buffer��С
#define  LPUART1_BUFFER_SIZE	512 	                         //LPUART1����buffer�ռ��С�����崦��NBIOTģ��ATָ����ջ������ͷ��ͻ������ĳ���
#define  UART2_BUFFER_SIZE	512 	                            //UART2����buffer�ռ��С�����崦��������õȽ��ջ������ͷ��ͻ������ĳ���

////lpuart1 ���ջ�����
//char  recvlp1_buf[LPUART1_BUFFER_SIZE];
//char * recvlp1_ptr;
//short  recvlp1_num;

////lpuart1 ���ͻ�����
//char   sendlp1_buf[LPUART1_BUFFER_SIZE];
//char * sendlp1_ptr;
//short  sendlp1_num;

////uart2 ���ջ�����
//char   recv2_buf[UART2_BUFFER_SIZE];
//char * recv2_ptr;
//short  recv2_num;

////uart2 ���ͻ�����
//char   send2_buf[UART2_BUFFER_SIZE];
//char * send2_ptr;
//short  send2_num;

void Buffer_Lpuart1RecvClear(void);
void Buffer_Lpuart1SendClear(void);
void Buffer_Uart2RecvClear(void);
void Buffer_Uart2SendClear(void);

#endif /*__ buffer_H */