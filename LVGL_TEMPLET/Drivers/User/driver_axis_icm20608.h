#ifndef __BSP_ICM20608_H_
#define __BSP_ICM20608_H_
#include "main.h"
#include "spi.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "typedefs.h"
#include "event_groups.h"

#define TTL_H       													GPIO_PIN_SET
#define TTL_L       													GPIO_PIN_RESET 
#define DUMMY_DATA  													0xFF
#define ICM_WRITE															0x00
#define ICM_READ															0x80


#define ICM_WHO_AM_I 													0x75
#define ICM_WOH_AM_I_VALUE										0xAF


//初始化，电源重启,时钟选择，采集率选择，量程选择，关闭中断，关闭FIFO，全采集模式
#define ICM_PWR_MGMT_1												0x6B		//时钟选择和睡眠模式选择，芯片温度测量开关 				 		    
#define ICM_PWR_MGMT_2												0x6C		//陀螺仪，加速计开关																  	
#define ICM_SMPLRT_DIV												0x19    //采样率设置为0x00为最大
#define ICM_CONFIG														0x1A    //陀螺仪的DLPF_CFG滤波，设置为4
#define ICM_GYRO_CONFIG												0x1B    //陀螺仪总刻度和FCHOICE_B，其中FCHOICE_B设置为00，采样率的计算公式和DLPF_CFG滤波设置才有作用
#define ICM_ACCEL_CONFIG											0x1C		//设置加速计的总刻度
#define ICM_ACCEL_CONFIG2											0x1D		//设置ACCEL_FCHOICE_B关闭，设置A_DLPF_CFG设置为4    	 
#define ICM_LP_MODE_CFG												0x1E		//低功耗模式的相关配置																	
#define ICM_FIFO_EN														0x23		//失能FIFO		
#define ICM_INT_ENABLE												0x38		//中断  																							
#define ACCEL_WOM_THR													0x1F	 	//加速度计唤醒运动中断的阈值														
#define ACCEL_INTEL_CTRL											0x69   //加速计智能控制  
#define INT_PIN_CFG														0x37   //中断pin设置


//加速计数据读取
#define ICM_ACCEL_XOUT_H											0x3B
#define ICM_ACCEL_XOUT_L											0x3C
#define ICM_ACCEL_YOUT_H											0x3D
#define ICM_ACCEL_YOUT_L											0x3E
#define ICM_ACCEL_ZOUT_H											0x3F
#define ICM_ACCEL_ZOUT_L											0x40


//芯片温度数据读取
#define ICM_TEMP_OUT_H												0x41
#define ICM_TEMP_OUT_L												0x42


//陀螺仪数据读取
#define ICM_GYRO_XOUT_H												0x43
#define ICM_GYRO_XOUT_L												0x44
#define ICM_GYRO_YOUT_H												0x45
#define ICM_GYRO_YOUT_L												0x46
#define ICM_GYRO_ZOUT_H												0x47
#define ICM_GYRO_ZOUT_L												0x48

/* 传感器数据修正值（消除芯片固定误差，根据硬件进行调整） */
#define ICM20608_X_ACCEL_OFFSET	(-64) 
#define ICM20608_Y_ACCEL_OFFSET 	(-30)
#define ICM20608_Z_ACCEL_OFFSET 	(14400) 
#define ICM20608_X_GYRO_OFFSET 	(40)
#define ICM20608_Y_GYRO_OFFSET 	(-7)
#define ICM20608_Z_GYRO_OFFSET 	(-14)

#define ICM_CS(level)  do{HAL_GPIO_WritePin(ICM_20608_CS_GPIO_Port, ICM_20608_CS_Pin, level?GPIO_PIN_SET:GPIO_PIN_RESET);}while(0)

#define g_xICM20608DataQueueSize 32

struct icm20608_data {
	int32_t angle_x;
};



//function
void ICM_Init(void);

void ICM_Show_Value(void);

void icm20608_ParseData(int16_t AccX, struct icm20608_data *result);

QueueHandle_t getICM20608DataQueue(void);

void ICM20608GetValue_Task(void *params);

void creatICM20608DataQueue(void);

void createICM20608EventGroup(void);

void ICM_ITInit(void);

int16_t ICM_R_Temp(void);
#endif
