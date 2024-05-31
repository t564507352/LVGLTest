#include "LVGLTask.h"
#include "stdio.h"
#include "my_gui.h"
//#include "./stress/lv_demo_stress.h"
extern lv_indev_t * indev_keypad;
volatile static SemaphoreHandle_t  mutex_lvgl = NULL;
SemaphoreHandle_t get_lvgl_mutex(void);
void indev_keypad_group_init(void);



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
		indev_keypad_group_init();
		
	

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


SemaphoreHandle_t get_lvgl_mutex(void)
{
	return mutex_lvgl;
}


void indev_keypad_group_init(void)
{
	//注册组
	lv_group_t* g = lv_group_create();
	//把这个组设置为默认组的配置（默认配置已经关联的一部分widges了）
	
	lv_group_set_default(g);
	//如果设置默认关联配置没用，用这个手动加一下obj到此组
	//lv_group_add_obj(group,boj);
	
	//关联键盘到组
	lv_indev_set_group(indev_keypad, g);
}
