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

#endif

const lv_font_t* g_font;
lv_obj_t* g_kb;
lv_obj_t* g_passwordArea;

#if !USE_FREERTOS
static void timer_cb(lv_timer_t* timer)
{

}
#endif

static void test_cb(lv_event_t* event)
{
    lv_obj_t* target = lv_event_get_target(event);
    lv_event_code_t code = lv_event_get_code(event);
    lv_keyboard_set_textarea(g_kb, target);
    //获取文本区域的文本
    if (target == g_passwordArea && code == LV_EVENT_VALUE_CHANGED)
    {
        const char* passwordValue = lv_textarea_get_text(target);
        if (!strcmp(passwordValue, "123456"))
        {
            printf("succ,password = %s\r\n", passwordValue);
        }
    }  
}

void show_ta(void)
{
    //用户名区域
    lv_obj_t* userNameArea = lv_textarea_create(lv_scr_act());
    lv_obj_align(userNameArea, LV_ALIGN_CENTER, 0, -scr_act_height() / 6);
    //设置单行模式
    lv_textarea_set_one_line(userNameArea, true);
    //设置占位符
    lv_textarea_set_placeholder_text(userNameArea, "user name");
    //设置最大接收长度
    lv_textarea_set_max_length(userNameArea, 10);
    lv_obj_add_event_cb(userNameArea, test_cb, LV_EVENT_CLICKED, NULL);

    //密码区域
    g_passwordArea = lv_textarea_create(lv_scr_act());
    lv_obj_align(g_passwordArea, LV_ALIGN_CENTER, 0, -scr_act_height() / 12);
    //设置单行模式
    lv_textarea_set_one_line(g_passwordArea, true);
    //设置占位符
    lv_textarea_set_placeholder_text(g_passwordArea, "password");
    //设置接收的的文本内容,除此之外的文本输入没用
    lv_textarea_set_accepted_chars(g_passwordArea, "0123456789");
    //设置最大接收长度
    lv_textarea_set_max_length(g_passwordArea, 8);
    //设置输入密码模式，默认显示1.5s后隐藏
    lv_textarea_set_password_mode(g_passwordArea, true);
    //修改密码模式时密码显示时间单位ms
    lv_textarea_set_password_show_time(g_passwordArea, 500);
    lv_obj_add_event_cb(g_passwordArea, test_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(g_passwordArea, test_cb, LV_EVENT_VALUE_CHANGED, NULL);

    //创建键盘部件
    g_kb = lv_keyboard_create(lv_scr_act());
    //键盘默认与用户名区域连接
    lv_keyboard_set_textarea(g_kb, userNameArea);
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
    //lv_obj_t* obj = lv_textarea_create(lv_scr_act());
    ////lv_textarea_add_char(obj, 't');
    ////lv_textarea_add_text(obj, "abcde");
    //////设置光标位置，0为最左侧，LV_TEXTAREA_CURSOR_LAST为最右侧
    ////lv_textarea_set_cursor_pos(obj, 1);
    //////删除光标前面的字符
    ////lv_textarea_delete_char(obj);
    //////删除光标后面的字符
    ////lv_textarea_delete_char_forward(obj);
    ////设置单行模式
    //lv_textarea_set_one_line(obj, true);

    ////设置接收的的文本内容,除此之外的文本输入没用
    //lv_textarea_set_accepted_chars(obj, "abcdefg");
    ////设置最大接收长度
    //lv_textarea_set_max_length(obj, 8);

    ////设置输入密码模式，默认显示1.5s后隐藏
    //lv_textarea_set_password_mode(obj, true);
    ////修改密码模式时密码显示时间单位ms
    //lv_textarea_set_password_show_time(obj,500);
    ////设置占位符
    //lv_textarea_set_placeholder_text(obj, "password");


    ////获取文本区域的文本
    //const char* temp = lv_textarea_get_text(obj);
    //printf("%s",temp);

    ////创建键盘部件并关联textarea
    //lv_obj_t* kb = lv_keyboard_create(lv_scr_act());
    //lv_keyboard_set_textarea(kb, obj);

    show_ta();

    return;
}
