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
#define BUFFSIZE 11
//为了兼容8，9版本函数
#if (LVGL_VERSION_MAJOR == 8)

#endif

//测试代码
//const char* btnMatrixMap[] = {"1", "\n", "2", "3", ""};

LV_IMG_DECLARE(img_userLogo);
const lv_font_t* g_font;
lv_obj_t* g_passwordLabel;
lv_obj_t* g_tipLabel;


#if !USE_FREERTOS
static void timer_cb(lv_timer_t* timer)
{

}
#endif

static void test_cb(lv_event_t* event)
{
    //测试代码
    //lv_obj_t* target = lv_event_get_target(event);
    //lv_event_code_t eventCode = lv_event_get_code(event);
    //if (eventCode == LV_EVENT_CLICKED)
    //{
    //    printf("%d %s\r\n", lv_btnmatrix_get_selected_btn(target), lv_btnmatrix_get_btn_text(target, lv_btnmatrix_get_selected_btn(target)));
    //}

    static char passwordBuf[BUFFSIZE] = { 0 };
    char tempbuf[BUFFSIZE] = { 0 };
    lv_obj_t* target = lv_event_get_target(event);
    lv_event_code_t eventCode = lv_event_get_code(event);
    if (eventCode == LV_EVENT_CLICKED)
    {
        //打印按钮序号，按钮TEXT
        printf("%d %s\r\n", lv_btnmatrix_get_selected_btn(target), lv_btnmatrix_get_btn_text(target, lv_btnmatrix_get_selected_btn(target)));
        //判断大小大小能否装下
        strncpy(tempbuf, lv_btnmatrix_get_btn_text(target, lv_btnmatrix_get_selected_btn(target)), sizeof(tempbuf));
        uint8_t textLen = strlen(tempbuf);
        uint8_t bufLen = strlen(passwordBuf);

        //先判断特殊字符
        if (strstr(tempbuf, "C"))//判断是不是C
        {
            memset(passwordBuf, 0, sizeof(passwordBuf));
            lv_label_set_text_fmt(g_tipLabel, " ");
        }
        else if (strstr(tempbuf, "backspace"))//判断是不是退格
        {
            if (bufLen == 1)
            {
                memset(passwordBuf, 0, sizeof(passwordBuf));
            }
            else if (bufLen != 0)
            {
                passwordBuf[bufLen - 1] = '\0';//最后一个字符改为字符串结束符号
                passwordBuf[bufLen] = 0;//'\0'清除
            }
            //buflen = 0，如果按了退格， 不处理
            lv_label_set_text_fmt(g_tipLabel, " ");
        }
        //没有特殊字符
        else if (textLen + bufLen <= BUFFSIZE - 1)//buffer能装下str
        {
            strcat(passwordBuf, tempbuf);
        }
        else
        {
            //提示写满了
            lv_label_set_text_fmt(g_tipLabel, "You can enter up to 8 numbers");
        }
        lv_label_set_text_fmt(g_passwordLabel, "%s", passwordBuf);
    }
}

void show_user_ui(void)
{
    //用户头像logo创建
    lv_obj_t* userLogo = lv_img_create(lv_scr_act());
    lv_img_set_src(userLogo, &img_userLogo);
    lv_obj_align(userLogo, LV_ALIGN_CENTER, 0, -scr_act_height() / 3);
    lv_img_set_zoom(userLogo, 128);
    lv_obj_set_style_img_recolor_opa(userLogo, 255, LV_PART_MAIN);
    lv_obj_set_style_img_recolor(userLogo, lv_color_make(100, 100, 100), LV_PART_MAIN);

    //用户密码输入框base
    lv_obj_update_layout(userLogo);
    lv_obj_t* baseObj = lv_obj_create(lv_scr_act());
    lv_obj_set_size(baseObj, scr_act_width() / 3, scr_act_width() / 9);
    lv_obj_align_to(baseObj, userLogo, LV_ALIGN_OUT_BOTTOM_MID, 0, -scr_act_height() / 12);
    lv_obj_remove_style(baseObj, NULL, LV_PART_SCROLLBAR);
    lv_obj_set_style_bg_color(baseObj, lv_color_make(100, 100, 100), LV_PART_MAIN);
    //设置边缘为圆角
    lv_obj_set_style_radius(baseObj, 20, LV_PART_MAIN);
    //设置边框宽度为0
    lv_obj_set_style_border_width(baseObj, 0, LV_PART_MAIN);

    //用户密码输入框label
    g_passwordLabel = lv_label_create(baseObj);
    lv_obj_align(g_passwordLabel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_font(g_passwordLabel, g_font, LV_PART_MAIN);
    lv_label_set_text(g_passwordLabel, " ");
    lv_label_set_long_mode(g_passwordLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_style_text_align(g_passwordLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

    //提示信息label
    g_tipLabel = lv_label_create(lv_scr_act());
    lv_obj_align_to(g_tipLabel, baseObj, LV_ALIGN_OUT_RIGHT_MID, scr_act_width()/24, 0);
    lv_obj_set_width(g_tipLabel, scr_act_width()/4);
    lv_label_set_long_mode(g_tipLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(g_tipLabel, " ");

}


const char* passwordBtnMap[] = { "1","2","3","\n","4","5","6","\n","7","8","9","\n","C","0","backspace" , ""};
void show_btnmatrix(void)
{
   lv_obj_t* passwordBtn = lv_btnmatrix_create(lv_scr_act());
   lv_btnmatrix_set_map(passwordBtn, passwordBtnMap);
   lv_obj_set_size(passwordBtn, scr_act_width(), scr_act_height() * 3 / 5);
   lv_obj_align(passwordBtn, LV_ALIGN_BOTTOM_MID, 0, 0);

   //按钮矩阵样式优化，进保留文字
   //设置边框宽度
   lv_obj_set_style_border_width(passwordBtn, 0, LV_PART_MAIN);
   //设置背景和按钮背景的透明度
   lv_obj_set_style_bg_opa(passwordBtn, 0, LV_PART_MAIN);
   lv_obj_set_style_bg_opa(passwordBtn, 0, LV_PART_ITEMS);
   //设置按钮阴影宽度
   lv_obj_set_style_shadow_width(passwordBtn, 0, LV_PART_ITEMS);
   lv_obj_add_event_cb(passwordBtn, test_cb, LV_EVENT_CLICKED, NULL);

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
    ////创建并设置按钮矩阵的文本，准备一个const char* map[]，换行用"\n"，且最后一个元素要用一个空元素""
    //lv_obj_t* obj = lv_btnmatrix_create(lv_scr_act());
    //lv_btnmatrix_set_map(obj, btnMatrixMap);

    ////设置按钮的相对宽度，默认是1
    //lv_btnmatrix_set_btn_width(obj, 1, 2);

    ////设置，清除单个和全部按钮属性，单个属性中按钮序号从0开始
    //lv_btnmatrix_set_btn_ctrl(obj, 0, LV_BTNMATRIX_CTRL_DISABLED);
    //lv_btnmatrix_clear_btn_ctrl(obj, 0, LV_BTNMATRIX_CTRL_DISABLED);
    //lv_btnmatrix_set_btn_ctrl_all(obj, LV_BTNMATRIX_CTRL_HIDDEN);
    //lv_btnmatrix_clear_btn_ctrl_all(obj, LV_BTNMATRIX_CTRL_HIDDEN);
    ////得找时间研究下v9下如何对按钮的文字重新着色

    ////设置按钮可以被切换，以及按钮单词只能选择一个的属性。这俩得配合使用
    //lv_btnmatrix_set_btn_ctrl_all(obj, LV_BTNMATRIX_CTRL_CHECKABLE);
    //lv_btnmatrix_set_one_checked(obj,true);
    //lv_obj_add_event_cb(obj, test_cb, LV_EVENT_CLICKED, NULL);

    show_user_ui();
    show_btnmatrix();
    return;
}
