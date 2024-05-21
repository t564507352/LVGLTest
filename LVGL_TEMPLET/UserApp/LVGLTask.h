#ifndef __LVGLAPP_H_
#define __LVGLAPP_H_
#include "main.h"
#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "semphr.h"







extern void LVGLTask(void* params);
extern SemaphoreHandle_t get_lvgl_mutex(void);
#endif
