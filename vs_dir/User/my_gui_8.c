#include "my_gui.h"
#include "lvgl.h"
#include <stdio.h>

#define scr_act_height() lv_obj_get_height(lv_scr_act())
#define scr_act_width() lv_obj_get_width(lv_scr_act())

//为了兼容8，9版本函数
#if (LVGL_VERSION_MAJOR == 8)

#endif


const lv_font_t* g_font;
lv_obj_t* g_base;
lv_obj_t* g_bedLED;
lv_obj_t* g_livingLED;
lv_obj_t* g_bathLED;


#if !USE_FREERTOS
static void timer_cb(lv_timer_t* timer)
{

}
#endif

static void test_cb(lv_event_t* event)
{
    lv_obj_t* whiceOBJ = lv_event_get_target(event);
    lv_led_toggle(whiceOBJ);
}

void show_bedroom_led(void)
{
    g_bedLED = lv_led_create(g_base);
    lv_obj_set_size(g_bedLED, scr_act_width() / 5, scr_act_width() / 5);
    lv_obj_align(g_bedLED, LV_ALIGN_CENTER, 0, -scr_act_height()/4);
    //设置LED灯的颜色
    lv_led_set_color(g_bedLED, lv_color_make(248, 212, 233));
    lv_obj_add_event_cb(g_bedLED, test_cb, LV_EVENT_CLICKED, NULL);


    lv_obj_update_layout(g_bedLED);
    lv_obj_t* bedroomLabel = lv_label_create(g_base);
    lv_obj_align_to(bedroomLabel, g_bedLED, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_label_set_text(bedroomLabel, "bedroom");
}

void show_living_room_led(void)
{
    g_livingLED = lv_led_create(g_base);
    lv_obj_set_size(g_livingLED, scr_act_width() / 5, scr_act_width() / 5);
    lv_obj_align(g_livingLED, LV_ALIGN_CENTER, 0, 0);
    lv_led_set_color(g_livingLED, lv_color_make(237, 212, 69));
    lv_obj_add_event_cb(g_livingLED, test_cb, LV_EVENT_CLICKED, NULL);


    lv_obj_update_layout(g_livingLED);
    lv_obj_t* livingroomLabel = lv_label_create(g_base);
    lv_obj_align_to(livingroomLabel, g_livingLED, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_label_set_text(livingroomLabel, "livingroom");
}

void show_bath_room_led(void)
{
    g_bathLED = lv_led_create(g_base);
    lv_obj_set_size(g_bathLED, scr_act_width() / 5, scr_act_width() / 5);
    lv_obj_align(g_bathLED, LV_ALIGN_CENTER, 0, scr_act_height() / 4);
    lv_obj_add_event_cb(g_bathLED, test_cb, LV_EVENT_CLICKED, NULL);


    lv_obj_update_layout(g_bathLED);
    lv_obj_t* bathroomLabel = lv_label_create(g_base);
    lv_obj_align_to(bathroomLabel, g_bathLED, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_label_set_text(bathroomLabel, "bedroom");
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
    //lv_obj_t* led = lv_led_create(lv_scr_act());
    //lv_obj_set_size(led, scr_act_width()/5, scr_act_width()/5);
    //lv_obj_center(led);
    //lv_led_set_color(led, lv_color_make(237, 212, 69));
    //lv_led_set_brightness(led, LV_LED_BRIGHT_MIN*2);
    //lv_led_off(led);
    //lv_led_on(led);
    //lv_led_toggle(led);


    //创建基础对象,整体更好看
    g_base = lv_obj_create(lv_scr_act());
    lv_obj_set_size(g_base, scr_act_width() * 9 / 10, scr_act_height() * 9 / 10);
    lv_obj_center(g_base);

    show_bedroom_led();
    show_living_room_led();
    show_bath_room_led();
    return;
}
