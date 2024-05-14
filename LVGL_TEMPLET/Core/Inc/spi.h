/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.h
  * @brief   This file contains all the function prototypes for
  *          the spi.c file
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
#ifndef __SPI_H__
#define __SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN Private defines */
#define  W25_RW_TIMEOUT 50



#define W25_CS_PIN                      GPIO_PIN_4               
#define W25_CS_GPIO_PORT                GPIOA   
#define W25_CS(level)                    HAL_GPIO_WritePin(W25_CS_GPIO_PORT, W25_CS_PIN, level?GPIO_PIN_SET:GPIO_PIN_RESET)

/* USER CODE END Private defines */

void MX_SPI1_Init(void);

/* USER CODE BEGIN Prototypes */
void SPI_WriteByte(SPI_HandleTypeDef* hspi , uint8_t data);

/*
 *  函数名：uint8_t SPI_ReadByte(void)
 *  输入参数：
 *  输出参数：无
 *  返回值：读到的数据
 *  函数作用：模拟SPI读一个字节
*/
uint8_t SPI_ReadByte(SPI_HandleTypeDef* hspi);

/*
 *  函数名：uint8_t SPI_WriteReadByte(uint8_t pdata)
 *  输入参数：pdata -> 要写的一个字节数据
 *  输出参数：无
 *  返回值：读到的数据
 *  函数作用：模拟SPI读写一个字节
*/
uint8_t SPI_WriteReadByte(SPI_HandleTypeDef* hspi ,uint8_t pdata);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __SPI_H__ */

