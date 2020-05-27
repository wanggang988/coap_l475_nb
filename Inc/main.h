/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2020 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h >


/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define MAIN_VERSION (1) 
#define SUB_VERSION (0)
#define DEBUG (0)
#define UART_LEN (1024)
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
typedef enum 
{
    AT_SENDOK = 0,
	AT_SENDFAIL,
	AT_SENDTIMEOUT,
}at_status;

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
uint16_t CRC16_modbus(uint8_t *_pBuf, uint16_t _usLen);
uint32_t adcvalue_get(int channel);
uint32_t * sensor_read(const char* sensorname);

#define LOG_INFO() printf("error file:%s;error line %d;\n",__FILE__,__LINE__);
#define SHOWN_VERSION()  printf("BC28L475_V0.1\n Compile time:%s\n Compile date:%s\n Versin:%d.%d.%d\t\n",  \
                               __TIME__,__DATE__,MAIN_VERSION,SUB_VERSION,DEBUG);

//#define INTERNAL12V_CHANNEL (1)
//#define EXTERNAL5V_CHANNEL (2)
//#define INTERNALTEMP_CHANNEL (17)
//#define ADC_CHANNEL1_PARA (3.3/4096*10.6f)
//#define ADC_CHANNEL2_PARA ((3.3/4096*50/30.0f)*(4000/5.0f))   //4MÀ­Éþ
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ACCEL_PEN_Pin GPIO_PIN_1
#define ACCEL_PEN_GPIO_Port GPIOA
#define NBIOT_PEN_Pin GPIO_PIN_5
#define NBIOT_PEN_GPIO_Port GPIOC
#define NBIOT_RST_Pin GPIO_PIN_12
#define NBIOT_RST_GPIO_Port GPIOB
#define ACCEL_INT1_Pin GPIO_PIN_10
#define ACCEL_INT1_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */
struct rx_fifo
{
	char *name;
	uint16_t total_len;
	uint8_t *buffer;
	uint8_t *resp;
	uint16_t put_index,get_index;
	bool  is_full;
};
typedef struct rx_fifo _rx_fifo;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
