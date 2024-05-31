/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.c
  * @brief   This file provides code for the configuration
  *          of the SPI instances.
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
/* Includes ------------------------------------------------------------------*/
#include "spi.h"

/* USER CODE BEGIN 0 */
#include "driver_timer.h"
#define SPI_Delay()     us_timer_delay(10)


#if 0
/* USER CODE END 0 */

SPI_HandleTypeDef hspi1;

/* SPI1 init function */
void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */
	W25_CS(1); //外部flash不片选
  /* USER CODE END SPI1_Init 2 */

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspInit 0 */

  /* USER CODE END SPI1_MspInit 0 */
    /* SPI1 clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI1_MspInit 1 */
		__HAL_RCC_GPIOA_CLK_ENABLE();
		GPIO_InitStruct.Pin = W25_CS_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		HAL_GPIO_Init(W25_CS_GPIO_PORT, &GPIO_InitStruct);
  /* USER CODE END SPI1_MspInit 1 */
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspDeInit 0 */

  /* USER CODE END SPI1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI1_CLK_DISABLE();

    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);

  /* USER CODE BEGIN SPI1_MspDeInit 1 */

  /* USER CODE END SPI1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */



#endif

void MX_SPI1_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    SPIx_SCK_GPIO_CLK_ENABLE();
    SPIx_MISO_GPIO_CLK_ENABLE();
    SPIx_MOSI_GPIO_CLK_ENABLE();
    W25_CS_GPIO_CLK_ENABLE();
    
    GPIO_InitStruct.Pin       = SPIx_SCK_PIN | W25_CS_PIN | SPIx_MOSI_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);
    
    
    GPIO_InitStruct.Pin       = SPIx_MISO_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStruct); 
    
    
    W25_CS(1);      // CS初始化高
    SPI_CLK(0);     // CLK初始化低
}

/*
 *  函数名：void SPI_WriteByte(uint8_t data)
 *  输入参数：data -> 要写的数据
 *  输出参数：无
 *  返回值：无
 *  函数作用：模拟SPI写一个字节
*/
void SPI_WriteByte(uint8_t data)
{
    uint8_t i = 0;
    uint8_t temp = 0;
    
    for(i=0; i<8; i++)
    {
        temp = data&0x80;
        data = data<<1;
        
        SPI_CLK(0);
        SPI_MOSI(temp);
        SPI_Delay();
        SPI_CLK(1);
        SPI_Delay();
    }
}

/*
 *  函数名：uint8_t SPI_ReadByte(void)
 *  输入参数：
 *  输出参数：无
 *  返回值：读到的数据
 *  函数作用：模拟SPI读一个字节
*/
uint8_t SPI_ReadByte(void)
{
    uint8_t i = 0;
    uint8_t read_data = 0xFF;
    
    for(i=0; i<8; i++)
    {
        read_data = read_data << 1;
        
        SPI_CLK(0);
        SPI_Delay();
        SPI_CLK(1);
        SPI_Delay();
        if(SPI_MISO()==1)
        {
            read_data = read_data + 1;
        }
    }
    SPI_CLK(0);
    return read_data;
}

/*
 *  函数名：uint8_t SPI_WriteReadByte(uint8_t pdata)
 *  输入参数：pdata -> 要写的一个字节数据
 *  输出参数：无
 *  返回值：读到的数据
 *  函数作用：模拟SPI读写一个字节
*/
uint8_t SPI_WriteReadByte(uint8_t pdata)
{
    uint8_t i = 0;
    uint8_t temp = 0;
    uint8_t read_data = 0xFF;

    for(i=0;i<8;i++)
    {
        temp = ((pdata&0x80)==0x80)? 1:0;
        pdata = pdata<<1;
        read_data = read_data<<1;
        
        SPI_CLK(0);
        SPI_MOSI(temp);
        SPI_Delay();
        SPI_CLK(1);
        SPI_Delay();
        if(SPI_MISO()==1)
        {
            read_data = read_data + 1;
        }
    }
     
    return read_data;
}

/* USER CODE END 1 */
