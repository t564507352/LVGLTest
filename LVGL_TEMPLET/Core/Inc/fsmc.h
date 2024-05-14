/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : FSMC.h
  * Description        : This file provides code for the configuration
  *                      of the FSMC peripheral.
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
#ifndef __FSMC_H
#define __FSMC_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern SRAM_HandleTypeDef hsram4;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_FSMC_Init(void);
void HAL_SRAM_MspInit(SRAM_HandleTypeDef* hsram);
void HAL_SRAM_MspDeInit(SRAM_HandleTypeDef* hsram);

/* USER CODE BEGIN Prototypes */
#define LCD_RS_PIN                  GPIO_PIN_2   // PE2  A23                          
#define LCD_BL_PIN                  GPIO_PIN_6   // PC6  LCD_PWM 
#define LCD_RST_PIN                 GPIO_PIN_11
#define LCD_BL(VALUE)              	ILI9488_BackLightSwitch(VALUE)//������
#define LCD_RS(VALUE)               HAL_GPIO_WritePin(GPIOE, LCD_RS_PIN, VALUE?GPIO_PIN_SET:GPIO_PIN_RESET)//��д�л�
#define LCD_RST(VALUE)              HAL_GPIO_WritePin(GPIOF, LCD_RST_PIN, VALUE?GPIO_PIN_SET:GPIO_PIN_RESET)
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__FSMC_H */

/**
  * @}
  */

/**
  * @}
  */
