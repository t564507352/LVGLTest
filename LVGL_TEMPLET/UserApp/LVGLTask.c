#include "LVGLTask.h"
#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "stdio.h"
#include "my_gui.h"

//#include "./stress/lv_demo_stress.h"



void LVGLTask(void* params)
{
		lv_init();
		lv_port_disp_init();
		lv_port_indev_init();


	
		//	lv_demo_stress();

		GUI_test();
		while(1)
		{
			lv_timer_handler();
			vTaskDelay(5);
		}
}

