#ifndef __UART_H
#define __UART_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */


/* USER CODE BEGIN Private defines */
void Mcu_UartxSendByte(UART_HandleTypeDef huart,char dat);
void Mcu_UartxSendString(UART_HandleTypeDef huart,char * str);
void Mcu_UartxSendLR(UART_HandleTypeDef huart);
void Mcu_UartxSendAtCommand(UART_HandleTypeDef huart,char * cmd);
void Mcu_UartxSendDatabyLength(UART_HandleTypeDef huart,char *data, unsigned short len);
/* USER CODE END Private defines */




#ifdef __cplusplus
}
#endif
#endif /*__ AT_UART_H */