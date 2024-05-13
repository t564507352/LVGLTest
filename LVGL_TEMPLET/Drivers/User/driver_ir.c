/*  Copyright (s) 2019 ���ڰ������Ƽ����޹�˾
 *  All rights reserved
 * 
 * �ļ����ƣ�ring_buffer.c
 * ժҪ��
 *  
 * �޸���ʷ     �汾��        Author       �޸�����
 *--------------------------------------------------
 * 2020.6.6      v01        ���ʿƼ�      �����ļ�
 *--------------------------------------------------
*/

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "driver_ir.h"
#include "tim.h"
#include "driver_lcd.h"
#include "driver_timer.h"


IRDA_DATA ir_data = {0};

static QueueHandle_t g_xIRRxDataQueue;

static QueueHandle_t g_xDispatchQueue[10];
static uint8_t g_dispatchQueueCnt = 0;

static uint16_t g_IRReceiverIRQ_TimerCnt[64];
static uint8_t g_IRReceiverIRQ_Cnt = 0;

static uint8_t IRTestFlag = 0;

//�������ã�ע��IR�ź��жϽ�����ķַ�����
void registerDispatchQueue(QueueHandle_t queue)
{
	if(g_dispatchQueueCnt<10)
	{
		g_xDispatchQueue[g_dispatchQueueCnt] = queue;
		g_dispatchQueueCnt++;		
	}
	return;
}

//����IR���ݶ���
void creatIRRxDataQueue(void)
{
	g_xIRRxDataQueue = xQueueCreate(g_xIRRxDataQueueSize, 1 );
}

//��ñ��ļ���static��IR����
QueueHandle_t getIRRxDataQueue(void)
{
	return g_xIRRxDataQueue;
}

//TIM��ʼ����IR�ź�
void startIRInputCapture(void)
{
	HAL_TIM_IC_Start_IT(&htim8,TIM_CHANNEL_2);
}

//LED������ʾ��������
void IRDA_LCD_Display(uint8_t* pdata )
{
	static uint8_t lineCounter = 0;
	LCD_ShowStringLine_EN (LINE(lineCounter), (char*)pdata);
	if(lineCounter < 29)
	{
		lineCounter++;
	}
	else
	{
		LCD_Clear(WHITE);
		lineCounter = 0;
	}
}


void GetKeyInfoAndDisPlay(uint8_t cmd_code)
{
    uint8_t showInfo[20];
	// ��ӡ��Ӧ����
    switch(cmd_code)  
    {
        case OPEN:
        {
            printf("Open          Key Value = 0x%x \n\r",cmd_code);
						sprintf((char*)showInfo,"Open,Value = 0x%x",cmd_code);
						IRDA_LCD_Display(showInfo);
            break;
        }
        case MENU:
        {
            printf("Menu          Key Value = 0x%x \n\r",cmd_code);
						sprintf((char*)showInfo,"Menu,Value = 0x%x",cmd_code);
						IRDA_LCD_Display(showInfo);
            break;
        }
        case TEST:
        {
            printf("Test          Key Value = 0x%x \n\r",cmd_code);
						sprintf((char*)showInfo,"Test,Value = 0x%x",cmd_code);
						IRDA_LCD_Display(showInfo);
            break;
        }
        case INCREASE:
        {
            printf("+             Key Value = 0x%x \n\r",cmd_code);
						sprintf((char*)showInfo,"+,Value = 0x%x",cmd_code);
						IRDA_LCD_Display(showInfo);
            break;
        }
        case BACK:
        {
            printf("Back          Key Value = 0x%x \n\r",cmd_code);
						sprintf((char*)showInfo,"Back,Value = 0x%x",cmd_code);
						IRDA_LCD_Display(showInfo);
            break;
        }
        case GOBACK:
        {
            printf("Fast Back     Key Value = 0x%x \n\r",cmd_code);
						sprintf((char*)showInfo,"Fast Back,Value = 0x%x",cmd_code);
						IRDA_LCD_Display(showInfo);
            break;
        }
        case PLAY:
        {
            printf("Play          Key Value = 0x%x \n\r",cmd_code);
						sprintf((char*)showInfo,"Play,Value = 0x%x",cmd_code);
						IRDA_LCD_Display(showInfo);
            break;
        }
        case FASTFORWARD:
        {
            printf("Fast Forward  Key Value = 0x%x \n\r",cmd_code);
						sprintf((char*)showInfo,"Fast Forward,Value = 0x%x",cmd_code);
						IRDA_LCD_Display(showInfo);
            break;
        }
        case ZERO:
        {
            printf("0             Key Value = 0x%x \n\r",cmd_code);
						sprintf((char*)showInfo,"0,Value = 0x%x",cmd_code);
						IRDA_LCD_Display(showInfo);
            break;
        }
        case DECREASE:
        {
            printf("-             Key Value = 0x%x \n\r",cmd_code);
						sprintf((char*)showInfo,"-,Value = 0x%x",cmd_code);
						IRDA_LCD_Display(showInfo);
            break;
        }
        case CANCEL:
        {
            printf("Cancel        Key Value = 0x%x \n\r",cmd_code);
						sprintf((char*)showInfo,"Cancel,Value = 0x%x",cmd_code);
						IRDA_LCD_Display(showInfo);
            break;
        }
        case ONE:
        {
            printf("1             Key Value = 0x%x \n\r",cmd_code);
						sprintf((char*)showInfo,"1,Value = 0x%x",cmd_code);
						IRDA_LCD_Display(showInfo);
            break;  
        }           
        case TWO:   
        {           
            printf("2             Key Value = 0x%x \n\r",cmd_code);
						sprintf((char*)showInfo,"2,Value = 0x%x",cmd_code);
						IRDA_LCD_Display(showInfo);
            break; 
        }          
        case THREE:
        {          
            printf("3             Key Value = 0x%x \n\r",cmd_code);
						sprintf((char*)showInfo,"3,Value = 0x%x",cmd_code);
						IRDA_LCD_Display(showInfo);
            break;  
        }           
        case FOUR:  
        {           
            printf("4             Key Value = 0x%x \n\r",cmd_code);
						sprintf((char*)showInfo,"4,Value = 0x%x",cmd_code);
						IRDA_LCD_Display(showInfo);
            break;  
        }           
        case FIVE:  
        {           
            printf("5             Key Value = 0x%x \n\r",cmd_code);
						sprintf((char*)showInfo,"5,Value = 0x%x",cmd_code);
						IRDA_LCD_Display(showInfo);
            break;  
        }           
        case SIX:   
        {           
            printf("6             Key Value = 0x%x \n\r",cmd_code);
						sprintf((char*)showInfo,"6,Value = 0x%x",cmd_code);
						IRDA_LCD_Display(showInfo);
            break;  
        }           
        case SEVEN: 
        {           
            printf("7             Key Value = 0x%x \n\r",cmd_code);
						sprintf((char*)showInfo,"7,Value = 0x%x",cmd_code);
						IRDA_LCD_Display(showInfo);
            break;  
        }           
        case EIGHT: 
        {           
            printf("8             Key Value = 0x%x \n\r",cmd_code);
						sprintf((char*)showInfo,"8,Value = 0x%x",cmd_code);
						IRDA_LCD_Display(showInfo);
            break;  
        }           
        case NINE:  
        {           
            printf("9             Key Value = 0x%x \n\r",cmd_code);
						sprintf((char*)showInfo,"9,Value = 0x%x",cmd_code);
						IRDA_LCD_Display(showInfo);
            break;
        }
        default:
				{
					printf("NULL            Key Value = 0x%x \n\r",cmd_code);
					sprintf((char*)showInfo,"NULL,Value = 0x%x",cmd_code);
					IRDA_LCD_Display(showInfo);
					break;
				}
    }
}

//���Գ��򣬲��԰�������ʾ����Ļ��,����freertos��ʹ��
void IRReceiver_Test(void)
{
	IRTestFlag = 1;
	while(1);
}

//��IR�����жϽ�����cmd����ô˺����ַ�����ע��Ķ���
void DispatchQueue(IRDA_DATA* sendData)
{
		
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	volatile uint8_t i,tempCmdCode;
	tempCmdCode = (sendData->value>>16)&0xFF;
	for(i=0 ; i<g_dispatchQueueCnt ; i++)
	{
		xQueueSendToBackFromISR(g_xDispatchQueue[i],(uint8_t*)&tempCmdCode,&xHigherPriorityTaskWoken);
	}
	if(IRTestFlag == 1)
	{
		GetKeyInfoAndDisPlay(tempCmdCode);
	}
		
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void IR_CaptureCallback(void)
{
	//tim8����Ϊ�����½��أ�Prescaler = 720-1,10usһ����
	//����100��Ϊ1ms
	//����10000��Ϊ100ms
	//uint64_t time;
	//static uint64_t pre_time = 0;
	volatile int32_t temp = 0;
  static uint16_t curTimOrigData = 0, preTimOrigData = 0;
	static uint8_t guidCode ,reptCode= 0;
	volatile uint32_t tempAnalyData = 0;
	uint8_t cnt = 0;
	//һ���ж�Ŀǰ�����жϵ�ʱ�̸���һʱ������Ƿ����100ms�����ڵĻ�˵��������Ч��cnt=0
//	time = system_get_ns();
//	if (time - pre_time > 100000000) 
//    {
//        g_IRReceiverIRQ_Cnt = 0;
//    }
//  pre_time = time;
	//�������ú����ж��Ƿ��������룬�ǵĻ��ַ��ϴ����ݣ�cnt=0	
	//�ж���û�й�һ���������ڲ�����
	curTimOrigData = (uint16_t)TIM8->CCR2;
	if((curTimOrigData - preTimOrigData)>0)
	{
		temp = curTimOrigData - preTimOrigData;
	}
	else
	{
		temp = (curTimOrigData - preTimOrigData + 65536);
	}
	//���汾�μ���ֵ,�����´����
	preTimOrigData = curTimOrigData; 
	//���״μ���
	if(temp != curTimOrigData) //����ע�͵�����Ϊ�״μ����Ļ�����Ĵ���Ҳ�ܴ������
	{
			temp *= 10; // ÿ�μ���Ϊ10us�����ʵ��us
	}
	//�ж��ǲ��������룬�ǵĻ�������־++����һ�������ַ��������ݣ�������־����������cnt��0
	if(temp > 10000 && temp < 12000)//������11.25ms
	{
		reptCode++;
		if(reptCode == 2)//��ֹ��������
		{
			reptCode = 0;
			g_IRReceiverIRQ_Cnt = 0;
			DispatchQueue(&ir_data);//�ַ��ϴν���������
		}
	}
	else if(guidCode == 1)//������Ϊ1������£���������
	{
		//��ʱ���������ݴ������飬cnt++
		g_IRReceiverIRQ_TimerCnt[g_IRReceiverIRQ_Cnt] = temp;
		g_IRReceiverIRQ_Cnt++;
	}
	//���������룬���������־Ϊ0���ж��ǲ��������룬�ǵĻ��������־��1��������������������cnt++�����ǵĻ�cnt��0
	else if(guidCode == 0 && temp > 13000 && temp < 14000)//������־Ϊ0����������-> 13.5ms
	{
		guidCode = 1;	
	}
	else//��������Ҳ��������Ҳû��������־,���������и���
	{
		g_IRReceiverIRQ_Cnt = 0;
	}
	
	//����ֵ�ﵽ�����������������־��0��cnt��0�������ý��������������ַ�����
	if(g_IRReceiverIRQ_Cnt == 31)
	{
		//��������
        for(uint8_t i=0; i<32; i++)
        {
			//����������ֵ��֮ǰ�ļ�����ֵ������õ�����������ֵ
					if(g_IRReceiverIRQ_TimerCnt[i]>1000 &&g_IRReceiverIRQ_TimerCnt[i]<1500)// λ0 (����ʱ��Ϊ1.12ms����)
					{
						tempAnalyData = tempAnalyData + (0<<cnt);
            cnt++;
					}
					else if(g_IRReceiverIRQ_TimerCnt[i] > 2000 && g_IRReceiverIRQ_TimerCnt[i] < 2500) // λ1 (����ʱ��Ϊ2.25ms����)
					{
						tempAnalyData = tempAnalyData + (1<<cnt);
						cnt++;
					}
				 }
				ir_data.value = tempAnalyData;  // ����ԭʼ32 Bit����
				DispatchQueue(&ir_data);//�ַ�����
				g_IRReceiverIRQ_Cnt = 0;
				guidCode = 0;
				return;
	}
}