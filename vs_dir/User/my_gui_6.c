#include "my_gui.h"
#include "lvgl.h"
#include <stdio.h>

#if USE_FREERTOS
#include "TimerTask.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "semphr.h"
static _bar_timer barTimer;
osThreadId_t barTimerTaskHandle;
const osThreadAttr_t barTimerTask_attributes = {
					.name = "barTimerTask",
					.stack_size = 512,
					.priority = (osPriority_t) osPriorityNormal,
				};
#endif

#define scr_act_height() lv_obj_get_height(lv_scr_act())
#define scr_act_width() lv_obj_get_width(lv_scr_act())



const lv_font_t* g_font;
lv_obj_t* g_barLabel;
lv_obj_t* g_bar;
uint8_t g_barValue = 0;
				
#if !USE_FREERTOS
static void timer_cb(lv_timer_t* timer)
{
    if (g_barValue < 100)
    {
        g_barValue++;
        lv_bar_set_value(g_bar, g_barValue, LV_ANIM_ON);
        lv_label_set_text_fmt(g_barLabel, "%d %%...", g_barValue);
    }
    else
    {
        lv_label_set_text_fmt(g_barLabel, "FINISH!!");
    }
}
#endif

static void test_cb(lv_event_t* event)
{
   
}


void  show_label(void)
{
    // 设置顶部标题
    lv_obj_t* titleLabel = lv_label_create(lv_scr_act());
    lv_obj_align(titleLabel, LV_ALIGN_CENTER, 0, -scr_act_height() / 3);
    lv_obj_set_style_text_font(titleLabel, g_font, LV_PART_MAIN);
    lv_label_set_long_mode(titleLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(titleLabel, "LOADING...");

    //设置底部计价板
    g_barLabel = lv_label_create(lv_scr_act());
    lv_obj_align(g_barLabel, LV_ALIGN_CENTER, 0, scr_act_height() / 3);
    lv_obj_set_style_text_font(g_barLabel, g_font, LV_PART_MAIN);
    lv_label_set_long_mode(g_barLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text_fmt(g_barLabel, "LOADING START...");
}
void  show_bar(void)
{
    g_bar = lv_bar_create(lv_scr_act());
    lv_obj_align(g_bar, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(g_bar, scr_act_width()*2/3, scr_act_height()/25);
    //设置动画时间要在设置当前值前，不然看不到效果--v8.2
    lv_obj_set_style_anim_time(g_bar, 100, LV_STATE_DEFAULT);
    //8.2和9版本可能默认值不一样，可以设置
    lv_bar_set_range(g_bar, 0, 100);
    //设置当前值，动画
    lv_bar_set_value(g_bar, 0, LV_ANIM_ON);

#if USE_FREERTOS
		barTimer.bar = g_bar;
		barTimer.barLabel = g_barLabel;
    barTimerTaskHandle = osThreadNew(barTimerTask, &barTimer, &barTimerTask_attributes);
#else
	    lv_timer_create(timer_cb, 100, NULL);
#endif
}
void GUI_test(void)
{
    //获取活动屏幕宽高，选择字体

    int32_t scr_act_width = scr_act_width();
    int32_t scr_act_height = scr_act_height();
    if (scr_act_width <= 320)
    {
        g_font = &lv_font_montserrat_14;
    }
    else if (scr_act_width <= 480)
    {
        g_font = &lv_font_montserrat_18;
    }
    else
    {
        g_font = &lv_font_montserrat_22;
    }
    show_label();
    show_bar();
    return;
}
