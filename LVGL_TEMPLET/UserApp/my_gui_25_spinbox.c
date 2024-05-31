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

//为了兼容8，9版本函数
#if (LVGL_VERSION_MAJOR == 9)
#define lv_spinbox_set_pos(x, y)     lv_spinbox_set_cursor_pos(x, y)
#endif
#define scr_act_height() lv_obj_get_height(lv_scr_act())
#define scr_act_width() lv_obj_get_width(lv_scr_act())


static int16_t calc_offset_len(uint16_t baseLenOneSide, uint16_t ElemLenOneSide, uint8_t NumOfElem, uint8_t elemId, uint8_t alignType);

const lv_font_t* g_font;
lv_obj_t* g_spinbox;

//减法回调，显示当前数值
static void subBtn_cb(lv_event_t* e)
{
    lv_spinbox_decrement(g_spinbox);
    printf(" %0.1f\n", (float)lv_spinbox_get_value(g_spinbox) / 10);
}

//加法回调，显示当前数值
static void addBtn_cb(lv_event_t* e)
{
    lv_spinbox_increment(g_spinbox);
    printf(" %0.1f\n", (float)lv_spinbox_get_value(g_spinbox) /10);
}


void show_spinbox(void)
{
    //微调器因为小数点只是装饰品，所以先确定小数是几位的，然后其他数值全都响应乘以倍数，组后好计算

    //创建
    g_spinbox = lv_spinbox_create(lv_scr_act());
    lv_obj_center(g_spinbox);
    // 设置光标为2，也就是步进值为100
    lv_spinbox_set_pos(g_spinbox, 2);
    // 设置当前值
    lv_spinbox_set_value(g_spinbox, 0);
    //设置微调器的范围值格式
    lv_spinbox_set_range(g_spinbox, -10000, 10000);
    //设置显示的数字格式（显示多少位）与小数点的位置，注意，小数点只是装饰，位置是从前面数的
    lv_spinbox_set_digit_format(g_spinbox, 5, 4);
    lv_obj_update_layout(g_spinbox);


    //创建减法按钮与符号label
    lv_obj_t* subBtn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(subBtn, 30, lv_obj_get_height(g_spinbox));
    lv_obj_t* btnLabel = lv_label_create(subBtn);
    lv_obj_set_style_text_font(btnLabel, g_font, LV_PART_MAIN);
    lv_obj_center(btnLabel);
    lv_label_set_text(btnLabel, LV_SYMBOL_MINUS);
    lv_obj_align_to(subBtn, g_spinbox, LV_ALIGN_OUT_LEFT_BOTTOM, -5, 0);

    //创建加法按钮与符号label
    lv_obj_t* addBtn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(addBtn, 30, lv_obj_get_height(g_spinbox));
    btnLabel = lv_label_create(addBtn);
    lv_obj_set_style_text_font(btnLabel, g_font, LV_PART_MAIN);
    lv_obj_center(btnLabel);
    lv_label_set_text(btnLabel, LV_SYMBOL_PLUS);
    lv_obj_align_to(addBtn, g_spinbox, LV_ALIGN_OUT_RIGHT_BOTTOM, 5, 0);

    //添加事件
    lv_obj_add_event_cb(subBtn, subBtn_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(addBtn, addBtn_cb, LV_EVENT_CLICKED, NULL);

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
    //g_spinbox = lv_spinbox_create(lv_scr_act());
    //lv_obj_center(g_spinbox);
    ////设置步进数值，此函数与设置光标冲突，二选一
    ////lv_spinbox_set_step(g_spinbox, 200);
    //// 设置光标，从右边，0开始，实际也是设置步进值，此函数与设置步进值冲突，二选一。
    //lv_spinbox_set_pos(g_spinbox, 2);
    //// 设置当前值
    //lv_spinbox_set_value(g_spinbox, 500);
    ////设置微调器的范围值格式
    //lv_spinbox_set_range(g_spinbox, -10000, 10000);
    ////设置显示的数字格式（显示多少位）与小数点的位置，注意，小数点只是装饰，位置是从前面数的
    //lv_spinbox_set_digit_format(g_spinbox, 6,5);

    //lv_obj_t* subBtn = lv_btn_create(lv_scr_act());
    //lv_obj_align_to(subBtn, g_spinbox, LV_ALIGN_OUT_LEFT_BOTTOM, 0, 0);
    //lv_obj_t* addBtn = lv_btn_create(lv_scr_act());
    //lv_obj_align_to(addBtn, g_spinbox, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
    //
    //lv_obj_add_event_cb(subBtn, subBtn_cb, LV_EVENT_CLICKED, NULL);
    //lv_obj_add_event_cb(addBtn, addBtn_cb, LV_EVENT_CLICKED, NULL);


    show_spinbox();

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
