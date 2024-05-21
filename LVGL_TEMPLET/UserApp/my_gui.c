#include "my_gui.h"
#include "lvgl.h"
#include <stdio.h>


#define scr_act_height() lv_obj_get_height(lv_scr_act())
#define scr_act_width() lv_obj_get_width(lv_scr_act())

#define PRICECOLA               4
#define PRICECHICKEN            8
#define PRICEHAMBURGER          12
#define PRICETABLEWARE          1


const lv_font_t* g_font;
uint32_t g_product = 1;
lv_obj_t* g_priceLabel;
lv_obj_t* g_checkbox1;
lv_obj_t* g_checkbox2;
lv_obj_t* g_checkbox3;
lv_obj_t* g_checkbox4;

static void test_cb(lv_event_t* event)
{
    //获取事件对象
    lv_obj_t* whiceObj = lv_event_get_target(event);
    //判断并计算价格
    if (whiceObj == g_checkbox1)
    {
        lv_obj_has_state(whiceObj, LV_STATE_CHECKED) ? (g_product += PRICECOLA) : (g_product -= PRICECOLA);
    }
    else if (whiceObj == g_checkbox2)
    {
        lv_obj_has_state(whiceObj, LV_STATE_CHECKED)? (g_product+= PRICECHICKEN): (g_product -= PRICECHICKEN);
    }
    else if (whiceObj == g_checkbox3)
    {
        lv_obj_has_state(whiceObj, LV_STATE_CHECKED) ? (g_product += PRICEHAMBURGER) : (g_product -= PRICEHAMBURGER);
    }
    //重新显示价格
    lv_label_set_text_fmt(g_priceLabel, "total price: %d $ ", g_product);
}


void show_label(void)
{
    // 设置顶部标题
    lv_obj_t* titleLabel = lv_label_create(lv_scr_act());
    lv_obj_align(titleLabel, LV_ALIGN_CENTER, 0, -scr_act_height()/3);
    lv_obj_set_style_text_font(titleLabel, g_font, LV_PART_MAIN);
    lv_label_set_long_mode(titleLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(titleLabel,"Please select product: ");

    //设置底部计价板
    g_priceLabel = lv_label_create(lv_scr_act());
    lv_obj_align(g_priceLabel, LV_ALIGN_CENTER, 0, scr_act_height() / 3);
    lv_obj_set_style_text_font(g_priceLabel, g_font, LV_PART_MAIN);
    lv_label_set_long_mode(g_priceLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(g_priceLabel, "total price: 0 $ ");
}
void show_checkbox(void)
{
    char ShowTextBuff[20];
    //添加底部基础obj
    lv_obj_t* base = lv_obj_create(lv_scr_act());
    lv_obj_set_size(base, scr_act_width(), scr_act_height() / 2);
    lv_obj_align(base, LV_ALIGN_CENTER, 0 ,0);


    //创建4个checkbox
    g_checkbox1 = lv_checkbox_create(base);
    lv_obj_align(g_checkbox1, LV_ALIGN_TOP_LEFT, 0, lv_obj_get_height(base)/8);
    //设置复选框后面的文字与复选框的间距
    lv_obj_set_style_pad_column(g_checkbox1, 20, LV_STATE_DEFAULT);
    sprintf(ShowTextBuff, "cola   $ %d", PRICECOLA);
    lv_checkbox_set_text(g_checkbox1, (const char*)ShowTextBuff);
    //添加事件，事件类型为“值改变”触发
    lv_obj_add_event_cb(g_checkbox1, test_cb, LV_EVENT_VALUE_CHANGED, NULL);


    g_checkbox2 = lv_checkbox_create(base);
    lv_obj_align_to(g_checkbox2, g_checkbox1, LV_ALIGN_OUT_BOTTOM_LEFT, 0, lv_obj_get_height(base) / 10);
    lv_obj_set_style_pad_column(g_checkbox1, 20, LV_STATE_DEFAULT);
    sprintf(ShowTextBuff, "fried chicken   $ %d", PRICECHICKEN);
    lv_checkbox_set_text(g_checkbox2, (const char*)ShowTextBuff);
    lv_obj_add_event_cb(g_checkbox2, test_cb, LV_EVENT_VALUE_CHANGED, NULL);

    g_checkbox3 = lv_checkbox_create(base);
    lv_obj_align_to(g_checkbox3, g_checkbox2, LV_ALIGN_OUT_BOTTOM_LEFT, 0, lv_obj_get_height(base) / 10);
    lv_obj_set_style_pad_column(g_checkbox1, 20, LV_STATE_DEFAULT);
    sprintf(ShowTextBuff, "hamburger   $ %d", PRICEHAMBURGER);
    lv_checkbox_set_text(g_checkbox3, (const char*)ShowTextBuff);
    lv_obj_add_event_cb(g_checkbox3, test_cb, LV_EVENT_VALUE_CHANGED, NULL);

    g_checkbox4 = lv_checkbox_create(base);
    lv_obj_align_to(g_checkbox4, g_checkbox3, LV_ALIGN_OUT_BOTTOM_LEFT, 0, lv_obj_get_height(base) / 10);
    lv_obj_set_style_pad_column(g_checkbox1, 20, LV_STATE_DEFAULT);
    sprintf(ShowTextBuff, "tableware   $ %d", PRICETABLEWARE);
    lv_checkbox_set_text(g_checkbox4, (const char*)ShowTextBuff);
    //添加默认状态，不可操作，并选中
    lv_obj_add_state(g_checkbox4, LV_STATE_DISABLED | LV_STATE_CHECKED);
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
    show_label();
    show_checkbox();
    return;
}
