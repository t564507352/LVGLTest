/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：ring_buffer.c
 * 摘要：
 *  
 * 修改历史     版本号        Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
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

//函数作用：注册IR信号中断解析后的分发队列
void registerDispatchQueue(QueueHandle_t queue)
{
	if(g_dispatchQueueCnt<10)
	{
		g_xDispatchQueue[g_dispatchQueueCnt] = queue;
		g_dispatchQueueCnt++;		
	}
	return;
}

//创建IR数据队列
void creatIRRxDataQueue(void)
{
	g_xIRRxDataQueue = xQueueCreate(g_xIRRxDataQueueSize, 1 );
}

//获得本文件的static的IR队列
QueueHandle_t getIRRxDataQueue(void)
{
	return g_xIRRxDataQueue;
}

//TIM开始捕获IR信号
void startIRInputCapture(void)
{
	HAL_TIM_IC_Start_IT(&htim8,TIM_CHANNEL_2);
}

//LED逐行显示按键内容
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
	// 打印对应按键
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

//测试程序，测试按键并显示在屏幕上,得在freertos下使用
void IRReceiver_Test(void)
{
	IRTestFlag = 1;
	while(1);
}

//在IR捕获中断解析到cmd后调用此函数分发至已注册的队列
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
	//tim8设置为捕获下降沿，Prescaler = 720-1,10us一次数
	//计数100次为1ms
	//计数10000次为100ms
	//uint64_t time;
	//static uint64_t pre_time = 0;
	volatile int32_t temp = 0;
  static uint16_t curTimOrigData = 0, preTimOrigData = 0;
	static uint8_t guidCode ,reptCode= 0;
	volatile uint32_t tempAnalyData = 0;
	uint8_t cnt = 0;
	//一：判断目前发生中断的时刻跟上一时刻相差是否大于100ms，大于的话说明数据无效，cnt=0
//	time = system_get_ns();
//	if (time - pre_time > 100000000) 
//    {
//        g_IRReceiverIRQ_Cnt = 0;
//    }
//  pre_time = time;
	//二：调用函数判断是否是连发码，是的话分发上次数据，cnt=0	
	//判断有没有过一个计数周期并处理
	curTimOrigData = (uint16_t)TIM8->CCR2;
	if((curTimOrigData - preTimOrigData)>0)
	{
		temp = curTimOrigData - preTimOrigData;
	}
	else
	{
		temp = (curTimOrigData - preTimOrigData + 65536);
	}
	//保存本次计数值,用于下次相减
	preTimOrigData = curTimOrigData; 
	//非首次计数
	if(temp != curTimOrigData) //尝试注释掉，因为首次计数的话下面的代码也能处理掉了
	{
			temp *= 10; // 每次计数为10us，算出实际us
	}
	//判断是不是连发码，是的话连发标志++，到一定数量分发航次数据，连发标志和数据数组cnt置0
	if(temp > 10000 && temp < 12000)//连发码11.25ms
	{
		reptCode++;
		if(reptCode == 2)//防止过于灵敏
		{
			reptCode = 0;
			g_IRReceiverIRQ_Cnt = 0;
			DispatchQueue(&ir_data);//分发上次解析的数据
		}
	}
	else if(guidCode == 1)//引导码为1的情况下，继续计数
	{
		//定时器计数数据存入数组，cnt++
		g_IRReceiverIRQ_TimerCnt[g_IRReceiverIRQ_Cnt] = temp;
		g_IRReceiverIRQ_Cnt++;
	}
	//不是连发码，且引导码标志为0，判断是不是引导码，是的话引导吗标志置1，继续计数并存入数组cnt++。不是的话cnt置0
	else if(guidCode == 0 && temp > 13000 && temp < 14000)//引导标志为0，是引导码-> 13.5ms
	{
		guidCode = 1;	
	}
	else//不是连发也不是引导也没进引导标志,可能数据有干扰
	{
		g_IRReceiverIRQ_Cnt = 0;
	}
	
	//计数值达到数据数量后，引导码标志置0，cnt置0，并调用解析函数解析、分发数据
	if(g_IRReceiverIRQ_Cnt == 31)
	{
		//解析数据
        for(uint8_t i=0; i<32; i++)
        {
			//将本计数器值与之前的计数器值相减，得到计数器增加值
					if(g_IRReceiverIRQ_TimerCnt[i]>1000 &&g_IRReceiverIRQ_TimerCnt[i]<1500)// 位0 (持续时间为1.12ms左右)
					{
						tempAnalyData = tempAnalyData + (0<<cnt);
            cnt++;
					}
					else if(g_IRReceiverIRQ_TimerCnt[i] > 2000 && g_IRReceiverIRQ_TimerCnt[i] < 2500) // 位1 (持续时间为2.25ms左右)
					{
						tempAnalyData = tempAnalyData + (1<<cnt);
						cnt++;
					}
				 }
				ir_data.value = tempAnalyData;  // 保存原始32 Bit数据
				DispatchQueue(&ir_data);//分发数据
				g_IRReceiverIRQ_Cnt = 0;
				guidCode = 0;
				return;
	}
}