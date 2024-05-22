#include "my_gui.h"
#include "lvgl.h"
#include <stdio.h>

#define scr_act_height() lv_obj_get_height(lv_scr_act())
#define scr_act_width() lv_obj_get_width(lv_scr_act())

//为了兼容8，9版本函数
#if (LVGL_VERSION_MAJOR == 8)

#endif


const lv_font_t* g_font;
lv_obj_t* g_label;

#if !USE_FREERTOS
static void timer_cb(lv_timer_t* timer)
{

}
#endif

static void test_cb(lv_event_t* event)
{
    lv_obj_t* obj = lv_event_get_target(event);

    //获取选择的选项的文字
    char buff[20] = { 0 };
    lv_dropdown_get_selected_str(obj, buff, sizeof(buff));
    printf("opt:%d,opt str:%s\r\n", lv_dropdown_get_selected(obj)/*获取选择选项的序号*/,buff);
    lv_label_set_text_fmt(g_label, "opt:%d,opt str:%s", lv_dropdown_get_selected(obj)/*获取选择选项的序号*/, buff);
}

void draw_label(void)
{
    g_label = lv_label_create(lv_scr_act());
    lv_label_set_long_mode(g_label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(g_label, " ");
    lv_obj_align(g_label, LV_ALIGN_CENTER, 0, -scr_act_height() / 5);
}

void draw_dropdowm(void)
{
    lv_obj_t* dropdown = lv_dropdown_create(lv_scr_act());
    lv_obj_center(dropdown);
    lv_dropdown_set_options(dropdown, "1990\n1991\n1992\n1993\n1995\n1996");
    //参数为obj，选项名字，选项位置，位置从0开始
    lv_dropdown_add_option(dropdown, "1994", 4);
    //设置默认选择项，最后一个是序号
    lv_dropdown_set_selected(dropdown, 6);
    lv_obj_add_event_cb(dropdown, test_cb, LV_EVENT_VALUE_CHANGED, NULL);
}

void draw_dropdowm_special(void)
{
    lv_obj_t* dropdown2 = lv_dropdown_create(lv_scr_act());
    lv_obj_align(dropdown2, LV_ALIGN_CENTER, 0 , scr_act_height()/5);
    /*设置列表展开方向*/
    lv_dropdown_set_dir(dropdown2, LV_DIR_RIGHT);
    //重新设置下拉列表右边的那个箭头图标
    lv_dropdown_set_symbol(dropdown2, LV_SYMBOL_RIGHT);
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
    //lv_obj_t* dropdown = lv_dropdown_create(lv_scr_act());
    //lv_obj_center(dropdown);
    //lv_dropdown_set_options(dropdown, "1\n2\n3\n4\n5");
    ////参数为obj，选项名字，选项位置，位置从0开始
    //lv_dropdown_add_option(dropdown, "4.5", 4);
    ////设置默认选择项，最后一个是序号
    //lv_dropdown_set_selected(dropdown, 4);
    //lv_obj_add_event_cb(dropdown, test_cb, LV_EVENT_VALUE_CHANGED, NULL);
    


    draw_label();
    draw_dropdowm();
    draw_dropdowm_special();
    return;
}
