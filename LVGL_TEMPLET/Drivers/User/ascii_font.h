#ifndef __BSP_FONTS_H__
#define __BSP_FONTS_H__


#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <string.h>


#define      WIDTH_CH_CHAR		                16	    //中文字符宽度 
#define      HEIGHT_CH_CHAR		              	16		  //中文字符高度 

#define LINE(x) ((x) * (((sFONT *)LCD_GetFont())->Height))


	/*使用FLASH字模*/
	/*中文字库存储在FLASH的起始地址*/
	/*FLASH*/
	#define GBKCODE_START_ADDRESS   0


	/*获取字库的函数*/
	//定义获取中文字符字模数组的函数名，ucBuffer为存放字模数组名，usChar为中文字符（国标码）
#define      GetGBKCode( ucBuffer, usChar )  GetGBKCode_from_EXFlash( ucBuffer, usChar )  

int GetGBKCode_from_EXFlash( uint8_t * pBuffer, uint16_t c);


typedef struct _tFont
{    
  const uint8_t *table;
  uint16_t Width;
  uint16_t Height;
  
} sFONT;

extern sFONT Font8x16;
extern sFONT Font16x24;
//extern sFONT Font24x32;
#endif
