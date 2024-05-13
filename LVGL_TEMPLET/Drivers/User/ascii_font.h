#ifndef __BSP_FONTS_H__
#define __BSP_FONTS_H__


#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <string.h>


#define      WIDTH_CH_CHAR		                16	    //�����ַ���� 
#define      HEIGHT_CH_CHAR		              	16		  //�����ַ��߶� 

#define LINE(x) ((x) * (((sFONT *)LCD_GetFont())->Height))


	/*ʹ��FLASH��ģ*/
	/*�����ֿ�洢��FLASH����ʼ��ַ*/
	/*FLASH*/
	#define GBKCODE_START_ADDRESS   0


	/*��ȡ�ֿ�ĺ���*/
	//�����ȡ�����ַ���ģ����ĺ�������ucBufferΪ�����ģ��������usCharΪ�����ַ��������룩
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
