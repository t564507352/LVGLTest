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

#define MSGBOX_FOOTER_BTN_MAX      3

//为了兼容8，9版本函数
#if (LVGL_VERSION_MAJOR == 9)

#endif


static int16_t calc_offset_len(uint16_t baseLenOneSide, uint16_t ElemLenOneSide, uint8_t NumOfElem, uint8_t elemId, uint8_t alignType);

const lv_font_t* g_font;
typedef struct
{
    lv_obj_t* obj;
    lv_obj_t* header;
    lv_obj_t* title;
    lv_obj_t* titleBtn;
    lv_obj_t* footerBtn[MSGBOX_FOOTER_BTN_MAX];
    lv_obj_t* content;
    lv_obj_t* contentLabel;
}_widgets_msgbox;


_widgets_msgbox msgbox;

#if !USE_FREERTOS
static void timer_cb(lv_timer_t* timer)
{

}
#endif


//static void test_cb(lv_event_t* event)
//{
//
//}

//msgbox部件在8版本和9版本中有较大不同：
//1：创建函数不同，8版本中需要将部件属性一同写进去，9则分开填
//2：页尾的按钮，8版本中是按钮矩阵，9中好像是一个一个的按钮，需要分别创建
//3：添加事件回调的方式不同：8版本中要对msgbox的对象添加回调函数，event为LV_EVENT_VALUE_CHANGED，而9版本中则只能对单独的按钮添加回调
//4：事件回调中取值方式不同，8版本中先用lv_event_get_current_target()取obj，再lv_msgbox_get_active_btn获取按钮编号； 9版本则需按常规的按钮来处理


static void msgbox_cb(lv_event_t* e)
{
#if (LVGL_VERSION_MAJOR == 9)
    lv_obj_t* target = lv_event_get_target(e);
    lv_obj_add_flag(target->parent->parent, LV_OBJ_FLAG_HIDDEN);
#else
    lv_obj_t* target = lv_event_get_current_target(e);
    if (lv_msgbox_get_active_btn(target) == 2)
    {
        lv_obj_add_flag(msgbox.obj, LV_OBJ_FLAG_HIDDEN);
    }
#endif
}


static void slider_cb(lv_event_t* e)
{
    lv_obj_t* target = lv_event_get_target(e);
    uint32_t value = lv_slider_get_value(target);
    if (value >= 80)
    {
#if (LVGL_VERSION_MAJOR == 9)
        lv_obj_remove_flag(msgbox.obj, LV_OBJ_FLAG_HIDDEN);
#else
        lv_obj_clear_flag(msgbox.obj, LV_OBJ_FLAG_HIDDEN);
#endif
        lv_obj_remove_event_cb(target, slider_cb);
    }
}



void show_msgbox(void)
{
#if (LVGL_VERSION_MAJOR == 9)
    //-------------------初始化各零件--------------------------
    msgbox.obj = lv_msgbox_create(lv_scr_act());
    msgbox.title = lv_msgbox_add_title(msgbox.obj, LV_SYMBOL_WARNING"warning");
    msgbox.contentLabel = lv_msgbox_add_text(msgbox.obj, "Too much noise can damage your hearing. Shall we continue?");
    msgbox.footerBtn[0] = lv_msgbox_add_footer_button(msgbox.obj, "");
    msgbox.footerBtn[1] = lv_msgbox_add_footer_button(msgbox.obj, "");
    msgbox.footerBtn[2] = lv_msgbox_add_footer_button(msgbox.obj, "close");
    //调试看下header和title一样不？；content和contentLabel一样不
    msgbox.header = lv_msgbox_get_header(msgbox.obj);
    msgbox.content = lv_msgbox_get_content(msgbox.obj);
#else
    static const char* msgboxMap[] = { " "," ","close","" };
    msgbox.obj = lv_msgbox_create(lv_scr_act(), LV_SYMBOL_WARNING"warning", "Too much noise can damage your hearing. Shall we continue?", msgboxMap, false);
    msgbox.title = lv_msgbox_get_title(msgbox.obj);
    msgbox.contentLabel = lv_msgbox_get_text(msgbox.obj);
    msgbox.footerBtn[2] = lv_msgbox_get_btns(msgbox.obj);
    //调试看下header和title一样不？；content和contentLabel一样不
    msgbox.header = msgbox.title;
    msgbox.content = lv_msgbox_get_content(msgbox.obj);
#endif

    //-------------------设置各大小，位置属性---------------------
    //设置窗口大小，居中位置
//    lv_obj_set_size(msgbox.obj, scr_act_width() * 3 / 5, scr_act_height() * 5 / 18);
    lv_obj_center(msgbox.obj);
    lv_obj_update_layout(msgbox.obj);

    //设置头部高度
    lv_obj_set_height(msgbox.header, 40);


    //设置content的字体，移除滚动条
    lv_obj_set_style_text_font(msgbox.contentLabel, g_font, LV_PART_MAIN);
    lv_obj_remove_style(msgbox.content, NULL, LV_PART_SCROLLBAR);

    //-------------------obj和各零件样式优化---------------------
    //设置窗口边框,边框颜色，圆角
    lv_obj_set_style_border_width(msgbox.obj, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(msgbox.obj, lv_color_make(116, 125, 136), LV_PART_MAIN);
    lv_obj_set_style_border_opa(msgbox.obj, 50, LV_PART_MAIN);

    //设置标题颜色
    lv_obj_set_style_text_color(msgbox.header, lv_color_make(232, 9, 36), LV_PART_MAIN);
    lv_obj_set_style_bg_color(msgbox.header, lv_color_make(250, 250, 250), LV_PART_MAIN);
#if (LVGL_VERSION_MAJOR == 9)
    //设置按钮样式
    //隐藏前2个按钮
    lv_obj_set_style_shadow_opa(msgbox.footerBtn[0], 0, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(msgbox.footerBtn[0], 0, LV_PART_MAIN);
    lv_obj_set_style_shadow_opa(msgbox.footerBtn[1], 0, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(msgbox.footerBtn[1], 0, LV_PART_MAIN);
    //第三个按钮只留文字，颜色优化
    lv_obj_set_style_shadow_opa(msgbox.footerBtn[2], 0, LV_PART_MAIN);
    lv_obj_set_style_shadow_opa(msgbox.footerBtn[2], 0, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(msgbox.footerBtn[2], 0, LV_PART_MAIN);
    lv_obj_set_style_text_color(msgbox.footerBtn[2], lv_color_make(85, 170, 255), LV_PART_MAIN);
#else
    lv_obj_set_style_shadow_opa(msgbox.footerBtn[2], 0, LV_PART_ITEMS);
    lv_obj_set_style_shadow_opa(msgbox.footerBtn[2], 0, LV_PART_ITEMS);
    lv_obj_set_style_bg_opa(msgbox.footerBtn[2], 0, LV_PART_ITEMS);
    lv_obj_set_style_text_color(msgbox.footerBtn[2], lv_color_make(85, 170, 255), LV_PART_ITEMS);
    lv_obj_set_style_text_color(msgbox.footerBtn[2], lv_color_make(255, 0, 0), LV_PART_ITEMS | LV_STATE_PRESSED);

#endif
    //先隐藏
    lv_obj_add_flag(msgbox.obj, LV_OBJ_FLAG_HIDDEN);


    //-------------------创建事件---------------------
#if (LVGL_VERSION_MAJOR == 9) 
    lv_obj_add_event_cb(msgbox.footerBtn[2], msgbox_cb, LV_EVENT_CLICKED, NULL);
#else
    lv_obj_add_event_cb(msgbox.obj, msgbox_cb, LV_EVENT_VALUE_CHANGED, NULL);
#endif
}


void show_slider(void)
{
    lv_obj_t* slider = lv_slider_create(lv_scr_act());
    lv_slider_set_range(slider, 0, 100);
    lv_obj_set_width(slider, scr_act_width() * 4 / 5);
    lv_obj_center(slider);
    //创建slider事件
    lv_obj_add_event_cb(slider, slider_cb, LV_EVENT_VALUE_CHANGED, NULL);
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
    //lv_obj_t* obj = lv_msgbox_create(lv_scr_act());
    ////添加标题
    //lv_msgbox_add_title(obj, LV_SYMBOL_WARNING"warning");
    ////添加关闭按钮
    //lv_msgbox_add_close_button(obj);
    ////添加容器文本
    //lv_msgbox_add_text(obj,"test");
    ////添加页脚按钮
    //lv_msgbox_add_footer_button(obj,"close");

    show_slider();
    show_msgbox();

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
