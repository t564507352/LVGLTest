#include "lvglApp.h"
#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "stdio.h"
#include "./stress/lv_demo_stress.h"
void LVGLTask()
{
	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();
//	
//	lv_obj_t* switch_obj = lv_switch_create(lv_scr_act());
//	lv_obj_set_size(switch_obj, 120, 60);
//	lv_obj_align(switch_obj, LV_ALIGN_CENTER, 0, 0);
//	
	lv_demo_stress();
	while(1)
	{
		lv_timer_handler();
		HAL_Delay(5);
	}
}

