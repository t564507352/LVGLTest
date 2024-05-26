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
#if 0
/* USER CODE END Includes */

extern SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_SPI1_Init(void);

/* USER CODE BEGIN Prototypes */
#endif
/************************* SPI Ӳ����ض��� *************************/
#define SPIx                             SPI1
#define SPIx_CLK_ENABLE()                __HAL_RCC_SPI1_CLK_ENABLE()
#define SPIx_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE() 
#define SPIx_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE() 
#define W25_CS_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE() 

#define SPIx_FORCE_RESET()               __HAL_RCC_SPI1_FORCE_RESET()
#define SPIx_RELEASE_RESET()             __HAL_RCC_SPI1_RELEASE_RESET()

#define SPIx_SCK_PIN                     GPIO_PIN_5
#define SPIx_SCK_GPIO_PORT               GPIOA

#define SPIx_MISO_PIN                    GPIO_PIN_6
#define SPIx_MISO_GPIO_PORT              GPIOA

#define SPIx_MOSI_PIN                    GPIO_PIN_7
#define SPIx_MOSI_GPIO_PORT              GPIOA

#define W25_CS_PIN                      GPIO_PIN_4               
#define W25_CS_GPIO_PORT                GPIOA   


#define SPI_CLK(level)                   HAL_GPIO_WritePin(SPIx_SCK_GPIO_PORT, SPIx_SCK_PIN, level&GPIO_PIN_SET)                                    
#define SPI_MISO()                       HAL_GPIO_ReadPin(SPIx_MISO_GPIO_PORT, SPIx_MISO_PIN)
#define SPI_MOSI(bit)                    HAL_GPIO_WritePin(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_PIN, bit&GPIO_PIN_SET)
#define W25_CS(level)                    HAL_GPIO_WritePin(W25_CS_GPIO_PORT, W25_CS_PIN, level&GPIO_PIN_SET)
/************************* SPI Ӳ����ض������ *************************/
/*
 *  ��������void SPI_Init(void)
 *  ���������
 *  �����������
 *  ����ֵ����
 *  �������ã���ʼ��SPI���ĸ�����
*/
extern void MX_SPI1_Init(void);

/*
 *  ��������uint8_t SPI_WriteReadByte(uint8_t pdata)
 *  ���������pdata -> Ҫд��һ���ֽ�����
 *  �����������
 *  ����ֵ������������
 *  �������ã�ģ��SPI��дһ���ֽ�
*/
extern uint8_t SPI_WriteReadByte(uint8_t pdata);

/*
 *  ��������void SPI_WriteByte(uint8_t data)
 *  ���������data -> Ҫд������
 *  �����������
 *  ����ֵ����
 *  �������ã�ģ��SPIдһ���ֽ�
*/
extern void SPI_WriteByte(uint8_t data);

/*
 *  ��������uint8_t SPI_ReadByte(void)
 *  ���������
 *  �����������
 *  ����ֵ������������
 *  �������ã�ģ��SPI��һ���ֽ�
*/
extern uint8_t SPI_ReadByte(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __SPI_H__ */

