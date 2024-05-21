#include "my_gui.h"
#include "lvgl.h"
#include <stdio.h>


#define scr_act_height() lv_obj_get_height(lv_scr_act())
#define scr_act_width() lv_obj_get_width(lv_scr_act())

const lv_font_t* g_font;

lv_obj_t* coolSwitch;
lv_obj_t* hotSwitch;



static void test_cb(lv_event_t* event)
{
    lv_obj_t* whichObj = lv_event_get_target(event);
    if (whichObj == coolSwitch)
    {
        if (lv_obj_has_state(hotSwitch, LV_STATE_CHECKED))//如果检测到制热开关状态是开的
        {
            lv_obj_clear_state(hotSwitch, LV_STATE_CHECKED);//清除制热开关的开状态
        }
    }
    else if (whichObj == hotSwitch)
    {
        if (lv_obj_has_state(coolSwitch, LV_STATE_CHECKED))
        {
#if(LVGL_VERSION_MAJOR == 9)
            lv_obj_set_state(coolSwitch, LV_STATE_CHECKED, false); /*只有9.0有，等同于下面的函数*/
#else
            lv_obj_clear_state(coolSwitch, LV_STATE_CHECKED);
#endif
        }
    }
}

//顶部文字区域
void top_label_create(void)
{
    lv_obj_t* label = lv_label_create(lv_scr_act());
    lv_obj_align(label, LV_ALIGN_CENTER ,0, - scr_act_height() * 9 / 20);
    lv_obj_set_style_text_font(label, g_font, LV_PART_MAIN);
    lv_label_set_text(label, "control center");
}

//制冷开关
void cool_switch_create(void)
{
    lv_obj_t* base = lv_obj_create(lv_scr_act());
    lv_obj_set_size(base, scr_act_width() / 3, scr_act_width() / 3);
    lv_obj_align(base, LV_ALIGN_CENTER, 0, - scr_act_height() / 4);

    lv_obj_t* coolLabel = lv_label_create(base);
    lv_obj_align(coolLabel, LV_ALIGN_TOP_MID, 0, lv_obj_get_width(base) * 1 / 5);
    lv_label_set_text(coolLabel,"COOL");


    coolSwitch = lv_switch_create(base);
    lv_obj_align(coolSwitch, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(coolSwitch, lv_color_make(129, 212, 250), LV_PART_KNOB | LV_EVENT_FOCUSED);
    lv_obj_add_event_cb(coolSwitch, test_cb, LV_EVENT_CLICKED, NULL);

}

//制热开关
void hot_switch_create(void)
{
    lv_obj_t* base = lv_obj_create(lv_scr_act());
    lv_obj_set_size(base, scr_act_width() / 3, scr_act_width() / 3);
    lv_obj_align(base, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t* coolLabel = lv_label_create(base);
    lv_obj_align(coolLabel, LV_ALIGN_TOP_MID, 0, lv_obj_get_width(base) * 1 / 5);
    lv_label_set_text(coolLabel, "HOT");


    hotSwitch = lv_switch_create(base);
    lv_obj_align(hotSwitch, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(hotSwitch, lv_color_make(243, 191, 228), LV_PART_KNOB | LV_EVENT_FOCUSED);//有PART部位,STATE状态，EVENT事件可以选择
    lv_obj_set_style_bg_color(hotSwitch, lv_color_make(164, 43, 45), LV_PART_INDICATOR | LV_STATE_CHECKED );//指示器必须选择STATE，不然默认为默认状态，打开后就不生效了
    lv_obj_add_event_cb(hotSwitch, test_cb, LV_EVENT_CLICKED, NULL);
}

//除湿开关
void xeransis_switch_create(void)
{
    //创建基础对象
    lv_obj_t* base = lv_obj_create(lv_scr_act());
    //设置大小均为屏幕宽度的三分之一
    lv_obj_set_size(base, scr_act_width() / 3, scr_act_width() / 3);
    //设置中心对其，向上偏移
    lv_obj_align(base, LV_ALIGN_CENTER, 0, scr_act_height() / 4);

    lv_obj_t* coolLabel = lv_label_create(base);
    lv_obj_align(coolLabel, LV_ALIGN_TOP_MID, 0, lv_obj_get_width(base) * 1 / 5);
    lv_label_set_text(coolLabel, "xeransis");


    lv_obj_t* xeransisSwitch = lv_switch_create(base);
    lv_obj_align(xeransisSwitch, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_state(xeransisSwitch, LV_STATE_CHECKED | LV_STATE_DISABLED);//添加LV_STATE_DISABLED后就不可编辑了
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
    top_label_create();
    cool_switch_create();
    hot_switch_create();
    xeransis_switch_create();
    return;
}
