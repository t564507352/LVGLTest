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

/********************GPIOD************************/
/* PD0 PD1 PD4 PD5 PD8 PD9 PD10 PD14 PD15 */ 
/*********************GPIOG***********************/
#define LCD_GPIOD_PIN           ( GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_9 \
                            | GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15)
                            
/********************GPIOE************************/
/* PE2 PE7 PE8 PE9 PE10 PE11 PE12 PE13 PE14 PE15 */
/*********************GPIOG***********************/
#define LCD_GPIOE_PIN           ( GPIO_PIN_2 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 \
                            | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15)
                            

/********************FSMC_D***********************/
#define LCD_D0_PIN                  GPIO_PIN_14 // PD14
#define LCD_D1_PIN                  GPIO_PIN_15 // PD15
#define LCD_D2_PIN                  GPIO_PIN_0  // PD0
#define LCD_D3_PIN                  GPIO_PIN_1  // PD1
#define LCD_D4_PIN                  GPIO_PIN_7  // PE7  
#define LCD_D5_PIN                  GPIO_PIN_8  // PE8 
#define LCD_D6_PIN                  GPIO_PIN_9  // PE9 
#define LCD_D7_PIN                  GPIO_PIN_10 // PE10 
#define LCD_D8_PIN                  GPIO_PIN_11 // PE11 
#define LCD_D9_PIN                  GPIO_PIN_12 // PE12 
#define LCD_D10_PIN                 GPIO_PIN_13 // PE13 
#define LCD_D11_PIN                 GPIO_PIN_14 // PE14 
#define LCD_D12_PIN                 GPIO_PIN_15 // PE15 
#define LCD_D13_PIN                 GPIO_PIN_8  // PD8
#define LCD_D14_PIN                 GPIO_PIN_9  // PD9
#define LCD_D15_PIN                 GPIO_PIN_10 // PD10
    
/********************其他信号线****  *******************/
#define LCD_CS_PIN                  GPIO_PIN_12  // PG12 NE4
#define LCD_RS_PIN                  GPIO_PIN_2   // PE2  A23     
#define LCD_WR_PIN                  GPIO_PIN_5   // PD5  NWE              
#define LCD_RD_PIN                  GPIO_PIN_4   // PD4  OE              

#define LCD_RST_PIN                 GPIO_PIN_11

#define GPIOC_CLK_EN()              __HAL_RCC_GPIOC_CLK_ENABLE()
#define GPIOD_CLK_EN()              __HAL_RCC_GPIOD_CLK_ENABLE()
#define GPIOE_CLK_EN()              __HAL_RCC_GPIOE_CLK_ENABLE()
#define GPIOF_CLK_EN()              __HAL_RCC_GPIOF_CLK_ENABLE()
#define GPIOG_CLK_EN()              __HAL_RCC_GPIOG_CLK_ENABLE()

#define LCD_BL(VALUE)              	ILI9488_BackLightSwitch(VALUE)//背景光                    
#define LCD_CS(VALUE)               HAL_GPIO_WritePin(GPIOG, LCD_CS_PIN, VALUE)
#define LCD_RS(VALUE)               HAL_GPIO_WritePin(GPIOE, LCD_RS_PIN, VALUE)
#define LCD_WR(VALUE)               HAL_GPIO_WritePin(GPIOD, LCD_WR_PIN, VALUE)
#define LCD_RD(VALUE)               HAL_GPIO_WritePin(GPIOD, LCD_RD_PIN, VALUE)
#define LCD_RST(VALUE)              HAL_GPIO_WritePin(GPIOF, LCD_RST_PIN, VALUE)


/* USER CODE END Includes */

extern SRAM_HandleTypeDef hsram4;

/* USER CODE BEGIN Private defines */
/* USER CODE END Private defines */

void MX_FSMC_Init(void);
void HAL_SRAM_MspInit(SRAM_HandleTypeDef* hsram);
void HAL_SRAM_MspDeInit(SRAM_HandleTypeDef* hsram);

/* USER CODE BEGIN Prototypes */

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
