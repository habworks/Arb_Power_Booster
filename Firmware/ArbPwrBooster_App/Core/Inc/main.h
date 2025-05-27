/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "stm32f7xx_hal.h"

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
#define EN_VO_2_Pin GPIO_PIN_4
#define EN_VO_2_GPIO_Port GPIOB
#define R_SHDN_1_Pin GPIO_PIN_15
#define R_SHDN_1_GPIO_Port GPIOA
#define EN_50_Z_2_Pin GPIO_PIN_3
#define EN_50_Z_2_GPIO_Port GPIOI
#define EN_VO_1_Pin GPIO_PIN_2
#define EN_VO_1_GPIO_Port GPIOI
#define USD_DETECT_Pin GPIO_PIN_13
#define USD_DETECT_GPIO_Port GPIOC
#define LCD_BL_CTRL_Pin GPIO_PIN_3
#define LCD_BL_CTRL_GPIO_Port GPIOK
#define EN_FAN_1_Pin GPIO_PIN_1
#define EN_FAN_1_GPIO_Port GPIOI
#define LCD_DISP_Pin GPIO_PIN_12
#define LCD_DISP_GPIO_Port GPIOI
#define R_SHDN_2_Pin GPIO_PIN_0
#define R_SHDN_2_GPIO_Port GPIOI
#define EN_CH_1_Pin GPIO_PIN_8
#define EN_CH_1_GPIO_Port GPIOA
#define EN_FAN_2_Pin GPIO_PIN_7
#define EN_FAN_2_GPIO_Port GPIOG
#define MAIN_PWR_EN_Pin GPIO_PIN_6
#define MAIN_PWR_EN_GPIO_Port GPIOG
#define IO_MON_2_Pin GPIO_PIN_10
#define IO_MON_2_GPIO_Port GPIOF
#define POS_20V_MON_Pin GPIO_PIN_9
#define POS_20V_MON_GPIO_Port GPIOF
#define NEG_20V_MON_Pin GPIO_PIN_8
#define NEG_20V_MON_GPIO_Port GPIOF
#define EN_CH_2_Pin GPIO_PIN_6
#define EN_CH_2_GPIO_Port GPIOH
#define ADC3_CRATE_Pin GPIO_PIN_14
#define ADC3_CRATE_GPIO_Port GPIOB
#define EN_50_Z_1_Pin GPIO_PIN_15
#define EN_50_Z_1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
