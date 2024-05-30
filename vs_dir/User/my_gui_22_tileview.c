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
#define TILEVIEW_TILEMAX 4
#define TILEVIEW_LABELMAX 8

//为了兼容8，9版本函数
#if (LVGL_VERSION_MAJOR == 9)

#endif


static int16_t calc_offset_len(uint16_t baseLenOneSide, uint16_t ElemLenOneSide, uint8_t NumOfElem, uint8_t elemId, uint8_t alignType);

const lv_font_t* g_font;

#if !USE_FREERTOS
static void timer_cb(lv_timer_t* timer)
{

}
#endif


static void test_cb(lv_event_t* event)
{

}

typedef struct
{
    lv_obj_t* thisTilev;
    lv_obj_t* tiles[TILEVIEW_TILEMAX];
    lv_obj_t* label[TILEVIEW_LABELMAX];
}_tileviewAttr;

void show_tileview(void)
{
    _tileviewAttr tilev;
    //创建平铺视图并添加视图
    tilev.thisTilev = lv_tileview_create(lv_scr_act());
    tilev.tiles[0] = lv_tileview_add_tile(tilev.thisTilev, 0, 0, LV_DIR_RIGHT);
    tilev.tiles[1] = lv_tileview_add_tile(tilev.thisTilev, 1, 0, LV_DIR_HOR);
    tilev.tiles[2] = lv_tileview_add_tile(tilev.thisTilev, 2, 0, LV_DIR_LEFT);
    //移除滚动条美化外观
    lv_obj_remove_style(tilev.thisTilev, NULL, LV_PART_SCROLLBAR);
    //更新布局并设置默认视图
    lv_obj_update_layout(tilev.thisTilev);
    lv_obj_set_tile_id(tilev.thisTilev, 1, 0, LV_ANIM_ON);

    //添加各视图的label
    tilev.label[0] = lv_label_create(tilev.tiles[0]);
    tilev.label[1] = lv_label_create(tilev.tiles[1]);
    tilev.label[2] = lv_label_create(tilev.tiles[2]);
    //设置label
    lv_obj_set_style_text_font(tilev.label[0], g_font, LV_PART_MAIN);
    lv_label_set_text(tilev.label[0], "                 view1               =>");
    lv_obj_center(tilev.label[0]);
    lv_obj_set_style_text_font(tilev.label[1], g_font, LV_PART_MAIN);
    lv_label_set_text(tilev.label[1], "<=               view2               =>");
    lv_obj_center(tilev.label[1]);
    lv_obj_set_style_text_font(tilev.label[2], g_font, LV_PART_MAIN);
    lv_label_set_text(tilev.label[2], "<=               view3                 ");
    lv_obj_center(tilev.label[2]);



    //创建左上角，右上角的图标
    lv_obj_t* label = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(label, g_font, LV_PART_MAIN);
    lv_label_set_text_fmt(label, "%s%s", LV_SYMBOL_HOME, " 8:30 A.M");
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 0, 0);

    label = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(label, g_font, LV_PART_MAIN);
    lv_label_set_text_fmt(label, "%s %s", LV_SYMBOL_WIFI, LV_SYMBOL_BATTERY_3);
    lv_obj_align(label, LV_ALIGN_TOP_RIGHT, 0, 0);


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
    //lv_obj_t* tilev = lv_tileview_create(lv_scr_act());
    ////添加一个tile，返回tile视图对象。
    ////参数为base，列，行，可以往哪个方向滑动。其中列，行参数可以理解为x轴，Y轴
    //lv_obj_t* tile1 = lv_tileview_add_tile(tilev, 0, 0, LV_DIR_RIGHT);
    //lv_obj_t* tile2 = lv_tileview_add_tile(tilev, 1, 0, LV_DIR_LEFT | LV_DIR_BOTTOM);
    //lv_obj_t* tile3 = lv_tileview_add_tile(tilev, 1, 1, LV_DIR_TOP);

    ////平铺视图部件，如果要设置默认视图，需要先更新tileview的布局
    //lv_obj_update_layout(tilev);
    ////设置默认显示的页面，以下两个函数都可以，一个是通过对象设置，一个是通过位置设置
    //lv_obj_set_tile(tilev, tile2, LV_ANIM_ON);
    ////lv_obj_set_tile_id(tilev, 1, 1, LV_ANIM_ON);


    show_tileview();
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
