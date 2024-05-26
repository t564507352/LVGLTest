#include "my_gui.h"
#include "lvgl.h"
#include <stdio.h>
#include "math.h"
#include <stdlib.h>
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

LV_IMG_DECLARE(img_marry);
const lv_font_t* g_font;
lv_obj_t* g_img;

lv_obj_t* g_slider_zoom;
lv_obj_t* g_slider_angle;
lv_obj_t* g_slider_opa;
lv_obj_t* g_slider_r;
lv_obj_t* g_slider_g;
lv_obj_t* g_slider_b;


#if !USE_FREERTOS
static void timer_cb(lv_timer_t* timer)
{

}
#endif

static void test_cb(lv_event_t* event)
{
    //lv_obj_t* target = lv_event_get_target(event);
    lv_img_set_zoom(g_img, lv_slider_get_value(g_slider_zoom));
    lv_img_set_angle(g_img, lv_slider_get_value(g_slider_angle));
    lv_obj_set_style_img_recolor_opa(g_img, lv_slider_get_value(g_slider_opa), LV_PART_MAIN);
    lv_obj_set_style_img_recolor(g_img, lv_color_make(lv_slider_get_value(g_slider_r), lv_slider_get_value(g_slider_g), lv_slider_get_value(g_slider_b)), LV_PART_MAIN);

}

static lv_obj_t* create_slider(lv_color_t color)
{
    lv_obj_t* slider = lv_slider_create(lv_scr_act());
    lv_obj_set_size(slider, scr_act_width() * 4 / 5, scr_act_height() / 24);
    lv_obj_remove_style(slider, NULL, LV_PART_KNOB);
    lv_obj_set_style_bg_color(slider, lv_color_make(183, 157, 166), LV_PART_MAIN);
    lv_obj_set_style_bg_color(slider, color, LV_PART_INDICATOR);
    lv_obj_set_style_bg_opa(slider, 100, LV_PART_MAIN);
    lv_obj_add_event_cb(slider, test_cb, LV_EVENT_VALUE_CHANGED, NULL);
    return slider;
}

void show_img()
{
    g_img = lv_img_create(lv_scr_act());
    lv_img_set_src(g_img,&img_marry);
    lv_obj_align(g_img, LV_ALIGN_TOP_MID, 0, scr_act_height() / 12);

}

void creat_title_label(lv_obj_t* base, const char* title)
{
    lv_obj_t* label = lv_label_create(lv_scr_act());
    lv_obj_align_to(label, base, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_label_set_text(label, title);
}

//红色、绿色和蓝色分别由 #FF0000, #00FF00, 和 #0000FF
void show_slider()
{
    g_slider_zoom = create_slider(lv_color_make(65, 59, 49));
    lv_slider_set_range(g_slider_zoom, 128, 256+128);/*二分之一倍到正常大小*/
    lv_slider_set_value(g_slider_zoom, 256, LV_ANIM_ON);
    lv_obj_align(g_slider_zoom, LV_ALIGN_CENTER, 0, 0);
    creat_title_label(g_slider_zoom, "zoom");
    
    g_slider_angle = create_slider(lv_color_make(164, 143, 174));
    lv_slider_set_range(g_slider_angle, 0, 3600);/*可以旋转一周360°*/
    lv_obj_align_to(g_slider_angle, g_slider_zoom, LV_ALIGN_OUT_BOTTOM_LEFT, 0, scr_act_height() / 24);
    creat_title_label(g_slider_angle, "angle");

    g_slider_opa = create_slider(lv_color_make(179, 147, 101));
    lv_slider_set_range(g_slider_opa, 0, 255);/*重新着色透明度*/
    lv_obj_align_to(g_slider_opa, g_slider_angle, LV_ALIGN_OUT_BOTTOM_LEFT, 0, scr_act_height() / 24);
    creat_title_label(g_slider_opa, "opa");


    g_slider_r = create_slider(lv_color_hex(0xFF0000));
    lv_slider_set_range(g_slider_r, 0, 255);/*红色*/
    lv_obj_align_to(g_slider_r, g_slider_opa, LV_ALIGN_OUT_BOTTOM_LEFT, 0, scr_act_height() / 24);
    creat_title_label(g_slider_r, "red");

    lv_obj_update_layout(g_slider_r);
    g_slider_g = create_slider(lv_color_hex(0x00FF00));
    lv_slider_set_range(g_slider_g, 0, 255);/*绿色*/
    lv_obj_align_to(g_slider_g, g_slider_r, LV_ALIGN_OUT_BOTTOM_LEFT, 0, scr_act_height() / 24);
    creat_title_label(g_slider_g, "green");

    g_slider_b = create_slider(lv_color_hex(0x0000FF));
    lv_slider_set_range(g_slider_b, 0, 255);/*蓝色*/
    lv_obj_align_to(g_slider_b, g_slider_g, LV_ALIGN_OUT_BOTTOM_LEFT, 0, scr_act_height() / 24);
    creat_title_label(g_slider_b, "blue");
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
    //lv_obj_t* img = lv_img_create(lv_scr_act());
    ////设置图片源，设置前需先LV_IMG_DECLARE声明
    //lv_img_set_src(img,&img_marry);

    ////设置图片X,Y的偏移
    //lv_img_set_offset_x(img,50);
    //lv_img_set_offset_y(img, 50);

    ////图片重新着色，需配合opa使用
    //lv_obj_set_style_img_recolor(img, lv_color_make(255, 238, 168), LV_PART_MAIN);
    //lv_obj_set_style_img_recolor_opa(img, 50, LV_PART_MAIN);  /*0-255*/

    ////设置支点，不设置默认在图片中间，设置好后根据设置的点进行后续的图片的放大，旋转
    //lv_obj_update_layout(img);
    //lv_img_set_pivot(img, lv_obj_get_width(img), lv_obj_get_height(img));

    ////设置图片的缩放与旋转
    //lv_img_set_zoom(img, 128);/*128 二分之一倍 256正常 512 两倍*/
    //lv_img_set_angle(img, 900); /*-顺时针转90°  要转的度数后多加个0 */

    show_img();
    show_slider();

    return;
}
