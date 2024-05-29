#include "my_gui.h"
#include "lvgl.h"
#include <stdio.h>

#define scr_act_height() lv_obj_get_height(lv_scr_act())
#define scr_act_width() lv_obj_get_width(lv_scr_act())

//为了兼容8，9版本函数
#if (LVGL_VERSION_MAJOR == 8)

#endif


const lv_font_t* g_font;
lv_obj_t* g_label_temp;
lv_obj_t* g_label_speed;
lv_obj_t* tempRoller;
lv_obj_t* speedRoller;

#if !USE_FREERTOS
static void timer_cb(lv_timer_t* timer)
{

}
#endif

static void test_cb(lv_event_t* event)
{
    lv_obj_t* obj = lv_event_get_target(event);
    if (obj == tempRoller)
    {
        //获取选择的选项的文字
        char buff[20] = { 0 };
        lv_roller_get_selected_str(obj, buff, sizeof(buff));
        printf("opt:%d,opt str:%s\r\n", lv_roller_get_selected(obj)/*获取选择选项的序号*/, buff);
        lv_label_set_text_fmt(g_label_temp, "opt:%d,opt str:%s", lv_roller_get_selected(obj)/*获取选择选项的序号*/, buff);
    }
    else if(obj == speedRoller)
    {
        //获取选择的选项的文字
        char buff[20] = { 0 };
        lv_roller_get_selected_str(obj, buff, sizeof(buff));
        printf("opt:%d,opt str:%s\r\n", lv_roller_get_selected(obj)/*获取选择选项的序号*/, buff);
        lv_label_set_text_fmt(g_label_speed, "opt:%d,opt str:%s", lv_roller_get_selected(obj)/*获取选择选项的序号*/, buff);
    }
    
}

void draw_show_msg_label(void)
{
    g_label_temp = lv_label_create(lv_scr_act());
    lv_label_set_long_mode(g_label_temp, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(g_label_temp, " ");
    lv_obj_align(g_label_temp, LV_ALIGN_CENTER, 0, -scr_act_height() / 5);

    lv_obj_update_layout(g_label_temp);
    g_label_speed = lv_label_create(lv_scr_act());
    lv_label_set_long_mode(g_label_speed, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(g_label_speed, " ");
    lv_obj_align(g_label_speed, LV_ALIGN_CENTER, 0,  -scr_act_height() / 4);
}

void draw_roller(void)
{
    //温度滚轮与label创建
    //
    tempRoller = lv_roller_create(lv_scr_act());
    lv_obj_set_width(tempRoller, scr_act_width()* 5/16 );
    lv_obj_align(tempRoller, LV_ALIGN_CENTER ,-scr_act_width() / 5, 0);
    //设置选项以及滚动模式
    lv_roller_set_options(tempRoller, "18\n19\n20\n21\n22\n23\n24", LV_ROLLER_MODE_INFINITE);
    //设置行间距
    lv_obj_set_style_text_line_space(tempRoller, 10, LV_STATE_DEFAULT);
    //设置默认选项
    lv_roller_set_selected(tempRoller, 4, LV_ANIM_ON);
    //设置可见行数
    lv_roller_set_visible_row_count(tempRoller, 4);
    lv_obj_add_event_cb(tempRoller, test_cb, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_update_layout(tempRoller);
    lv_obj_t* tempLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(tempLabel,"Temp");
    lv_obj_align_to(tempLabel, tempRoller, LV_ALIGN_OUT_TOP_MID, 0, 0);


    //风速滚轮与label创建
    //
    speedRoller = lv_roller_create(lv_scr_act());
    lv_obj_set_width(speedRoller, scr_act_width() * 5 / 16);
    lv_obj_align(speedRoller, LV_ALIGN_CENTER, scr_act_width() / 4, 0);
    //设置选项以及滚动模式
    lv_roller_set_options(speedRoller, "low\nmiddle\nhigh", LV_ROLLER_MODE_INFINITE);
    //设置行间距
    lv_obj_set_style_text_line_space(speedRoller, 10, LV_STATE_DEFAULT);
    //设置默认选项
    lv_roller_set_selected(speedRoller, 1, LV_ANIM_ON);
    //设置可见行数
    lv_roller_set_visible_row_count(speedRoller, 4);
    lv_obj_add_event_cb(speedRoller, test_cb, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_update_layout(speedRoller);
    lv_obj_t* speedLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(speedLabel, "Wind speed");
    lv_obj_align_to(speedLabel, speedRoller, LV_ALIGN_OUT_TOP_MID, 0, 0);
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
    //lv_obj_t* roller = lv_roller_create(lv_scr_act());
    //lv_obj_center(roller);
    //lv_roller_set_options(roller, "1\n2\n3\n4\n5", LV_ROLLER_MODE_INFINITE);
    ////设置每个选项之间的行距
    //lv_obj_set_style_text_line_space(roller, 10, LV_STATE_DEFAULT);
    ////设置默认选择项，最后一个是序号
    //lv_roller_set_selected(roller, 4, LV_ANIM_ON);
    ////设置可见行数
    //lv_roller_set_visible_row_count(roller, 4);
    //lv_obj_add_event_cb(roller, test_cb, LV_EVENT_VALUE_CHANGED, NULL);
    //


    draw_show_msg_label();
    draw_roller();
    return;
}
