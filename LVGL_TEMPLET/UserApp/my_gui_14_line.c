#include "my_gui.h"
#include "lvgl.h"
#include <stdio.h>
#include "math.h"
#if USE_FREERTOS
#include "lcd_function.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdlib.h>
#endif	

#define scr_act_height() lv_obj_get_height(lv_scr_act())
#define scr_act_width() lv_obj_get_width(lv_scr_act())

//为了兼容8，9版本函数
#if (LVGL_VERSION_MAJOR == 8)

#endif


const lv_font_t* g_font;

#define SIN_POINTS_NUM   200    
static lv_point_t sin_line_points[SIN_POINTS_NUM];              /* 正弦波坐标数组 */
static void create_sin_buf(uint16_t maxval, uint16_t samples)
{
    uint16_t i;
    float y = 0;

    /*
     * 正弦波最小正周期为2π，约等于2 * 3.1415926
     * 曲线上相邻的两个点在x轴上的间隔 = 2 * 3.1415926 / 采样点数量
     */
    float inc = (2 * 3.1415926) / samples;                      /* 计算相邻两个点的x轴间隔 */

    for (i = 0; i < samples; i++)                               /* 连续产生samples个点 */
    {
        /*
         * 正弦波函数解析式：y = Asin(wx + φ）+ b
         * 计算每个点的y值，将峰值放大maxval倍，并将曲线向上偏移maxval到正数区域
         */
        y = maxval * sin(inc * i) + maxval;

        sin_line_points[i].x = 2 * i;                           /* 存入x轴坐标 */
        sin_line_points[i].y = y;                               /* 存入y轴坐标 */
    }
}


#if !USE_FREERTOS
static void timer_cb(lv_timer_t* timer)
{

}
#endif

static void test_cb(lv_event_t* event)
{
    lv_obj_t* target = lv_event_get_target(event);

}

void draw_sin()
{
    //创建sin波数组
    create_sin_buf(scr_act_height() / 4, SIN_POINTS_NUM);
    lv_obj_t* sinLine = lv_line_create(lv_scr_act());
    lv_line_set_points(sinLine, sin_line_points, SIN_POINTS_NUM);
    lv_obj_align(sinLine, LV_ALIGN_CENTER, 0, 0);
    //设置线条宽度
    lv_obj_set_style_line_width(sinLine, 4, LV_PART_MAIN);
    //设置线条转弯处的圆角处理
    lv_obj_set_style_line_rounded(sinLine, true, LV_PART_MAIN);
    //设置线条颜色
    lv_obj_set_style_line_color(sinLine, lv_color_make(215, 124, 58), LV_PART_MAIN);
    //设置Y轴反转
    //lv_line_set_y_invert(sinLine, true);
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
    //lv_obj_t* obj = lv_line_create(lv_scr_act());
    ////创建坐标点
    //static lv_point_t pointBuf[] = { {0,0},{100,0},{50,50},{0,0} };
    //lv_line_set_points(obj, pointBuf, 4);
    //lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);
    ////设置线条宽度
    //lv_obj_set_style_line_width(obj, 10, LV_PART_MAIN);
    ////设置线条转弯处的圆角处理
    //lv_obj_set_style_line_rounded(obj, true, LV_PART_MAIN);
    //设置线条颜色
    //lv_obj_set_style_line_color(obj, lv_color_make(215, 124, 58), LV_PART_MAIN);
    ////设置Y轴反转
    //lv_line_set_y_invert(obj, true);



    lv_obj_t* label = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(label, g_font, LV_PART_MAIN);
    lv_label_set_text(label, "SIN");
    lv_obj_update_layout(label);
    lv_obj_t* textLine = lv_line_create(lv_scr_act());
    static lv_point_t textLineBuf[] = { {0,0},{100,0}};
    lv_line_set_points(textLine, textLineBuf, 2);
    lv_obj_align_to(textLine, label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);


    draw_sin();

    return;
}
