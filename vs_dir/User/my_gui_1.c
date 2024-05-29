#include "my_gui.h"
#include "lvgl/lvgl.h"
#include <stdio.h>

int32_t scr_act_width;
int32_t scr_act_hight;
lv_obj_t* obj1;
lv_obj_t* obj2;
lv_obj_t* obj3;




static void test_cb(lv_event_t* event)
{
    lv_obj_t* tempObj = lv_event_get_target(event);
    lv_event_code_t tempCode = lv_event_get_code(event);
    
    if (tempObj == obj1)
    {
        if (tempCode == LV_EVENT_PRESSED)
        {
            lv_obj_set_align(obj1, LV_ALIGN_CENTER);
            printf("obj1_LV_EVENT_PRESSED\r\n");
        }
        else if(tempCode == LV_EVENT_CLICKED)
        {
            lv_obj_set_align(obj1, LV_ALIGN_TOP_MID);
            printf("obj1_LV_EVENT_CLICKED\r\n");
        }
    }
    else if (tempObj == obj2)
    {
        if (tempCode == LV_EVENT_PRESSED)
        {
            lv_obj_align(obj2, LV_ALIGN_CENTER, 10, 0);
            printf("obj2_LV_EVENT_PRESSED\r\n");
        }
        else if (tempCode == LV_EVENT_CLICKED)
        {
            lv_obj_align(obj2, LV_ALIGN_CENTER, 0, 0);
            printf("obj2_LV_EVENT_CLICKED\r\n");
        }
    }
    else if (tempObj == obj3)
    {
        if (tempCode == LV_EVENT_LONG_PRESSED_REPEAT || tempCode == LV_EVENT_PRESSING || tempCode == LV_EVENT_PRESSED)
        {
            uint32_t sliderValue = lv_slider_get_value(obj3);
            lv_obj_align(obj2, LV_ALIGN_CENTER, sliderValue, 0);
            printf("obj3__EVENT，value = %d\r\n", sliderValue);
        }
    }
}


void GUI_test(void)
{
    scr_act_width = lv_obj_get_width(lv_scr_act());
    scr_act_hight = lv_obj_get_height(lv_scr_act());

    //父对象
    obj1 = lv_obj_create(lv_scr_act());
    lv_obj_set_size(obj1, scr_act_width * 2 / 3, scr_act_hight * 2 / 3);
    lv_obj_set_align(obj1, LV_ALIGN_TOP_MID);
    lv_obj_set_style_bg_color(obj1, lv_color_hex(0x00FFFF), LV_STATE_DEFAULT);
    lv_obj_add_event_cb(obj1, test_cb, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(obj1, test_cb, LV_EVENT_CLICKED, NULL);

    //子对象1
    obj2 = lv_obj_create(obj1);                                                     //创建obj1的子对象obj2
    lv_obj_set_size(obj2, scr_act_width / 3, scr_act_hight / 3);                    //设置宽高
    lv_obj_set_align(obj2, LV_ALIGN_CENTER);                                        //设置对齐方式，注意如果带偏移量的函数实现，后续改变位置也要用lv_obj_align
    lv_obj_set_style_bg_color(obj2, lv_color_hex(0x800080), LV_STATE_DEFAULT);      //设置背景颜色为蓝色，默认就变色
    lv_obj_set_style_bg_color(obj2, lv_color_hex(0x800000), LV_STATE_PRESSED);      //设置背景颜色为紫色，按下时变色
    lv_obj_add_event_cb(obj2, test_cb, LV_EVENT_PRESSED, NULL);                     //设置按下的触发事件
    lv_obj_add_event_cb(obj2, test_cb, LV_EVENT_CLICKED, NULL);                     //设置点击的触发事件


    //子对象2
    obj3 = lv_slider_create(obj1);                                                    
    lv_obj_set_width(obj3, scr_act_width/2);                                    
    lv_obj_set_align(obj3, LV_ALIGN_BOTTOM_MID);                                       
    lv_obj_set_style_bg_color(obj3, lv_color_hex(0x49bc40), LV_STATE_DEFAULT );                         //设置背景颜色为绿色


    //自己认为的样式使用规范：生效情况（默认还是被按下还是其他）| 生效范围（手柄还是指示器还是背景）
    //情况和范围哪个不填就是默认的（情况是默认不操作时，范围是背景）
    lv_obj_set_style_bg_color(obj3, lv_color_hex(0x49bc40), LV_STATE_DEFAULT | LV_PART_INDICATOR);       //设置INDICATOR指示器为默认时绿色
    lv_obj_set_style_bg_color(obj3, lv_color_hex(0xFFFF00),  LV_STATE_DEFAULT | LV_PART_KNOB);            //设置KNOB手柄为默认时黄色
    lv_obj_set_style_bg_color(obj3, lv_color_hex(0xFFA500),  LV_STATE_PRESSED | LV_PART_KNOB);            //设置KNOB手柄为按下时黄色   
    lv_obj_add_event_cb(obj3, test_cb, LV_EVENT_LONG_PRESSED_REPEAT , NULL);
    lv_obj_add_event_cb(obj3, test_cb, LV_EVENT_PRESSING , NULL);
    lv_obj_add_event_cb(obj3, test_cb, LV_EVENT_PRESSED, NULL);  //设置按下的触发事件

    return;
}
