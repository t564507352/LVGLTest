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

//为了兼容8，9版本函数
#if (LVGL_VERSION_MAJOR == 9)
#define lv_tabview_create(x, y, z)    lv_tabview_create(x);lv_tabview_set_tab_bar_size(tv.thisTabview, z)
#endif

#define TABMAX 3
#define TABLABMAX 6
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

//所有选项卡要用到的零件
typedef struct
{
    lv_obj_t* base;
    lv_obj_t* thisTabview;
    lv_obj_t* tab[TABMAX];
    lv_obj_t* label[TABLABMAX];
    lv_obj_t* btns;
    lv_obj_t* content;
}_tabviewAttr;

typedef enum {
    CONTROL = 0,
    MESSAGE,
} __tvTab;

typedef enum {
    CONTROL_OPTION = 0,
    CONTROL_TIPS,
    MESSAGE_CONTENT,
} __tvTabLabel;


void show_tabview(void)
{
    //初始化选项卡零件结构体
    _tabviewAttr tv;
    tv.base = lv_scr_act();
    //创建选项卡，设置字体
    tv.thisTabview = lv_tabview_create(tv.base, LV_DIR_TOP, scr_act_height() / 10);
    lv_obj_set_style_text_font(tv.thisTabview, g_font, LV_PART_MAIN);

    //添加tab1
    tv.tab[CONTROL] = lv_tabview_add_tab(tv.thisTabview, "control center");
    //创建tab1的label1
    tv.label[CONTROL_OPTION] = lv_label_create(tv.tab[CONTROL]);
    lv_obj_align(tv.label[CONTROL_OPTION], LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text_fmt(tv.label[CONTROL_OPTION], "Here you put the control options button");
	//创建tab1的label2
    tv.label[CONTROL_TIPS] = lv_label_create(tv.tab[CONTROL]);
    lv_obj_align(tv.label[CONTROL_TIPS], LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_label_set_text(tv.label[CONTROL_TIPS], "Here you put the control tips");
    //隐藏tab1滚动条
    lv_obj_remove_style(tv.tab[CONTROL], NULL, LV_PART_SCROLLBAR);

    //添加tab2
    tv.tab[MESSAGE] = lv_tabview_add_tab(tv.thisTabview, "message center");
    //创建tab2的label
    tv.label[MESSAGE_CONTENT] = lv_label_create(tv.tab[MESSAGE]);
    lv_obj_align(tv.label[MESSAGE_CONTENT], LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(tv.label[MESSAGE_CONTENT], "This is where reminder messages are placed");
    //隐藏tab2滚动条
    lv_obj_remove_style(tv.tab[MESSAGE], NULL, LV_PART_SCROLLBAR);

    //设置选项卡按钮的样式
    //先获取btns区域的对象
    tv.btns = lv_tabview_get_tab_btns(tv.thisTabview);
    //设置默认状态的背景以及字体颜色
    lv_obj_set_style_bg_color(tv.btns, lv_color_hex(0xb7472a), LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(tv.btns, 200, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(tv.btns, lv_color_hex(0xf3f3f3), LV_PART_ITEMS | LV_STATE_DEFAULT);

    //设置选中状态的背景以及字体颜色
    lv_obj_set_style_bg_color(tv.btns, lv_color_hex(0xe1e1e1), LV_PART_ITEMS | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(tv.btns, 200, LV_PART_ITEMS | LV_STATE_CHECKED);
    lv_obj_set_style_text_color(tv.btns, lv_color_hex(0xb7472a), LV_PART_ITEMS | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(tv.btns, 0, LV_PART_ITEMS | LV_STATE_CHECKED);

    //获取容器对象
    tv.content = lv_tabview_get_content(tv.thisTabview);
    //设置容器区域的样式
    lv_obj_set_style_bg_color(tv.content, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(tv.content, 255, LV_STATE_DEFAULT);
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
    //lv_obj_t* obj = lv_tabview_create(lv_scr_act());
    ////设施选项卡上面的标题栏的宽度
    //lv_tabview_set_tab_bar_size(obj, scr_act_height() / 10);
    ////为选项卡添加tab
    //lv_obj_t* objTab1 = lv_tabview_add_tab(obj, "msg");
    //lv_obj_t* objTab2 = lv_tabview_add_tab(obj, "date");
    ////设置默认选中的tab
    //lv_tabview_set_act(obj, 1, LV_ANIM_ON);
    ////以tab1为容器创建label
    //lv_obj_t* objTab1Label = lv_label_create(objTab1);
    ////获取tabview中的按钮部分的对象
    //lv_obj_t* tempBtns = lv_tabview_get_tab_btns(obj);


    show_tabview();

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
