/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED2_Pin GPIO_PIN_13
#define LED2_GPIO_Port GPIOC
#define LED1_Pin GPIO_PIN_14
#define LED1_GPIO_Port GPIOC
#define DO5_Pin GPIO_PIN_15
#define DO5_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define DO1_Pin GPIO_PIN_2
#define DO1_GPIO_Port GPIOB
#define DO2_Pin GPIO_PIN_10
#define DO2_GPIO_Port GPIOB
#define DO3_Pin GPIO_PIN_12
#define DO3_GPIO_Port GPIOB
#define DO4_Pin GPIO_PIN_13
#define DO4_GPIO_Port GPIOB
#define GSM_PWR_Pin GPIO_PIN_14
#define GSM_PWR_GPIO_Port GPIOB
#define GSM_RST_Pin GPIO_PIN_15
#define GSM_RST_GPIO_Port GPIOB
#define TX_EN_Pin GPIO_PIN_8
#define TX_EN_GPIO_Port GPIOA
#define DI1_Pin GPIO_PIN_11
#define DI1_GPIO_Port GPIOA
#define DI2_Pin GPIO_PIN_12
#define DI2_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define DI5_Pin GPIO_PIN_15
#define DI5_GPIO_Port GPIOA
#define DI3_Pin GPIO_PIN_3
#define DI3_GPIO_Port GPIOB
#define DI3B4_Pin GPIO_PIN_4
#define DI3B4_GPIO_Port GPIOB
#define EE_WP_Pin GPIO_PIN_5
#define EE_WP_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
