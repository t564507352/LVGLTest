#include "my_gui.h"
#include "lvgl.h"
#include <stdio.h>

#define scr_act_height() lv_obj_get_height(lv_scr_act())
#define scr_act_width() lv_obj_get_width(lv_scr_act())

//为了兼容8，9版本函数
#if (LVGL_VERSION_MAJOR == 8)

#endif


const lv_font_t* g_font;
lv_obj_t* g_base_left;
lv_obj_t* g_base_right;
lv_obj_t* list;
lv_obj_t* btn;
lv_obj_t* label;

#if !USE_FREERTOS
static void timer_cb(lv_timer_t* timer)
{

}
#endif

static void test_cb(lv_event_t* event)
{
    lv_obj_t* btn = lv_event_get_target(event);
    lv_label_set_text(label, lv_list_get_btn_text(list, btn));
    printf("%s\r\n", lv_list_get_btn_text(list, btn));
    lv_obj_add_state(btn, LV_STATE_FOCUS_KEY);
}

void draw_base(void)
{
    g_base_left = lv_obj_create(lv_scr_act());
    lv_obj_set_size(g_base_left, scr_act_width() /2, scr_act_height() * 9 / 10);
    lv_obj_align(g_base_left, LV_ALIGN_LEFT_MID, scr_act_width() / 50, 0);

    g_base_right = lv_obj_create(lv_scr_act());
    lv_obj_set_size(g_base_right, scr_act_width() *9 / 20 , scr_act_height() * 9 / 10);
    lv_obj_align(g_base_right, LV_ALIGN_RIGHT_MID, -scr_act_width() / 50, 0);
    lv_obj_update_layout(g_base_left);
    lv_obj_update_layout(g_base_right);
}


void draw_list(void)
{
    list = lv_list_create(g_base_left);
    lv_obj_set_size(list, lv_obj_get_width(g_base_left) * 4 / 5, lv_obj_get_height(g_base_left) * 9 / 10);
    lv_obj_align(list, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_list_add_text(list, "setting");
    btn = lv_list_add_btn(list, LV_SYMBOL_AUDIO, "AUDIO");
    lv_obj_add_event_cb(btn, test_cb, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list, LV_SYMBOL_VIDEO, "VIDEO");
    lv_obj_add_event_cb(btn, test_cb, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list, LV_SYMBOL_GPS, "GPS");
    lv_obj_add_event_cb(btn, test_cb, LV_EVENT_CLICKED, NULL);
    lv_list_add_text(list, "link");
    lv_obj_add_event_cb(btn, test_cb, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list, LV_SYMBOL_KEYBOARD, "KEYBOARD");
    lv_obj_add_event_cb(btn, test_cb, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list, LV_SYMBOL_USB, "USB");
    lv_obj_add_event_cb(btn, test_cb, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list, LV_SYMBOL_BLUETOOTH, "BLUETOOTH");
    lv_obj_add_event_cb(btn, test_cb, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list, LV_SYMBOL_SD_CARD, "CARD");
    lv_obj_add_event_cb(btn, test_cb, LV_EVENT_CLICKED, NULL);
    lv_list_add_text(list, "other");
    btn = lv_list_add_btn(list, LV_SYMBOL_PREV, "PREV");
    lv_obj_add_event_cb(btn, test_cb, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list, LV_SYMBOL_AUDIO, "AUDIO");
    lv_obj_add_event_cb(btn, test_cb, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list, LV_SYMBOL_AUDIO, "AUDIO");
    lv_obj_add_event_cb(btn, test_cb, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list, LV_SYMBOL_AUDIO, "AUDIO");
    lv_obj_add_event_cb(btn, test_cb, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list, LV_SYMBOL_AUDIO, "AUDIO");
    lv_obj_add_event_cb(btn, test_cb, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list, LV_SYMBOL_AUDIO, "AUDIO");
    lv_obj_add_event_cb(btn, test_cb, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list, LV_SYMBOL_AUDIO, "AUDIO");
    lv_obj_add_event_cb(btn, test_cb, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list, LV_SYMBOL_AUDIO, "AUDIO");
    lv_obj_add_event_cb(btn, test_cb, LV_EVENT_CLICKED, NULL);
}

void draw_label(void)
{
    label = lv_label_create(g_base_right);
    lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(label, " ");
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 0, 0);
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

    ////测试代码
    //lv_obj_t* list = lv_list_create(lv_scr_act());
    //lv_obj_center(list);
    //lv_list_add_text(list, "setting");
    //lv_obj_t* btn = lv_list_add_btn(list, LV_SYMBOL_AUDIO, "AUDIO");
    //printf("%s", lv_list_get_btn_text(list, btn));
    ////添加按钮状态，被聚焦按下
    //lv_obj_add_state(btn, LV_STATE_FOCUS_KEY);

    draw_base();
    draw_list();
    draw_label();
    return;
}
