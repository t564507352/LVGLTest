/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_timer.c
 * 摘要：
 *  
 * 修改历史     版本号       Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/
#include "main.h"
#include "driver_timer.h"


extern TIM_HandleTypeDef htim2;

/*
 *  函数名：void us_timer_delay(uint16_t t)
 *  输入参数：t-延时时间us
 *  输出参数：无
 *  返回值：无
 *  函数作用：定时器实现的延时函数，延时时间为t us，为了缩短时间，函数体使用寄存器操作，用户可对照手册查看每个寄存器每一位的意义
*/
void us_timer_delay(uint16_t t)
{
    uint16_t counter = 0;
	__HAL_TIM_SET_AUTORELOAD(&htim2, t);		
	__HAL_TIM_SET_COUNTER(&htim2, counter);
	HAL_TIM_Base_Start(&htim2);
	while(counter != t)
	{
		counter = __HAL_TIM_GET_COUNTER(&htim2);
	}
	HAL_TIM_Base_Stop(&htim2);
}


void ms_timer_delay(uint16_t t)
{
		us_timer_delay(t*1000);
}