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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
typedef enum
{
    ADDR_DebugUart  = (0x00),                    // 0x00
    ADDR_RS232      = (ADDR_DebugUart  + 4096),  // 0x01
    ADDR_RS485_CAN  = (ADDR_RS232      + 4096),  // 0x02
    ADDR_SRAM_RW    = (ADDR_RS485_CAN  + 4096),  // 0x03
    ADDR_EEPROM_RW  = (ADDR_SRAM_RW    + 4096),  // 0x04
    ADDR_AP3426     = (ADDR_EEPROM_RW  + 4096),  // 0x05
    ADDR_W25Q64_RW  = (ADDR_AP3426     + 4096),  // 0x06
    ADDR_ICM_20608  = (ADDR_W25Q64_RW  + 4096),  // 0x07
    ADDR_NET_CARD   = (ADDR_ICM_20608  + 4096),  // 0x08
    ADDR_USB_CDC    = (ADDR_NET_CARD   + 4096),  // 0x09
    ADDR_LED_T      = (ADDR_USB_CDC    + 4096),  // 0x0A
    ADDR_BEEP_T     = (ADDR_LED_T      + 4096),  // 0x0B
    ADDR_KEY_T      = (ADDR_BEEP_T     + 4096),  // 0x0C
    ADDR_IRD_T      = (ADDR_KEY_T      + 4096),  // 0x0D
    ADDR_VOICE_CARD = (ADDR_IRD_T      + 4096),  // 0x0E
    ADDR_ADC_T      = (ADDR_VOICE_CARD + 4096),  // 0x0F
    ADDR_FACT_UI    = (ADDR_ADC_T      + 4096),  // 0x10
    ADDR_CONFIG     = (ADDR_FACT_UI    + 4096),  // 0x11
    ILI9341_PARA_FLASH_ADDR = (ADDR_CONFIG    + 4096),  // 0x12
    ILI9488_PARA_FLASH_ADDR = (ILI9341_PARA_FLASH_ADDR    + 4096),  // 0x13
}TEST_ADDR;   

//typedef enum
//{
//	ILI9488_PARA_FLASH_ADDR  = 0x00,
//}	TEST_ADDR;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
