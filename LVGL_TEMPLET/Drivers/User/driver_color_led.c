// SPDX-License-Identifier: GPL-3.0-only
/*
 * Copyright (c) 2008-2023 100askTeam : Dongshan WEI <weidongshan@qq.com> 
 * Discourse:  https://forums.100ask.net
 */

 
/*  Copyright (C) 2008-2023 深圳百问网科技有限公司
 *  All rights reserved
 *
 *
 * 免责声明: 百问网编写的文档，仅供学员学习使用，可以转发或引用(请保留作者信息)，禁止用于商业用途！
 * 免责声明: 百问网编写的程序，可以用于商业用途，但百问网不承担任何后果！
 * 
 * 
 * 本程序遵循GPL V3协议，使用请遵循协议许可
 * 本程序所用的开发板：	DShanMCU-F103
 * 百问网嵌入式学习平台：https://www.100ask.net
 * 百问网技术交流社区：	https://forums.100ask.net
 * 百问网官方B站：				https://space.bilibili.com/275908810
 * 百问网官方淘宝：			https://100ask.taobao.com
 * 联系我们(E-mail)：	  weidongshan@qq.com
 *
 * 版权所有，盗版必究。
 *  
 * 修改历史     版本号           作者        修改内容
 *-----------------------------------------------------
 * 2023.08.04      v01         百问科技      创建文件
 *-----------------------------------------------------
 */


#include "driver_color_led.h"
//#include "driver_lcd.h"
#include "driver_timer.h"
#include "tim.h"

#define CHANNEL_RED   TIM_CHANNEL_3
#define CHANNEL_GREEN TIM_CHANNEL_4
#define CHANNEL_BLUE  TIM_CHANNEL_2

LED_COLOUR ledColour = {0,0,0};
uint16_t LedGrdtFct = 50;
extern TIM_HandleTypeDef htim3;

static TIM_HandleTypeDef *g_HPWM_ColorLED = &htim3;

/**********************************************************************
 * 函数名称： ColorLED_Init
 * 功能描述： 全彩LED的初始化函数
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 * 修改日期：      版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2023/08/04	     V1.0	  韦东山	      创建
 ***********************************************************************/
void ColorLED_Init(void)
{
	MX_TIM3_Init(colorLED);
	HAL_TIM_PWM_Start_IT(g_HPWM_ColorLED,CHANNEL_BLUE);
	HAL_TIM_PWM_Start_IT(g_HPWM_ColorLED,CHANNEL_RED);
	HAL_TIM_PWM_Start_IT(g_HPWM_ColorLED,CHANNEL_GREEN);
}


/**********************************************************************
 * 函数名称： ColorLED_Set
 * 功能描述： 全彩LED设置颜色函数
 * 输入参数： color - 24bit颜色,格式为0x00RRGGBB
 * 输出参数： 无
 * 返 回 值： 无
 * 修改日期：      版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2023/08/04	     V1.0	  韦东山	      创建
 ***********************************************************************/


/**********************************************************************
 * 函数名称： ColorLED_Test
 * 功能描述： 全彩LED测试程序
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2023/08/04        V1.0     韦东山       创建
 ***********************************************************************/
void ColorLED_Test(void)
{
    while (1)
    {
			ledColour.led_r_value = 0;
			ledColour.led_g_value = 0;
			ledColour.led_b_value = 0;
			//ºì--->ÂÌ
			//(255,0,0)-->(0,255,0)
			ledColour.led_r_value = 255;
			for(float i=1; i<LedGrdtFct ; i++)
			{
			ledColour.led_r_value = 255+(0-255)*i/LedGrdtFct;
			ledColour.led_g_value = 0+(255-0)*i/LedGrdtFct;
			ledColour.led_b_value = 0;
			//printf("R=%f G=%f B=%f \n",R,G,B);
			mdelay(4);
			}

			//ÂÌ--->À¶
			//(0,255,0)-->(0,0,255)
			ledColour.led_r_value = 0;
			ledColour.led_g_value = 255;
			for(float i=1; i<LedGrdtFct ; i++)
			{
			ledColour.led_r_value = 0;
			ledColour.led_g_value = 255+(0-255)*i/LedGrdtFct;
			ledColour.led_b_value = 0+(255-0)*i/LedGrdtFct;
			mdelay(4);
			//printf("R=%f G=%f B=%f \n",R,G,B);
			}

			//À¶--->ºì
			//(0,0,255)-->(255,0,0)
			ledColour.led_g_value = 0;
			ledColour.led_b_value = 255;
			for(float i=1; i<LedGrdtFct ; i++)
			{
			ledColour.led_r_value = 0+(255-0)*i/LedGrdtFct;
			ledColour.led_g_value = 0;
			ledColour.led_b_value = 255+(0-255)*i/LedGrdtFct;
			//printf("R=%f G=%f B=%f \n",R,G,B);
			mdelay(4);
			}
    }    
}

