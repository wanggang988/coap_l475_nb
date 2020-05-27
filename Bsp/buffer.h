#ifndef __buffer_H
#define __buffer_H
#include "stm32l4xx_hal.h"

////uart串口buffer大小
#define  LPUART1_BUFFER_SIZE	512 	                         //LPUART1串口buffer空间大小，定义处理NBIOT模块AT指令接收缓存区和发送缓存区的长度
#define  UART2_BUFFER_SIZE	512 	                            //UART2串口buffer空间大小，定义处理参数设置等接收缓存区和发送缓存区的长度

////lpuart1 接收缓存区
//char  recvlp1_buf[LPUART1_BUFFER_SIZE];
//char * recvlp1_ptr;
//short  recvlp1_num;

////lpuart1 发送缓存区
//char   sendlp1_buf[LPUART1_BUFFER_SIZE];
//char * sendlp1_ptr;
//short  sendlp1_num;

////uart2 接收缓存区
//char   recv2_buf[UART2_BUFFER_SIZE];
//char * recv2_ptr;
//short  recv2_num;

////uart2 发送缓存区
//char   send2_buf[UART2_BUFFER_SIZE];
//char * send2_ptr;
//short  send2_num;

void Buffer_Lpuart1RecvClear(void);
void Buffer_Lpuart1SendClear(void);
void Buffer_Uart2RecvClear(void);
void Buffer_Uart2SendClear(void);

#endif /*__ buffer_H */