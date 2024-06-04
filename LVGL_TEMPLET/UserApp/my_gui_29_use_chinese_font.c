#include "my_gui.h"
#include "lvgl.h"
#include <stdio.h>

int32_t scr_act_width;
int32_t scr_act_hight;
LV_FONT_DECLARE(font_30);


static void test_cb(lv_event_t* event)
{

}


void GUI_test(void)
{
    //获取活动屏幕宽高
    const lv_font_t* font;
    scr_act_width = lv_obj_get_width(lv_scr_act());
    scr_act_hight = lv_obj_get_height(lv_scr_act());
    if (scr_act_width <= 320)
    {
        font = &lv_font_montserrat_14;
    }
    else if (scr_act_width <= 480)
    {
        font = &lv_font_montserrat_18;
    }
    else
    {
        font = &lv_font_montserrat_22;
    }

    //设置背景图层以及阴影
    lv_obj_t* fatherShadow = lv_obj_create(lv_scr_act());
    lv_obj_set_size(fatherShadow, scr_act_width - 20, scr_act_hight - 20);
    lv_obj_set_style_bg_color(fatherShadow, lv_color_make(174, 66, 125), LV_STATE_DEFAULT);
    lv_obj_set_style_opa(fatherShadow, LV_OPA_50, LV_STATE_DEFAULT | LV_PART_MAIN);

    lv_obj_t* father = lv_obj_create(lv_scr_act());
    lv_obj_set_size(father, scr_act_width - 20, scr_act_hight - 20);
    lv_obj_set_align(father, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(father, lv_color_make(237, 80, 138), LV_STATE_DEFAULT);

    lv_obj_align_to(fatherShadow, father, LV_ALIGN_CENTER, 5, 5);

    //添加三个label
    int32_t father_width = lv_obj_get_width(father);
    int32_t father_hight = lv_obj_get_height(father);

    lv_obj_t* topLabel = lv_label_create(father);
    lv_obj_set_size(topLabel, father_width / 2, father_hight / 3);
    lv_obj_set_align(topLabel, LV_ALIGN_TOP_MID);
    lv_obj_set_style_bg_color(topLabel, lv_color_make(92, 203, 176), LV_PART_MAIN);//不知道为啥没生效
    lv_obj_set_style_opa(topLabel, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_text_font(topLabel, font, LV_PART_MAIN);
    lv_obj_set_style_text_align(topLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_label_set_long_mode(topLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_style_text_font(topLabel, &font_30, LV_PART_MAIN);
    lv_label_set_text(topLabel, "郝本通测试");

    lv_obj_t* centerLabel = lv_label_create(father);
    lv_obj_set_size(centerLabel, father_width / 2, father_hight / 3);
    lv_obj_set_align(centerLabel, LV_ALIGN_CENTER);
    lv_obj_set_style_text_font(centerLabel, font, LV_PART_MAIN);
    lv_obj_set_style_text_align(centerLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_text_color(centerLabel, lv_color_make(96, 203, 175), LV_PART_MAIN);
    lv_label_set_long_mode(centerLabel, LV_LABEL_LONG_DOT);
    lv_label_set_text_fmt(centerLabel, "qianqian I LOVE YOU %d years,you are my baby \n from fmt funtion", 10000);






    lv_obj_t* bottomLabel = lv_label_create(father);
    lv_obj_set_size(bottomLabel, father_width / 2, father_hight / 3);
    lv_obj_set_align(bottomLabel, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_style_text_font(bottomLabel, font, LV_PART_MAIN);
    lv_obj_set_style_text_align(bottomLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_label_set_long_mode(bottomLabel, LV_LABEL_LONG_DOT);
#if (LVGL_VERSION_MAJOR != 9)
    lv_label_set_recolor(bottomLabel, true);
#endif
    lv_label_set_text(bottomLabel, "qianqian #ff0000 I LOVE YOU,you are my baby# ");

    return;
}
