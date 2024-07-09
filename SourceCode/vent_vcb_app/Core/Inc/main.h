/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
extern CAN_HandleTypeDef hcan1;

extern SPI_HandleTypeDef hspi1;
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;
extern I2C_HandleTypeDef hi2c3;
extern TIM_HandleTypeDef htim4;
extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi4;
extern SPI_HandleTypeDef hspi5;
extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart6;
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
#define O2_SPI4_SCK_Pin GPIO_PIN_2
#define O2_SPI4_SCK_GPIO_Port GPIOE
#define Valve_NO_Pin GPIO_PIN_3
#define Valve_NO_GPIO_Port GPIOE
#define O2_SPI4_CS_Pin GPIO_PIN_4
#define O2_SPI4_CS_GPIO_Port GPIOE
#define Valve_MIX_Pin GPIO_PIN_5
#define Valve_MIX_GPIO_Port GPIOE
#define O2_SPI4_MOSI_Pin GPIO_PIN_6
#define O2_SPI4_MOSI_GPIO_Port GPIOE
#define Valve_Safe_Pin GPIO_PIN_8
#define Valve_Safe_GPIO_Port GPIOI
#define Value_Nebu_Pin GPIO_PIN_13
#define Value_Nebu_GPIO_Port GPIOC
#define FB_Valve_SOL_INP_Pin GPIO_PIN_9
#define FB_Valve_SOL_INP_GPIO_Port GPIOI
#define FB_Valve_NO_Pin GPIO_PIN_10
#define FB_Valve_NO_GPIO_Port GPIOI
#define FB_Valve_MIX_Pin GPIO_PIN_11
#define FB_Valve_MIX_GPIO_Port GPIOI
#define Temp_Ctrl_Pin GPIO_PIN_1
#define Temp_Ctrl_GPIO_Port GPIOF
#define FB_Valve_Safe_Pin GPIO_PIN_2
#define FB_Valve_Safe_GPIO_Port GPIOF
#define FB_Valve_Nebu_Pin GPIO_PIN_3
#define FB_Valve_Nebu_GPIO_Port GPIOF
#define AIR_SPI5_CS_Pin GPIO_PIN_6
#define AIR_SPI5_CS_GPIO_Port GPIOF
#define AIR_SPI5_SCK_Pin GPIO_PIN_7
#define AIR_SPI5_SCK_GPIO_Port GPIOF
#define AIR_SPI5_MOSI_Pin GPIO_PIN_9
#define AIR_SPI5_MOSI_GPIO_Port GPIOF
#define WDI_Pin GPIO_PIN_1
#define WDI_GPIO_Port GPIOC
#define AD_12V_VAL_Pin GPIO_PIN_1
#define AD_12V_VAL_GPIO_Port GPIOA
#define AD_7V_VAL_Pin GPIO_PIN_2
#define AD_7V_VAL_GPIO_Port GPIOA
#define MBEE_WP_Pin GPIO_PIN_3
#define MBEE_WP_GPIO_Port GPIOH
#define AD_3V3_VAL_Pin GPIO_PIN_3
#define AD_3V3_VAL_GPIO_Port GPIOA
#define AD_5V_VAL_Pin GPIO_PIN_5
#define AD_5V_VAL_GPIO_Port GPIOA
#define ADC_3V3_VAL_Pin GPIO_PIN_6
#define ADC_3V3_VAL_GPIO_Port GPIOA
#define ADC_5V_VAL_Pin GPIO_PIN_7
#define ADC_5V_VAL_GPIO_Port GPIOA
#define WORK_LED1_Pin GPIO_PIN_0
#define WORK_LED1_GPIO_Port GPIOG
#define WORK_LED2_Pin GPIO_PIN_1
#define WORK_LED2_GPIO_Port GPIOG
#define O2_IIC_SCL_Pin GPIO_PIN_10
#define O2_IIC_SCL_GPIO_Port GPIOB
#define O2_IIC_SDA_Pin GPIO_PIN_11
#define O2_IIC_SDA_GPIO_Port GPIOB
#define AIR_IIC_SCL_Pin GPIO_PIN_7
#define AIR_IIC_SCL_GPIO_Port GPIOH
#define AIR_IIC_SDA_Pin GPIO_PIN_8
#define AIR_IIC_SDA_GPIO_Port GPIOH
#define VCM_SPI2_CS_Pin GPIO_PIN_12
#define VCM_SPI2_CS_GPIO_Port GPIOB
#define VCM_SPI2_SCK_Pin GPIO_PIN_13
#define VCM_SPI2_SCK_GPIO_Port GPIOB
#define VCM_SPI2_MOSI_Pin GPIO_PIN_15
#define VCM_SPI2_MOSI_GPIO_Port GPIOB
#define Nebu_PWR_EN_Pin GPIO_PIN_13
#define Nebu_PWR_EN_GPIO_Port GPIOD
#define O2_FLOW_EN_Pin GPIO_PIN_4
#define O2_FLOW_EN_GPIO_Port GPIOG
#define AIR_FLOW_EN_Pin GPIO_PIN_5
#define AIR_FLOW_EN_GPIO_Port GPIOG
#define Valve2_Pin GPIO_PIN_14
#define Valve2_GPIO_Port GPIOH
#define PB_Valve2_Pin GPIO_PIN_15
#define PB_Valve2_GPIO_Port GPIOH
#define ADC_SPI_CS_Pin GPIO_PIN_15
#define ADC_SPI_CS_GPIO_Port GPIOA
#define MBEE_WPI6_Pin GPIO_PIN_6
#define MBEE_WPI6_GPIO_Port GPIOI
#define Valve_SOL_INP_Pin GPIO_PIN_7
#define Valve_SOL_INP_GPIO_Port GPIOI

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
