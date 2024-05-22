#ifndef __BAR_TIMER_TASK_
#define __BAR_TIMER_TASK_

#include "main.h"
#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "semphr.h"



typedef struct
{
	lv_obj_t* barLabel;
	lv_obj_t* bar;
} _bar_timer;



extern void barTimerTask(void* params);
#endif
