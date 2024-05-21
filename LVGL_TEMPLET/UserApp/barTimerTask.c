#include "barTimerTask.h"
#include "LVGLTask.h"
#include <stdio.h>
#define GET_MUTEX_TIME 2000

volatile static SemaphoreHandle_t  mutex_lvgl = NULL;
void barTimerTask(void* params)
{
		mutex_lvgl = get_lvgl_mutex();
		if(mutex_lvgl == NULL)
			{
				printf("get_mutex_lvgl_err!");
				Error_Handler();
			}
		_bar_timer* barParams = (_bar_timer*)params;
		int8_t barValue = 0;

		while(1)
		{
			if (barValue < 100)
			{
					barValue++;
					if(xSemaphoreTake(mutex_lvgl, GET_MUTEX_TIME))
					{
							lv_bar_set_value(barParams->bar, barValue, LV_ANIM_ON);
							lv_label_set_text_fmt(barParams->barLabel, "%d %%...", barValue);
							xSemaphoreGive(mutex_lvgl);
					}	
					else
					{
							printf("get_mutex_lvgl_timeout!");
							Error_Handler();
					}
			}
			else
			{
					if(xSemaphoreTake(mutex_lvgl, GET_MUTEX_TIME))
					{
							lv_label_set_text_fmt(barParams->barLabel, "FINISH!!");
							xSemaphoreGive(mutex_lvgl);
							vTaskDelete(NULL);
					}
					else
					{
							printf("get_mutex_lvgl_timeout!");
							Error_Handler();
					}
			}
			vTaskDelay(95);
		}
}
