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
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_RED_Pin GPIO_PIN_2
#define LED_RED_GPIO_Port GPIOE
#define LED_GREEN_Pin GPIO_PIN_3
#define LED_GREEN_GPIO_Port GPIOE
#define LED_BLUE_Pin GPIO_PIN_4
#define LED_BLUE_GPIO_Port GPIOE
#define BAT_VOL_SENSE_Pin GPIO_PIN_0
#define BAT_VOL_SENSE_GPIO_Port GPIOC
#define CURR_SENS_OUT_Pin GPIO_PIN_1
#define CURR_SENS_OUT_GPIO_Port GPIOC
#define PYRO_1_CONT_Pin GPIO_PIN_2
#define PYRO_1_CONT_GPIO_Port GPIOC
#define PYRO_2_CONT_Pin GPIO_PIN_3
#define PYRO_2_CONT_GPIO_Port GPIOC
#define SERVO_2_Pin GPIO_PIN_2
#define SERVO_2_GPIO_Port GPIOA
#define SERVO_3_Pin GPIO_PIN_3
#define SERVO_3_GPIO_Port GPIOA
#define NRF_IRQ_Pin GPIO_PIN_4
#define NRF_IRQ_GPIO_Port GPIOA
#define NRF_CS_Pin GPIO_PIN_4
#define NRF_CS_GPIO_Port GPIOC
#define NRF_CE_Pin GPIO_PIN_5
#define NRF_CE_GPIO_Port GPIOC
#define SERVO_5_Pin GPIO_PIN_0
#define SERVO_5_GPIO_Port GPIOB
#define SERVO_6_Pin GPIO_PIN_1
#define SERVO_6_GPIO_Port GPIOB
#define SERVO_1_Pin GPIO_PIN_13
#define SERVO_1_GPIO_Port GPIOE
#define SERVO_4_Pin GPIO_PIN_14
#define SERVO_4_GPIO_Port GPIOE
#define IO4_Pin GPIO_PIN_11
#define IO4_GPIO_Port GPIOD
#define IO5_Pin GPIO_PIN_13
#define IO5_GPIO_Port GPIOD
#define IO6_Pin GPIO_PIN_14
#define IO6_GPIO_Port GPIOD
#define IO7_Pin GPIO_PIN_15
#define IO7_GPIO_Port GPIOD
#define GPS_TIMEPULSE_Pin GPIO_PIN_11
#define GPS_TIMEPULSE_GPIO_Port GPIOA
#define GPS_RST_Pin GPIO_PIN_12
#define GPS_RST_GPIO_Port GPIOA
#define CURR_SENS_FAULT_Pin GPIO_PIN_12
#define CURR_SENS_FAULT_GPIO_Port GPIOC
#define IO2_Pin GPIO_PIN_2
#define IO2_GPIO_Port GPIOD
#define IO3_Pin GPIO_PIN_3
#define IO3_GPIO_Port GPIOD
#define PYRO_2_FIRE_Pin GPIO_PIN_4
#define PYRO_2_FIRE_GPIO_Port GPIOD
#define PYRO_1_FIRE_Pin GPIO_PIN_5
#define PYRO_1_FIRE_GPIO_Port GPIOD
#define IMU_INT_Pin GPIO_PIN_5
#define IMU_INT_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
