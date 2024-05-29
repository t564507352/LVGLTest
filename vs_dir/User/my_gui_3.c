#include "my_gui.h"
#include "lvgl.h"
#include <stdio.h>


const lv_font_t* g_font;
lv_obj_t* g_curSpeed;
uint8_t g_showSpeed = 0;

lv_obj_t* g_topBtn;
lv_obj_t* g_centerBtn;
lv_obj_t* g_bottomBtn;

static void test_cb(lv_event_t* event)
{
	//获取并判断obj，做出相应的全局speed变量的+，-
    lv_obj_t* whichObj = lv_event_get_target(event);
    if (whichObj == g_topBtn)
    {
        if (g_showSpeed < 100) g_showSpeed += 10;
        printf("speed + \r\n");
    }
    else if (whichObj == g_centerBtn)
    {
        g_showSpeed = 0;
        printf("STOP \r\n");
       
    }
    else if (whichObj == g_bottomBtn)
    {
        if (g_showSpeed >= 10) g_showSpeed -= 10;
        printf("speed - \r\n");
    }
	//格式化文本并显示新speed
    lv_label_set_text_fmt(g_curSpeed, "The Speed is %d RPM", g_showSpeed);
}

void top_label_create(void)
{
	//创建label
    g_curSpeed = lv_label_create(lv_scr_act());
    lv_obj_align(g_curSpeed, LV_ALIGN_TOP_MID, 0, lv_obj_get_height(lv_scr_act())/10);
    lv_obj_set_style_text_font(g_curSpeed, g_font, LV_PART_MAIN);
    lv_obj_set_style_text_align(g_curSpeed, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
	//设置长文本模式为循环滚动
    lv_label_set_long_mode(g_curSpeed, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(g_curSpeed, "The Speed is 0 RPM");
}

void top_btn_create(void)
{

    //设置按钮
    g_topBtn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(g_topBtn, lv_obj_get_width(lv_scr_act()) / 3, lv_obj_get_height(lv_scr_act())/10);
    lv_obj_align(g_topBtn, LV_ALIGN_TOP_MID, 0, lv_obj_get_height(lv_scr_act()) * 1 / 5);
    lv_obj_set_style_bg_color(g_topBtn, lv_color_make(244, 212, 173), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_t* label = lv_label_create(g_topBtn);
	
	
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_font(label, g_font, LV_PART_MAIN);
    lv_label_set_text(label, "SPEED +");
    lv_obj_add_event_cb(g_topBtn, test_cb, LV_EVENT_CLICKED, NULL);
}

void center_btn_create(void)
{
    //设置按钮
    g_centerBtn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(g_centerBtn, lv_obj_get_width(lv_scr_act()) * 2 / 5, lv_obj_get_height(lv_scr_act()) /10 );
    lv_obj_align(g_centerBtn, LV_ALIGN_TOP_MID, 0, lv_obj_get_height(lv_scr_act()) * 2 / 5);
    lv_obj_set_style_bg_color(g_centerBtn, lv_color_make(214, 60, 54), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_t* label = lv_label_create(g_centerBtn);
	
	
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_font(label, g_font, LV_PART_MAIN);
    lv_label_set_text(label, "STOP");
    lv_obj_add_event_cb(g_centerBtn, test_cb, LV_EVENT_CLICKED, NULL);
    //lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);//增加UI状态切换的，本项目用不到
}

void bottom_btn_create(void)
{
    //创建按钮对象
    g_bottomBtn = lv_btn_create(lv_scr_act());
	//设置按钮大小
    lv_obj_set_size(g_bottomBtn, lv_obj_get_width(lv_scr_act()) / 3, lv_obj_get_height(lv_scr_act()) / 10);
	//设置按钮对齐
    lv_obj_align(g_bottomBtn, LV_ALIGN_TOP_MID, 0, lv_obj_get_height(lv_scr_act()) * 3 / 5);
	//设置按钮背景颜色
    lv_obj_set_style_bg_color(g_bottomBtn, lv_color_make(169, 182, 82), LV_PART_MAIN | LV_STATE_DEFAULT);
	
	
	//创建bottomBtn的子对象label对象
    lv_obj_t* label = lv_label_create(g_bottomBtn);
	//设置在夫对象中对齐
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
	//设置字体
    lv_obj_set_style_text_font(label, g_font, LV_PART_MAIN);
	//设置文本
    lv_label_set_text(label, "SPEED -");
	//添加事件，CLICKED时触发事件
    lv_obj_add_event_cb(g_bottomBtn, test_cb, LV_EVENT_CLICKED, NULL);
}

void GUI_test(void)
{
    //获取活动屏幕宽高，选择字体

    int32_t scr_act_width = lv_obj_get_width(lv_scr_act());
    int32_t scr_act_hight = lv_obj_get_height(lv_scr_act());
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
    top_btn_create();
    center_btn_create();
    bottom_btn_create();
    return;
}
