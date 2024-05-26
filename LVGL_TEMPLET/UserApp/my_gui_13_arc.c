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
lv_obj_t* g_leftArc;
lv_obj_t* g_leftLabel;
lv_obj_t* g_rightArc;
lv_obj_t* g_rightLabel;


#if !USE_FREERTOS
static void timer_cb(lv_timer_t* timer)
{

}
#endif

static void test_cb(lv_event_t* event)
{
    lv_obj_t* target = lv_event_get_target(event);
    int32_t text = 
    printf("%d\r\n", lv_arc_get_value(target));
    lv_label_set_text_fmt(g_leftLabel, "%d%%", lv_arc_get_value(target));
    lv_label_set_text_fmt(g_rightLabel, "%d%%", lv_arc_get_value(target));
    lv_arc_set_value(g_rightArc, lv_arc_get_value(target));
}

void show_arc_left()
{
    g_leftArc = lv_arc_create(lv_scr_act());
    lv_obj_align(g_leftArc, LV_ALIGN_CENTER, -scr_act_width()/4, 0);
    lv_arc_set_range(g_leftArc, 1, 100);
    lv_arc_set_value(g_leftArc, 1);
    lv_obj_add_event_cb(g_leftArc, test_cb, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_update_layout(g_leftArc);
    g_leftLabel = lv_label_create(g_leftArc);
    lv_obj_align(g_leftLabel, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_long_mode(g_leftLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(g_leftLabel, " ");
}

void show_arc_right()
{
    g_rightArc = lv_arc_create(lv_scr_act());
    lv_obj_align(g_rightArc, LV_ALIGN_CENTER, scr_act_width() / 4, 0);

    //设置背景角度
    lv_arc_set_bg_angles(g_rightArc, 91, 89);
    // 设置前景角度，最好先设置背景再设置前景
    lv_arc_set_angles(g_rightArc, 91, 89);
    lv_arc_set_range(g_rightArc, 1, 100);
    lv_arc_set_value(g_rightArc, 1);
    //添加不可操作的状态，仅用于显示
    lv_obj_add_state(g_rightArc, LV_STATE_DISABLED);

    lv_obj_update_layout(g_rightArc);
    g_rightLabel = lv_label_create(g_rightArc);
    lv_obj_align(g_rightLabel, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_long_mode(g_rightLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(g_rightLabel, " ");
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
    //lv_obj_t* arc = lv_arc_create(lv_scr_act());
    //lv_obj_align(arc, LV_ALIGN_CENTER, 0, 0);
    //lv_arc_set_range(arc, 0, 100);
    //lv_arc_set_value(arc, 0);

    ////设置旋转速率，最后一个参数为每秒能旋转多少度
    //lv_arc_set_change_rate(arc, 180);
    ////设置背景角度
    //lv_arc_set_bg_angles(arc, 91, 89);
    //// 设置前景角度，最好先设置背景再设置前景
    //lv_arc_set_angles(arc, 91, 89);
    //// 设置圆弧的整体旋转角度
    //lv_arc_set_rotation(arc,180);
    ////设置手柄模式，可以设置反向与对称
    //lv_arc_set_mode(arc, LV_ARC_MODE_SYMMETRICAL);


    //说明性标签
    lv_obj_t* label = lv_label_create(lv_scr_act());
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -scr_act_height() / 4);
    lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(label, "scr sync test");
    show_arc_left();
    show_arc_right();
    return;
}
