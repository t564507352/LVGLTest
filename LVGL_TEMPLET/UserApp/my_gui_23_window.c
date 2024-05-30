#include "my_gui.h"
#include "lvgl.h"
#include <stdio.h>
#include "math.h"
#include <stdlib.h>
#include <string.h>
#if USE_FREERTOS
#include "lcd_function.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#endif	

#define scr_act_height() lv_obj_get_height(lv_scr_act())
#define scr_act_width() lv_obj_get_width(lv_scr_act())
#define WIN_CONTENT_LABEL_MAX    2
#define WIN_CONTENT_BTN_MAX      2

//为了兼容8，9版本函数
#if (LVGL_VERSION_MAJOR == 9)
#define lv_win_add_btn(x,y,z) lv_win_add_button(x,y,z)
#endif


static int16_t calc_offset_len(uint16_t baseLenOneSide, uint16_t ElemLenOneSide, uint8_t NumOfElem, uint8_t elemId, uint8_t alignType);

const lv_font_t* g_font;
typedef struct
{
    lv_obj_t* obj;
    lv_obj_t* header;
    lv_obj_t* title;
    lv_obj_t* btn;
    lv_obj_t* content;
    lv_obj_t* contentLabel[WIN_CONTENT_LABEL_MAX];
    lv_obj_t* contentSlider[WIN_CONTENT_BTN_MAX];
}_widgets_windows;


enum userEventID {
    EVENT_WIN_BTN = 0,                 // 0
    EVENT_WIN_CONTENT_SLIDER_SOUND,    // 1
    EVENT_WIN_CONTENT_SLIDER_BG_LT,    // 2
};

static const uint8_t userEventCode[3] = { EVENT_WIN_BTN, EVENT_WIN_CONTENT_SLIDER_SOUND, EVENT_WIN_CONTENT_SLIDER_BG_LT };


#if !USE_FREERTOS
static void timer_cb(lv_timer_t* timer)
{

}
#endif


static void test_cb(lv_event_t* event)
{
    lv_obj_t* target = lv_event_get_target(event);
    uint8_t userEventCode = *(uint8_t*)event->user_data;
    switch (userEventCode)
    {
    case EVENT_WIN_BTN:
    {
        lv_obj_add_flag(target->parent->parent, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:break;
    }
}



void show_win(void)
{
    _widgets_windows win;
    //-------------part1,创建零件---------------------------

    //创建窗口
#if (LVGL_VERSION_MAJOR == 9)
    win.obj = lv_win_create(lv_scr_act());
#else
    win.obj = lv_win_create(lv_scr_act(), 20);
#endif

    //获取窗口的头部对象
    win.header = lv_win_get_header(win.obj);
    //添加标题并获取标题对象
    //uint8_t titleBuf[20];
    //sprintf(titleBuf,"%s setting", LV_SYMBOL_SETTINGS);
    win.title = lv_win_add_title(win.obj, LV_SYMBOL_SETTINGS" setting");
    //添加按钮并获取按钮对象
    win.btn = lv_win_add_btn(win.obj, LV_SYMBOL_CLOSE, 20);
    //获取窗口的容器对象
    win.content = lv_win_get_content(win.obj);
    //以窗口的容器为父对象创建其他子对象
    win.contentLabel[0] = lv_label_create(win.content);
    win.contentSlider[0] = lv_slider_create(win.content);
    win.contentSlider[1] = lv_slider_create(win.content);

    //----------------------part2,设置各对象的内容，大小和位置----------------------------


    //设置窗口大小，居中位置
    lv_obj_set_size(win.obj, scr_act_width() * 4 / 5, scr_act_height() * 2 / 3);
    lv_obj_center(win.obj);
    lv_obj_update_layout(win.obj);

#if (LVGL_VERSION_MAJOR == 9)
    //设置头部高度
    lv_obj_set_height(win.header, 20);
#endif

    //设置label和slider的大小与对齐
    lv_obj_set_width(win.contentLabel[0], lv_obj_get_width(win.contentLabel[0]->parent) * 9 / 10);
    lv_obj_set_style_text_font(win.contentLabel[0], g_font, LV_PART_MAIN);
    lv_obj_set_style_text_align(win.contentLabel[0], LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_align(win.contentLabel[0], LV_ALIGN_TOP_MID, 0, 20);
    lv_label_set_text(win.contentLabel[0], "Adjust the volume and brightness ");


    //设置slider1,slider2样式
    lv_obj_set_size(win.contentSlider[0], lv_obj_get_width(win.content) * 4 / 5, 20);
    lv_obj_align(win.contentSlider[0], LV_ALIGN_CENTER, 0, calc_offset_len(lv_obj_get_width(win.content), 20, 6, 3, 1));
    lv_obj_set_size(win.contentSlider[1], lv_obj_get_width(win.content) * 4 / 5, 20);
    lv_obj_align(win.contentSlider[1], LV_ALIGN_CENTER, 0, calc_offset_len(lv_obj_get_width(win.content), 20, 6, 4, 1));


    //-------------------------------part3优化样式-------------------------------
    //设置窗口边框,边框颜色，圆角
    lv_obj_set_style_border_width(win.obj, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(win.obj, lv_color_make(116, 125, 136), LV_PART_MAIN);
    lv_obj_set_style_border_opa(win.obj, 50, LV_PART_MAIN);
    lv_obj_set_style_radius(win.obj, 10, LV_PART_MAIN);



    //设置按钮样式，简约一些
    lv_obj_set_style_shadow_opa(win.btn, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(win.btn, 0, LV_PART_MAIN);
    lv_obj_set_style_text_color(win.btn, lv_color_make(31, 31, 31), LV_PART_MAIN);

    //移除滚动条
    lv_obj_remove_style(win.content, NULL, LV_PART_SCROLLBAR);


    //-------------------------------part4添加功能------------------------------
    //给按钮添加关闭功能

    lv_obj_add_event_cb(win.btn, test_cb, LV_EVENT_CLICKED, (void*)&userEventCode[0]);
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

    ////创建窗口
    //lv_obj_t* obj = lv_win_create(lv_scr_act());
    ////设置窗口大小，居中位置，边框
    //lv_obj_set_size(obj, scr_act_width() * 2 / 3, scr_act_height() / 3);
    //lv_obj_center(obj);
    //lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN);
    //lv_obj_update_layout(obj);
    ////获取头部对象并设置头部宽度
    //lv_obj_t* winHeader = lv_win_get_header(obj);
    //lv_obj_set_height(winHeader, 20);

    ////添加标题，按钮（标题顺序按照代码先后，从头部左边开始添加）
    //lv_win_add_title(obj, LV_SYMBOL_SETTINGS);
    //lv_win_add_title(obj, "setting");
    //lv_win_add_button(obj, LV_SYMBOL_CLOSE, 20);

    ////获取主体部分对象
    //lv_obj_t* winCont = lv_win_get_content(obj);
    ////创建label，slider并设置样式
    //lv_obj_t* label = lv_label_create(winCont);
    //lv_obj_center(label);
    //lv_obj_t* slider = lv_slider_create(winCont);
    //lv_obj_set_size(slider, 50, 20);
    //lv_obj_align_to(slider, label, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);


    show_win();
    return;
}





//均匀排列元素,返回计算出的当前元素在base中均匀排列，需要居中或靠边对齐后偏移的位置
//各子元素的大小应一致，尽量用居中的方式，因为靠边的方式，有时候lv提供的尺寸不含边框了，不准确
//参数baseLenOneSide：需要均匀排列的一边父对象的边长
//参数ElemLenOneSide：子元素的边长
//参数NumOfElem：子元素的个数
//参数elemId：子元素的排序，从1开始
//参数alignType：0：靠边；1：居中；  （元素以哪种对齐方式来计算？可以先居中来算，也可以靠边来算）
static int16_t calc_offset_len(uint16_t baseLenOneSide, uint16_t ElemLenOneSide, uint8_t NumOfElem, uint8_t elemId, uint8_t alignType)
{
    int16_t OffsetLen = 0;
    uint16_t elemIdMedian = 0;
    //需特殊处理的情况
    if ((elemId == 0) | (NumOfElem < 2) | ((ElemLenOneSide * NumOfElem) > baseLenOneSide))
    {
        return OffsetLen;
    }
    //计算每个空隙的大小
    uint16_t intervalSize = (baseLenOneSide - (ElemLenOneSide * NumOfElem)) / (NumOfElem + 1);
    //靠边对齐方式计算
    if (alignType == 0)
    {
        OffsetLen = (intervalSize * elemId) + ((elemId - 1) * ElemLenOneSide);
    }
    //居中对齐方式来计算
    else if (alignType == 1)
    {
        //元素数量为单数的情况
        if (NumOfElem % 2)
        {
            //计算应该center的元素ID
            elemIdMedian = (NumOfElem / 2) + 1;
            if (elemIdMedian == elemId)//此元素应该居中显示
            {
                return OffsetLen;
            }
            //应向下偏移
            else if (elemId > elemIdMedian)
            {
                OffsetLen = (elemId - elemIdMedian) * (intervalSize + ElemLenOneSide);
            }
            //应向上偏移
            else
            {
                OffsetLen = -((elemIdMedian - elemId) * (intervalSize + ElemLenOneSide));
            }
        }
        //元素数量为双数
        else
        {
            elemIdMedian = NumOfElem / 2;
            //应向下偏移
            if (elemId > elemIdMedian)
            {
                //就在中位线下边一个
                if (elemId - elemIdMedian == 1)
                {
                    OffsetLen = (intervalSize + ElemLenOneSide) / 2;
                }
                else
                {
                    OffsetLen = ((elemId - elemIdMedian - 1) * (intervalSize + ElemLenOneSide)) + ((intervalSize + ElemLenOneSide) / 2);
                }
            }
            //应向上偏移
            else
            {
                //就在中位线向上一个
                if (elemIdMedian == elemId)
                {
                    OffsetLen = -((intervalSize + ElemLenOneSide) / 2);
                }
                else
                {
                    OffsetLen = -((elemIdMedian - elemId) * (intervalSize + ElemLenOneSide) + ((intervalSize + ElemLenOneSide) / 2));
                }
            }
        }
    }

    return OffsetLen;
}
