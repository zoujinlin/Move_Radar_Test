/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cmsis_os.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "usart.h"
#include "tim.h"
#include "user_key.h"
#include "user_fun.h"
#include "user_radar.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void microdelay(uint32_t us);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY_B_Pin GPIO_PIN_13
#define KEY_B_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define M_DIR_Pin GPIO_PIN_8
#define M_DIR_GPIO_Port GPIOC
#define M_EN_Pin GPIO_PIN_9
#define M_EN_GPIO_Port GPIOC
#define V_CTR_Pin GPIO_PIN_11
#define V_CTR_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

#define AUTO 0
#define MANU 1

//motor run direction define
#define M_STOP  0 //motor stop
#define M_FAR   1 //motor far away
#define M_NEAR  2 //motor close to

#define MAX_MV_TIME 1000 //ms
#define BUFF_LEN    768
#define UR2R_LEN 32
typedef struct
{
	uint8_t  key;
	uint8_t  M_mode;//the mode of motor,1=manual / 0=auto 
	uint8_t  M_dir;//the direction of motor move on, 0:stop,1:far away, 2:close to
	uint8_t  M_pos_end;//to point out stop at whitch end 
	uint32_t Move_cnt;//move count for one time
	uint8_t  move_com;//move compensation
	
	uint16_t rcv_timeout;//time out of data receive of radar
	uint8_t  rcv_flag;//data receive flag, 1:received ong pack, 0:no receive 
	uint8_t  urbuff[BUFF_LEN];//the buff of data receive from radar
	uint8_t  rcv2_flag;//uart2 receive flag
	uint8_t  ur2buff[UR2R_LEN];//uart2 reveive buff
	uint8_t  spcbuff[256];//the buff of data send to pc
	uint8_t  send_cfg;//send ods cfg flag 
}SYSRAMTYPE;
extern SYSRAMTYPE SysRam;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
