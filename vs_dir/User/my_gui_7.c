#include "my_gui.h"
#include "lvgl.h"
#include <stdio.h>

#define scr_act_height() lv_obj_get_height(lv_scr_act())
#define scr_act_width() lv_obj_get_width(lv_scr_act())

//为了兼容8，9版本函数
#if (LVGL_VERSION_MAJOR == 8)
#define lv_spinner_create(x) lv_spinner_create(x,1000,50)//参数为obj，动画时间，指示器长度
#endif


const lv_font_t* g_font;

#if !USE_FREERTOS
static void timer_cb(lv_timer_t* timer)
{

}
#endif

static void test_cb(lv_event_t* event)
{
	
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

    //创建加载器spinner
    lv_obj_t* spinner = lv_spinner_create(lv_scr_act());
    lv_obj_align(spinner, LV_ALIGN_CENTER, 0, -scr_act_height()/20);
//	设置加载器的指示器的颜色，不能用bg_clolr,得用下面这个
    lv_obj_set_style_arc_color(spinner, lv_color_make(247, 94, 136), LV_PART_INDICATOR);
		
    //创建label
    lv_obj_t* label = lv_label_create(lv_scr_act());
    lv_obj_align(label, LV_ALIGN_CENTER, 0, scr_act_height() / 8);
    lv_label_set_text_fmt(label, "LOADING...");
    return;
}
