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
#if (LVGL_VERSION_MAJOR == 8)
#define lv_imagebutton_create(base)			 lv_imgbtn_create(base)
#define lv_imagebutton_set_src(x,y,z,a,b) 	 lv_imgbtn_set_src(x,y,z,a,b)
#define LV_IMAGEBUTTON_STATE_RELEASED		 LV_IMGBTN_STATE_RELEASED
#endif

#define IMGBTNNUM 3


LV_IMG_DECLARE(img_arefaction);
LV_IMG_DECLARE(img_cool);
LV_IMG_DECLARE(img_hot);


const lv_font_t* g_font;

#if !USE_FREERTOS
static void timer_cb(lv_timer_t* timer)
{

}
#endif


//均匀排列元素,返回计算出的当前元素在base中均匀排列，需要center后偏移的位置
//各子元素的大小应一致
//参数baseLenOneSide：需要均匀排列的一边父对象的边长
//参数ElemLenOneSide：子元素的边长
//参数NumOfElem：子元素的个数
//参数elemId：子元素的排序，从1开始
static int16_t calcOffsetLen(uint16_t baseLenOneSide, uint16_t ElemLenOneSide, uint8_t NumOfElem, uint8_t elemId)
{
    int16_t offsetY = 0;
    uint16_t elemIdMedian = 0;
    //需特殊处理的情况
    if (elemId == 0 | ElemLenOneSide * NumOfElem > baseLenOneSide)
    {
        return offsetY;
    }
    //计算每个空隙的大小
    uint16_t intervalSize = (baseLenOneSide - (ElemLenOneSide * NumOfElem)) / (NumOfElem + 1);

    //元素数量为单数的情况
    if (NumOfElem % 2)
    {
        //计算应该center的元素ID
        elemIdMedian = (NumOfElem / 2) + 1;
        if (elemIdMedian == elemId)//此元素应该居中显示
        {
            return offsetY;
        }
        //应向下偏移
        else if (elemId > elemIdMedian)
        {
            offsetY = (elemId - elemIdMedian) * (intervalSize + ElemLenOneSide);
        }
        //应向上偏移
        else
        {
            offsetY = -((elemIdMedian - elemId) * (intervalSize + ElemLenOneSide));
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
                offsetY = (intervalSize + ElemLenOneSide) / 2;
            }
            else
            {
                offsetY = ((elemId - elemIdMedian - 1) * (intervalSize + ElemLenOneSide)) + ((intervalSize + ElemLenOneSide) / 2);
            }
        }
        //应向上偏移
        else
        {
            //就在中位线向上一个
            if (elemIdMedian == elemId)
            {
                offsetY = -((intervalSize + ElemLenOneSide) / 2);
            }
            else
            {
                offsetY = -((elemIdMedian - elemId) * (intervalSize + ElemLenOneSide) + ((intervalSize + ElemLenOneSide) / 2));
            }
        }
    }
    return offsetY;
}

static void test_cb(lv_event_t* event)
{
    static uint8_t swtichFlag1 = 0;
    static uint8_t swtichFlag2 = 0;
    static uint8_t swtichFlag3 = 0;
    lv_obj_t* target = lv_event_get_target(event);
    uint8_t* tempStr = event->user_data;
    switch (*tempStr)
    {
    case 1:
    {
        if (!swtichFlag1)
        {
            swtichFlag1 = 1;
            lv_obj_set_style_img_recolor(target, lv_color_make(85, 170, 255), LV_PART_MAIN);
        }
        else
        {
            swtichFlag1 = 0;
            lv_obj_set_style_img_recolor(target, lv_color_hex(0x000000), LV_PART_MAIN);
        }
        break;
    }
    case 2:
    {
        if (!swtichFlag2)
        {
            swtichFlag2 = 1;
            lv_obj_set_style_img_recolor(target, lv_color_make(250, 89, 136), LV_PART_MAIN);
        }
        else
        {
            swtichFlag2 = 0;
            lv_obj_set_style_img_recolor(target, lv_color_hex(0x000000), LV_PART_MAIN);
        }
        break;
    }
    case 3:
    {
        if (!swtichFlag3)
        {
            swtichFlag3 = 1;
            lv_obj_set_style_img_recolor(target, lv_color_make(174, 118, 49), LV_PART_MAIN);
        }
        else
        {
            swtichFlag3 = 0;
            lv_obj_set_style_img_recolor(target, lv_color_hex(0x000000), LV_PART_MAIN);
        }
        break;
    }
    default:break;
    }
    return;
}

void my_label_create(lv_obj_t* alignBase, const char* text)
{
    lv_obj_update_layout(alignBase);
    lv_obj_t* label = lv_label_create(lv_scr_act());
    lv_obj_set_width(label, lv_obj_get_width(alignBase));
    lv_obj_align_to(label, alignBase, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_style_text_font(label, g_font, LV_PART_MAIN);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(label, text);
}

static uint8_t uesrDate[3] = { 1, 2, 3};
void show_imgbtn()
{
    uint16_t imgElemLenOneSide = img_cool.header.h;

    //创建基础对象
    lv_obj_t* base = lv_obj_create(lv_scr_act());
    lv_obj_set_size(base, scr_act_width() / 2, scr_act_height() * 19 / 20);
    lv_obj_center(base);
    lv_obj_update_layout(base);

    //创建制冷图标
    lv_obj_t* coolBtn = lv_imagebutton_create(base);
    lv_obj_set_size(coolBtn, img_cool.header.w, img_cool.header.h);
    lv_obj_align(coolBtn, LV_ALIGN_CENTER, 0, calcOffsetLen(lv_obj_get_height(base), imgElemLenOneSide, IMGBTNNUM, 1));
    //给图片按钮部件添加图片源，可以添加左中右三个图片
    lv_imagebutton_set_src(coolBtn, LV_IMAGEBUTTON_STATE_RELEASED, NULL, &img_cool, NULL);
    //原图不够黑，重新着色为黑色
    lv_obj_set_style_img_recolor_opa(coolBtn, 255, LV_PART_MAIN);
    lv_obj_set_style_img_recolor(coolBtn, lv_color_hex(0x000000), LV_PART_MAIN);
    my_label_create(coolBtn, "COOL");
    lv_obj_add_event_cb(coolBtn, test_cb, LV_EVENT_CLICKED, &uesrDate[0]);

    //创建制热图标
    lv_obj_t* hotBtn = lv_imagebutton_create(base);
    lv_obj_set_size(hotBtn, img_hot.header.w, img_hot.header.h);
    lv_obj_align(hotBtn, LV_ALIGN_CENTER, 0, calcOffsetLen(lv_obj_get_height(base), imgElemLenOneSide, IMGBTNNUM, 2));
    //给图片按钮部件添加图片源，可以添加左中右三个图片
    lv_imagebutton_set_src(hotBtn, LV_IMAGEBUTTON_STATE_RELEASED, NULL, &img_hot, NULL);
    //原图不够黑，重新着色为黑色
    lv_obj_set_style_img_recolor_opa(hotBtn, 255, LV_PART_MAIN);
    lv_obj_set_style_img_recolor(hotBtn, lv_color_hex(0x000000), LV_PART_MAIN);
    my_label_create(hotBtn, "HOT");
    lv_obj_add_event_cb(hotBtn, test_cb, LV_EVENT_CLICKED, &uesrDate[1]);

    //创建除湿图标
    lv_obj_t* arefactionBtn = lv_imagebutton_create(base);
    lv_obj_set_size(arefactionBtn, img_arefaction.header.w, img_arefaction.header.h);
    lv_obj_align(arefactionBtn, LV_ALIGN_CENTER, 0, calcOffsetLen(lv_obj_get_height(base), imgElemLenOneSide, IMGBTNNUM, 3));
    //给图片按钮部件添加图片源，可以添加左中右三个图片
    lv_imagebutton_set_src(arefactionBtn, LV_IMAGEBUTTON_STATE_RELEASED, NULL, &img_arefaction, NULL);
    //原图不够黑，重新着色为黑色
    lv_obj_set_style_img_recolor_opa(arefactionBtn, 255, LV_PART_MAIN);
    lv_obj_set_style_img_recolor(arefactionBtn, lv_color_hex(0x000000), LV_PART_MAIN);
    my_label_create(arefactionBtn, "arefaction");
    lv_obj_add_event_cb(arefactionBtn, test_cb, LV_EVENT_CLICKED, &uesrDate[2]);
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
    //lv_obj_t* obj = lv_imagebutton_create(lv_scr_act());
    //lv_obj_set_size(obj, img_cool.header.w, img_cool.header.h);
    ////给图片按钮部件添加图片源，可以添加左中右三个图片
    //lv_imagebutton_set_src(obj, LV_IMAGEBUTTON_STATE_RELEASED, NULL, &img_cool, NULL);
    ////设置图片按钮状态
    //lv_imagebutton_set_state(obj, LV_IMAGEBUTTON_STATE_PRESSED);


    // 这个需要试一下
    //lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);

    show_imgbtn();

    return;
}
