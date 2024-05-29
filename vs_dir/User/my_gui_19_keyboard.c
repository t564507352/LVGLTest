#include "my_gui.h"
#include "lvgl.h"
#include <stdio.h>
#include "math.h"
#include <stdlib.h>
#include <string.h>
#if USE_FREERTOS
#include "lcd_function.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#endif	

#define scr_act_height() lv_obj_get_height(lv_scr_act())
#define scr_act_width() lv_obj_get_width(lv_scr_act())

//为了兼容8，9版本函数
#if (LVGL_VERSION_MAJOR == 8)

#endif

const lv_font_t* g_font;
static lv_obj_t* g_ta;

#if !USE_FREERTOS
static void timer_cb(lv_timer_t* timer)
{

}
#endif

static void test_cb(lv_event_t* event)
{
    uint32_t btnnb = 0;
    lv_obj_t* target = lv_event_get_target(event);
    //lv_event_code_t code = lv_event_get_code(event);
    btnnb = lv_btnmatrix_get_selected_btn(target);
    //strcmp对比两个字符串，如果相等返回0，不相等返回1
    //判断是否按下了键盘图标
    if (!strcmp(lv_btnmatrix_get_btn_text(target, btnnb), LV_SYMBOL_KEYBOARD))
    {
        //如果按下了键盘图标，获取当前键盘模式，切换为另外的模式
        if (lv_keyboard_get_mode(target) == LV_KEYBOARD_MODE_NUMBER)
        {
            lv_keyboard_set_mode(target, LV_KEYBOARD_MODE_TEXT_LOWER);
        }
        else
        {
            lv_keyboard_set_mode(target, LV_KEYBOARD_MODE_NUMBER);
        }   
    }
}


void show_textarea(void)
{
    g_ta = lv_textarea_create(lv_scr_act());
    lv_obj_set_size(g_ta, scr_act_width(), scr_act_height() * 2 / 5);
    lv_obj_align(g_ta, LV_ALIGN_TOP_MID, 0, 0);

}

void show_keyboard(void)
{
    lv_obj_t* kb = lv_keyboard_create(lv_scr_act());
    lv_keyboard_set_textarea(kb, g_ta);
    lv_obj_add_event_cb(kb, test_cb, LV_EVENT_VALUE_CHANGED, NULL);
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
    lv_obj_t* obj = lv_textarea_create(lv_scr_act());

    lv_obj_t* obj2 = lv_keyboard_create(lv_scr_act());
    //设置弹窗模式
    lv_keyboard_set_popovers(obj2, true);
    //设置键盘模式，可设置默认是什么键盘
    lv_keyboard_set_mode(obj2, LV_KEYBOARD_MODE_NUMBER);
    lv_keyboard_set_textarea(obj2, obj);


    show_textarea();
    show_keyboard();


    return;
}
