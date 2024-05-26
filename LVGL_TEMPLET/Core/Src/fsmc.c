/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : FSMC.c
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

/* Includes ------------------------------------------------------------------*/
#include "fsmc.h"

/* USER CODE BEGIN 0 */
static FSMC_NORSRAM_TimingTypeDef   hfsmc_rw;
/* USER CODE END 0 */

SRAM_HandleTypeDef hsram4;

/* FSMC initialization function */
void MX_FSMC_Init(void)
{
  /* USER CODE BEGIN FSMC_Init 0 */
    hsram4.Instance = FSMC_NORSRAM_DEVICE;
    hsram4.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;

    hsram4.Init.NSBank               = FSMC_NORSRAM_BANK4;     				// 使用NE4
    hsram4.Init.DataAddressMux       = FSMC_DATA_ADDRESS_MUX_DISABLE; 	    // 地址/数据线不复用
    hsram4.Init.MemoryType           = FSMC_MEMORY_TYPE_SRAM;   				// SRAM
    hsram4.Init.MemoryDataWidth      = FSMC_NORSRAM_MEM_BUS_WIDTH_16; 	    // 16位数据宽度
    hsram4.Init.BurstAccessMode      = FSMC_BURST_ACCESS_MODE_DISABLE;       // 是否使能突发访问,仅对同步突发存储器有效,此处未用到
    hsram4.Init.WaitSignalPolarity   = FSMC_WAIT_SIGNAL_POLARITY_LOW;        // 等待信号的极性,仅在突发模式访问下有用
    hsram4.Init.WaitSignalActive     = FSMC_WAIT_TIMING_BEFORE_WS;   	    // 存储器是在等待周期之前的一个时钟周期还是等待周期期间使能NWAIT
    hsram4.Init.WriteOperation       = FSMC_WRITE_OPERATION_ENABLE;    	    // 存储器写使能
    hsram4.Init.WaitSignal           = FSMC_WAIT_SIGNAL_DISABLE;           	// 等待使能位,此处未用到
    hsram4.Init.ExtendedMode         = FSMC_EXTENDED_MODE_DISABLE;        	// 读写使用相同的时序
    hsram4.Init.AsynchronousWait     = FSMC_ASYNCHRONOUS_WAIT_DISABLE;	    // 是否使能同步传输模式下的等待信号,此处未用到
    hsram4.Init.WriteBurst           = FSMC_WRITE_BURST_DISABLE;           	// 禁止突发写
    
    hfsmc_rw.AddressSetupTime      = 0x00;
    hfsmc_rw.AddressHoldTime       = 0x00;
    hfsmc_rw.DataSetupTime         = 0x08;
    hfsmc_rw.BusTurnAroundDuration = 0x00;
    hfsmc_rw.CLKDivision           = 0x00;
    hfsmc_rw.DataLatency           = 0x00;
    hfsmc_rw.AccessMode            = FSMC_ACCESS_MODE_A;
    
    
    if(HAL_SRAM_Init(&hsram4, &hfsmc_rw, &hfsmc_rw) != HAL_OK)
    {
        Error_Handler();
    }    
#if 0	
  /* USER CODE END FSMC_Init 0 */

  FSMC_NORSRAM_TimingTypeDef Timing = {0};
  FSMC_NORSRAM_TimingTypeDef ExtTiming = {0};

  /* USER CODE BEGIN FSMC_Init 1 */

  /* USER CODE END FSMC_Init 1 */

  /** Perform the SRAM4 memory initialization sequence
  */
  hsram4.Instance = FSMC_NORSRAM_DEVICE;
  hsram4.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram4.Init */
  hsram4.Init.NSBank = FSMC_NORSRAM_BANK4;
  hsram4.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
  hsram4.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
  hsram4.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram4.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
  hsram4.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram4.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
  hsram4.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
  hsram4.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
  hsram4.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
  hsram4.Init.ExtendedMode = FSMC_EXTENDED_MODE_ENABLE;
  hsram4.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram4.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
  /* Timing */
  Timing.AddressSetupTime = 0;
  Timing.AddressHoldTime = 15;
  Timing.DataSetupTime = 2;
  Timing.BusTurnAroundDuration = 0;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FSMC_ACCESS_MODE_A;
  /* ExtTiming */
  ExtTiming.AddressSetupTime = 0;
  ExtTiming.AddressHoldTime = 15;
  ExtTiming.DataSetupTime = 2;
  ExtTiming.BusTurnAroundDuration = 0;
  ExtTiming.CLKDivision = 16;
  ExtTiming.DataLatency = 17;
  ExtTiming.AccessMode = FSMC_ACCESS_MODE_A;

  if (HAL_SRAM_Init(&hsram4, &Timing, &ExtTiming) != HAL_OK)
  {
    Error_Handler( );
  }

  /** Disconnect NADV
  */

  __HAL_AFIO_FSMCNADV_DISCONNECTED();

  /* USER CODE BEGIN FSMC_Init 2 */
#endif
  /* USER CODE END FSMC_Init 2 */
}

static uint32_t FSMC_Initialized = 0;

static void HAL_FSMC_MspInit(void){
  /* USER CODE BEGIN FSMC_MspInit 0 */
	GPIO_InitTypeDef    GPIO_InitStruct = {0};
    
    __HAL_RCC_FSMC_CLK_ENABLE();
    GPIOC_CLK_EN();
    GPIOD_CLK_EN();
    GPIOE_CLK_EN();
    GPIOF_CLK_EN();
    GPIOG_CLK_EN();
    
    
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    
    GPIO_InitStruct.Pin = LCD_RST_PIN;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
    

    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
	
	// PGx
    GPIO_InitStruct.Pin = LCD_CS_PIN;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
	
    // PDx
    GPIO_InitStruct.Pin = LCD_GPIOD_PIN;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    // PEx
    GPIO_InitStruct.Pin = LCD_GPIOE_PIN;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	
	
	
#if 0
  /* USER CODE END FSMC_MspInit 0 */
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if (FSMC_Initialized) {
    return;
  }
  FSMC_Initialized = 1;

  /* Peripheral clock enable */
  __HAL_RCC_FSMC_CLK_ENABLE();

  /** FSMC GPIO Configuration
  PE2   ------> FSMC_A23
  PF0   ------> FSMC_A0
  PE7   ------> FSMC_D4
  PE8   ------> FSMC_D5
  PE9   ------> FSMC_D6
  PE10   ------> FSMC_D7
  PE11   ------> FSMC_D8
  PE12   ------> FSMC_D9
  PE13   ------> FSMC_D10
  PE14   ------> FSMC_D11
  PE15   ------> FSMC_D12
  PD8   ------> FSMC_D13
  PD9   ------> FSMC_D14
  PD10   ------> FSMC_D15
  PD14   ------> FSMC_D0
  PD15   ------> FSMC_D1
  PD0   ------> FSMC_D2
  PD1   ------> FSMC_D3
  PD4   ------> FSMC_NOE
  PD5   ------> FSMC_NWE
  PG12   ------> FSMC_NE4
  */
  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9
                          |GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14
                          |GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4
                          |GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /* USER CODE BEGIN FSMC_MspInit 1 */
	//CUBEMX配置FSMC必须要配置一个addr口，这里不需要，也不好更改，更改了一旦保存就又填上了，这里deinit一下
  HAL_GPIO_DeInit(GPIOF, GPIO_PIN_0);
	__HAL_RCC_GPIOF_CLK_DISABLE();
#endif
  /* USER CODE END FSMC_MspInit 1 */
}

void HAL_SRAM_MspInit(SRAM_HandleTypeDef* sramHandle){
  /* USER CODE BEGIN SRAM_MspInit 0 */

  /* USER CODE END SRAM_MspInit 0 */
  HAL_FSMC_MspInit();
  /* USER CODE BEGIN SRAM_MspInit 1 */

  /* USER CODE END SRAM_MspInit 1 */
}

static uint32_t FSMC_DeInitialized = 0;

static void HAL_FSMC_MspDeInit(void){
  /* USER CODE BEGIN FSMC_MspDeInit 0 */

  /* USER CODE END FSMC_MspDeInit 0 */
  if (FSMC_DeInitialized) {
    return;
  }
  FSMC_DeInitialized = 1;
  /* Peripheral clock enable */
  __HAL_RCC_FSMC_CLK_DISABLE();

  /** FSMC GPIO Configuration
  PE2   ------> FSMC_A23
  PF0   ------> FSMC_A0
  PE7   ------> FSMC_D4
  PE8   ------> FSMC_D5
  PE9   ------> FSMC_D6
  PE10   ------> FSMC_D7
  PE11   ------> FSMC_D8
  PE12   ------> FSMC_D9
  PE13   ------> FSMC_D10
  PE14   ------> FSMC_D11
  PE15   ------> FSMC_D12
  PD8   ------> FSMC_D13
  PD9   ------> FSMC_D14
  PD10   ------> FSMC_D15
  PD14   ------> FSMC_D0
  PD15   ------> FSMC_D1
  PD0   ------> FSMC_D2
  PD1   ------> FSMC_D3
  PD4   ------> FSMC_NOE
  PD5   ------> FSMC_NWE
  PG12   ------> FSMC_NE4
  */

  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_2|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9
                          |GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_14|GPIO_PIN_15);

  HAL_GPIO_DeInit(GPIOF, GPIO_PIN_0);

  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14
                          |GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4
                          |GPIO_PIN_5);

  HAL_GPIO_DeInit(GPIOG, GPIO_PIN_12);

  /* USER CODE BEGIN FSMC_MspDeInit 1 */

  /* USER CODE END FSMC_MspDeInit 1 */
}

void HAL_SRAM_MspDeInit(SRAM_HandleTypeDef* sramHandle){
  /* USER CODE BEGIN SRAM_MspDeInit 0 */

  /* USER CODE END SRAM_MspDeInit 0 */
  HAL_FSMC_MspDeInit();
  /* USER CODE BEGIN SRAM_MspDeInit 1 */

  /* USER CODE END SRAM_MspDeInit 1 */
}
/**
  * @}
  */

/**
  * @}
  */
