/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "lcd_function.h"
//#include "XPT2046.h"
#include "LVGLTask.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
//osThreadId_t TouchTaskTaskHandle;//²âÊÔ´¥ÃþÆÁµÄÈÎÎñ
osThreadId_t LVGLTaskHandle;
const osThreadAttr_t LVGLTask_attributes = {
  .name = "LVGLTask",
  .stack_size = (512 * 4 - 256),
  .priority = (osPriority_t) osPriorityNormal,
};

QueueHandle_t LED_Frequency;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
//void TouchTask(void *argument);//²âÊÔ´¥ÃþÆÁµÄÈÎÎñ
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
//	TouchTaskTaskHandle = osThreadNew(TouchTask, NULL, &defaultTask_attributes);//²âÊÔ´¥ÃþÆÁµÄÈÎÎñ

	LVGLTaskHandle = osThreadNew(LVGLTask, NULL, &LVGLTask_attributes);
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	static uint8_t Frequency ;
	static uint16_t actualFrequency = 1010;
	LED_Frequency = xQueueCreate( 40, 1 );
  /* Infinite loop */
  while(1)
  {
		if(xQueueReceive(LED_Frequency, &Frequency, 1))
		{
			actualFrequency = (101 - Frequency) * 10;
		}
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
		vTaskDelay(actualFrequency);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
//void TouchTask(void *argument)
//{
//	LCD_SetAllColor(WHITE,BLACK);
//	uint8_t line = 0;
//	uint16_t fontSize = 16;
//	lv_indev_data_t testData;
//	char showMsgBuff[50];
//	LCD_Clear(WHITE);
//	while(1)
//	{
//		xpt2046_read(&testData);
//		if(testData.state == LV_INDEV_STATE_PR)
//		{
//			sprintf(showMsgBuff,"Screen touch occurs! x = %d ; y = %d\r\n",testData.point.x,testData.point.y);
//			printf("%s",showMsgBuff);
//			LCD_ShowString_Line(line,fontSize,(uint8_t*)showMsgBuff);
//			line++;
//			if(line > (lcddev.ver_res/fontSize))
//			{
//				line = 0;
//			}
//		}
//	}
//}

		
/* USER CODE END Application */

