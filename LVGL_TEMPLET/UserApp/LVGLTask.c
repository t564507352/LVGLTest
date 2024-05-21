#include "LVGLTask.h"
#include "stdio.h"
#include "my_gui.h"
//#include "./stress/lv_demo_stress.h"

volatile static SemaphoreHandle_t  mutex_lvgl = NULL;

SemaphoreHandle_t get_lvgl_mutex(void)
{
	return mutex_lvgl;
}

void LVGLTask(void* params)
{
		mutex_lvgl = xSemaphoreCreateMutex();
		if(mutex_lvgl == NULL)
		{
			printf("mutex_lvgl_creatr_err!");
			Error_Handler();
		}
		lv_init();
		lv_port_disp_init();
		lv_port_indev_init();


		vTaskDelay(1000);
		//	lv_demo_stress();
		xSemaphoreTake(mutex_lvgl, portMAX_DELAY);
		GUI_test();
		xSemaphoreGive(mutex_lvgl);
		while(1)
		{
			xSemaphoreTake(mutex_lvgl, portMAX_DELAY);
			lv_timer_handler();
			xSemaphoreGive(mutex_lvgl);
			vTaskDelay(5);
		}
}


