#include "my_gui.h"
#include "lvgl.h"
#include <stdio.h>

#if USE_FREERTOS
#include "lcd_function.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdlib.h>
#endif	

#define scr_act_height() lv_obj_get_height(lv_scr_act())
#define scr_act_width() lv_obj_get_width(lv_scr_act())

//为了兼容8，9版本函数
#if (LVGL_VERSION_MAJOR == 8)

#endif


const lv_font_t* g_font;
lv_obj_t*  g_label;
lv_obj_t* g_slider1;//背景亮度
lv_obj_t* g_slider2;//LED频率

#if !USE_FREERTOS
static void timer_cb(lv_timer_t* timer)
{

}
#endif

static void test_cb(lv_event_t* event)
{
    static uint8_t brightness;
    static uint8_t ledFQ;
    lv_obj_t* obj = lv_event_get_target(event);
    if (obj == g_slider1)//背景亮度
    {
       brightness = lv_slider_get_value(obj);
#if USE_FREERTOS	
		//控制LCD屏幕高度
	   LCD_Brightness(brightness);
#endif	
    }
    else if (obj == g_slider2)//LED频率
    {
       ledFQ = lv_slider_get_value(obj);
#if USE_FREERTOS
		//发送给默认任务中的点灯中的队列，控制亮灭频率
	   static uint8_t pre_ledFQ = 0;
		//避免发送太频繁
	   if(abs(ledFQ - pre_ledFQ) > 10)
	   {
		   extern QueueHandle_t LED_Frequency;
		   xQueueSend(LED_Frequency,&ledFQ,100);		   
	   }
	   pre_ledFQ = ledFQ;
#endif		
    }
    printf("brightness : %d%% \r\nLED frequency : %d%%", brightness, ledFQ);
    lv_label_set_text_fmt(g_label, "brightness : %d%% \r\nLED frequency : %d%%", brightness, ledFQ);

}

//背景亮度滑块
void show_slider(void)
{
    //设置slider滑块
    g_slider1 =  lv_slider_create(lv_scr_act());
    lv_obj_align(g_slider1, LV_ALIGN_CENTER, -scr_act_width() / 40, 0);
    //设置取值范围
    lv_slider_set_range(g_slider1, 0, 100);
    //设置默认值
    lv_slider_set_value(g_slider1, 90, LV_ANIM_ON);
    //设置滑块模式
    lv_slider_set_mode(g_slider1, LV_BAR_MODE_NORMAL);
    lv_obj_set_style_bg_color(g_slider1, lv_color_make(245, 93, 138), LV_PART_KNOB);
    lv_obj_set_style_bg_color(g_slider1, lv_color_make(235, 188, 224), LV_PART_INDICATOR);

    //添加亮度图标
    lv_obj_t* label = lv_label_create(lv_scr_act());
    lv_obj_align_to(label, g_slider1, LV_ALIGN_OUT_RIGHT_MID, scr_act_width()/30, 0);
    lv_obj_set_style_text_font(label, g_font, LV_STATE_DEFAULT);
    lv_label_set_text(label, LV_SYMBOL_CHARGE);

    //给slider添加事件
    lv_obj_add_event_cb(g_slider1, test_cb, LV_EVENT_VALUE_CHANGED, NULL);
}

//LED频率滑块
void show_slider2(void)
{
    //设置slider滑块
    g_slider2 = lv_slider_create(lv_scr_act());
    lv_obj_align(g_slider2, LV_ALIGN_CENTER, -scr_act_width() / 40, -scr_act_height() / 10);
    //设置取值范围
    lv_slider_set_range(g_slider2, 0, 100);
    //设置默认值
    lv_slider_set_value(g_slider2, 0, LV_ANIM_ON);
    //设置滑块模式
    lv_slider_set_mode(g_slider2, LV_BAR_MODE_NORMAL);
    lv_obj_set_style_bg_color(g_slider2, lv_color_make(245, 93, 138), LV_PART_KNOB);
    lv_obj_set_style_bg_color(g_slider2, lv_color_make(235, 188, 224), LV_PART_INDICATOR);

    //添加LED频率图标
    lv_obj_t* label = lv_label_create(lv_scr_act());
    lv_obj_align_to(label, g_slider2, LV_ALIGN_OUT_RIGHT_MID, scr_act_width() / 30, 0);
    lv_obj_set_style_text_font(label, g_font, LV_STATE_DEFAULT);
    lv_label_set_text(label, LV_SYMBOL_REFRESH);
    //给slider添加事件
    lv_obj_add_event_cb(g_slider2, test_cb, LV_EVENT_VALUE_CHANGED, NULL);
}

void show_valueLabel(void)
{
    //添加数值label
    g_label = lv_label_create(lv_scr_act());
    lv_obj_align(g_label, LV_ALIGN_CENTER, 0, -scr_act_height() / 5);
    lv_label_set_text(g_label, " ");
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

    //测试代码
    //lv_obj_t* obj =  lv_slider_create(lv_scr_act());
    //lv_obj_align(obj, LV_ALIGN_CENTER, 0, -scr_act_height() / 10);
    //lv_slider_set_range(obj, -100, 100);
    //lv_slider_set_value(obj, 0, LV_ANIM_ON);
    ////设置双向的，可以设置左值
    //lv_slider_set_mode(obj, LV_BAR_MODE_RANGE);
    //lv_slider_set_left_value(obj, 0, LV_ANIM_ON);
    //lv_obj_set_style_bg_color(obj, lv_color_make(245, 93, 138), LV_PART_KNOB);
    //lv_obj_add_event_cb(obj, test_cb, LV_EVENT_VALUE_CHANGED, NULL);

    

    show_slider();
    show_slider2();
    show_valueLabel();
    return;
}
