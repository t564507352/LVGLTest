#include "driver_axis_icm20608.h"
#include "spi.h"
#include <math.h>
#include "driver_lcd.h"
//uint8_t ICM_20608_RW(uint8_t regAddr,uint8_t inst);
//uint8_t ICM_GetDeviceID(void);
//int16_t ICM_R_Temp(void);
//int16_t ICM_R_ACCEL_X(void);
//int16_t ICM_R_ACCEL_Y(void);
//int16_t ICM_R_ACCEL_Z(void);
//int16_t ICM_R_GYRO_X(void);
//int16_t ICM_R_GYRO_Y(void);
//int16_t ICM_R_GYRO_Z(void);

static volatile QueueHandle_t g_xICM20608DataQueue = {0};
static EventGroupHandle_t g_xICM20608EventGroup;
volatile uint8_t  resultFlag = 0;
//6轴传感器的专用读写函数，
//第一个参数是对寄存器是读还是写传参时与上宏定义,第二个参数是指令，如果是要读寄存器就传个dummy
uint8_t ICM_20608_RW(uint8_t regAddr,uint8_t inst)
{
	uint8_t result;
	uint8_t data[2] = {0};
	ICM_CS(TTL_L);
	result = HAL_SPI_TransmitReceive(&hspi1,&regAddr,&data[0],1,10);
	result = HAL_SPI_TransmitReceive(&hspi1,&inst,&data[1],1,10);
	ICM_CS(TTL_H);
	return data[1];
}




#if 0
//初始化
void ICM_Init(void)
{
	//RESET
	ICM_20608_RW(ICM_PWR_MGMT_1|ICM_WRITE,0x80);
	HAL_Delay(100);
	//选择时钟，关闭睡眠模式,打开芯片温度测量开关
	ICM_20608_RW(ICM_PWR_MGMT_1|ICM_WRITE,0x00);
	HAL_Delay(50);
	//FIFO_EN中失能FIFO
	ICM_20608_RW(ICM_FIFO_EN|ICM_WRITE,0x00);
	HAL_Delay(50);
	//设置陀螺仪的3db滤波20HZ
	ICM_20608_RW(ICM_CONFIG|ICM_WRITE,0x04);
	HAL_Delay(50);
	//设置陀螺仪的总量程为最大的2000dps和FCHOICE_B为00
	ICM_20608_RW(ICM_GYRO_CONFIG|ICM_WRITE,0x18);
	HAL_Delay(50);
	//设置加速计量程为最大的16g
	ICM_20608_RW(ICM_ACCEL_CONFIG|ICM_WRITE,0x18);
	HAL_Delay(50);
	//设置加速计FCHOICE_B为00和3db低通滤波设置为21.2
	ICM_20608_RW(ICM_ACCEL_CONFIG2|ICM_WRITE,0x04);
	HAL_Delay(50);
	//设置采样率
	ICM_20608_RW(ICM_SMPLRT_DIV|ICM_WRITE,0x00);
	HAL_Delay(50);
	//失能所有中断
	ICM_20608_RW(ICM_INT_ENABLE|ICM_WRITE,0x00);
	HAL_Delay(50);
	//陀螺仪加速计6轴全开
	ICM_20608_RW(ICM_PWR_MGMT_2|ICM_WRITE,0x00);
	HAL_Delay(50);
	return;
}
#else
void ICM_Init(void)
{
	//RESET
	ICM_20608_RW(ICM_PWR_MGMT_1|ICM_WRITE,0x80);
	HAL_Delay(100);
	//===============以下为手册要求的步骤==================
	//选择时钟，关闭睡眠模式,打开芯片温度测量开关
	ICM_20608_RW(ICM_PWR_MGMT_1|ICM_WRITE,0x00);
	HAL_Delay(50);
	//陀螺仪关  加速计开
	ICM_20608_RW(ICM_PWR_MGMT_2|ICM_WRITE,0x07);
	HAL_Delay(50);
	//设置加速计FCHOICE_B为01
	ICM_20608_RW(ICM_ACCEL_CONFIG2|ICM_WRITE,0x09);
	HAL_Delay(50);
	//使能运动中断
	ICM_20608_RW(ICM_INT_ENABLE|ICM_WRITE,0xE0);
	HAL_Delay(50);
	//运动中断阈值设置
	ICM_20608_RW(ACCEL_WOM_THR|ICM_WRITE,0x40);//得研究下，改下值
	HAL_Delay(50);
	//开启加速计智能中断，开启与上一数值对比
	ICM_20608_RW(ACCEL_INTEL_CTRL|ICM_WRITE,0xC0);
	HAL_Delay(50);
	//新增
	//设置采样率1000/1+3=250
	ICM_20608_RW(ICM_SMPLRT_DIV|ICM_WRITE,0x03);
	HAL_Delay(50);	
	
	//唤醒芯片的频率最大
	ICM_20608_RW(ICM_LP_MODE_CFG|ICM_WRITE,0x0B);//0x0B->最高  0x09->100多
	HAL_Delay(50);
	//设置加速计开启循环模式
	ICM_20608_RW(ICM_PWR_MGMT_1|ICM_WRITE,0x20);//0x20开启循环  0x00关闭
	HAL_Delay(50);
	//================以上为手册要求的步骤================
	
//	//设置中断引脚
//	ICM_20608_RW(INT_PIN_CFG|ICM_WRITE,0x10);//0x10,中断后引脚电平保持50us自动恢复高；0x30 中断状态被读取后在恢复高电平
//	HAL_Delay(50);	
	//设置加速计量程为最大的16g
//	ICM_20608_RW(ICM_ACCEL_CONFIG|ICM_WRITE,0x18);
//	HAL_Delay(50);	
//	//FIFO_EN中失能FIFO
//	ICM_20608_RW(ICM_FIFO_EN|ICM_WRITE,0x00);
//	HAL_Delay(50);
//	//设置陀螺仪的3db滤波20HZ
//	ICM_20608_RW(ICM_CONFIG|ICM_WRITE,0x04);
//	HAL_Delay(50);
//	//设置陀螺仪的总量程为最大的2000dps和FCHOICE_B为00
//	ICM_20608_RW(ICM_GYRO_CONFIG|ICM_WRITE,0x18);
//	HAL_Delay(50);

//	//设置采样率
//	ICM_20608_RW(ICM_SMPLRT_DIV|ICM_WRITE,0x00);
//	HAL_Delay(50);

	return;
}



#endif


void ICM_ITInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitStruct.Pin = GPIO_PIN_1;
//  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_NVIC_SetPriority(EXTI1_IRQn, 15, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);
}



uint8_t ICM_GetDeviceID(void)
{
	uint8_t deviceId = 0;
	deviceId=ICM_20608_RW(ICM_WHO_AM_I|ICM_READ,DUMMY_DATA);
	printf("DeviceId = 0x%x\r\n",deviceId);
	return deviceId;
}
int16_t ICM_R_Temp(void)
{
	uint8_t temp_h = 0;
	uint8_t temp_l = 0;
	int16_t tempData = 0;
	temp_h = ICM_20608_RW(ICM_TEMP_OUT_H|ICM_READ,DUMMY_DATA);
	temp_l = ICM_20608_RW(ICM_TEMP_OUT_L|ICM_READ,DUMMY_DATA);
	tempData = (temp_h << 8)|temp_l;
	return tempData;
}
	
int16_t ICM_R_ACCEL_X(void)
{
	uint8_t accelX_h = 0;
	uint8_t accelX_l = 0;
	int16_t accelXData = 0;
	accelX_h = ICM_20608_RW(ICM_ACCEL_XOUT_H|ICM_READ,DUMMY_DATA);
	accelX_l = ICM_20608_RW(ICM_ACCEL_XOUT_L|ICM_READ,DUMMY_DATA);
	accelXData = (accelX_h << 8)|accelX_l;
	return accelXData;
}

int16_t ICM_R_ACCEL_Y(void)
{
	uint8_t accelY_h = 0;
	uint8_t accelY_l = 0;
	int16_t accelYData = 0;
	accelY_h = ICM_20608_RW(ICM_ACCEL_YOUT_H|ICM_READ,DUMMY_DATA);
	accelY_l = ICM_20608_RW(ICM_ACCEL_YOUT_L|ICM_READ,DUMMY_DATA);
	accelYData = (accelY_h << 8)|accelY_l;
	return accelYData;
}

int16_t ICM_R_ACCEL_Z(void)
{
	uint8_t accelZ_h = 0;
	uint8_t accelZ_l = 0;
	int16_t accelZData = 0;
	accelZ_h = ICM_20608_RW(ICM_ACCEL_ZOUT_H|ICM_READ,DUMMY_DATA);
	accelZ_l = ICM_20608_RW(ICM_ACCEL_ZOUT_L|ICM_READ,DUMMY_DATA);
	accelZData = (accelZ_h << 8)|accelZ_l;
	return accelZData;
}

int16_t ICM_R_GYRO_X(void)
{
	uint8_t gyroX_h = 0;
	uint8_t gyroX_l = 0;
	int16_t gyroXData = 0;
	gyroX_h = ICM_20608_RW(ICM_GYRO_XOUT_H	|ICM_READ,DUMMY_DATA);
	gyroX_l = ICM_20608_RW(ICM_GYRO_XOUT_L	|ICM_READ,DUMMY_DATA);
	gyroXData = (gyroX_h << 8)|gyroX_l;
	return gyroXData;
}
int16_t ICM_R_GYRO_Y(void)
{
	uint8_t gyroY_h = 0;
	uint8_t gyroY_l = 0;
	int16_t gyroYData = 0;
	gyroY_h = ICM_20608_RW(ICM_GYRO_YOUT_H	|ICM_READ,DUMMY_DATA);
	gyroY_l = ICM_20608_RW(ICM_GYRO_YOUT_L	|ICM_READ,DUMMY_DATA);
	gyroYData = (gyroY_h << 8)|gyroY_l;
	return gyroYData;
}

int16_t ICM_R_GYRO_Z(void)
{
	uint8_t gyroZ_h = 0;
	uint8_t gyroZ_l = 0;
	int16_t gyroZData = 0;
	gyroZ_h = ICM_20608_RW(ICM_GYRO_ZOUT_H	|ICM_READ,DUMMY_DATA);
	gyroZ_l = ICM_20608_RW(ICM_GYRO_ZOUT_L	|ICM_READ,DUMMY_DATA);
	gyroZData = (gyroZ_h << 8)|gyroZ_l;
	return gyroZData;
}

void ICM_Show_Value(void)
{
	int16_t tempReadValue = 0;
	float tempActValue = 0;
	int16_t accelReadValueX = 0;
	float accelActValueX = 0;
	int16_t accelReadValueY = 0;
	float accelActValueY = 0;
	int16_t accelReadValueZ = 0;
	float accelActValueZ = 0;
	int16_t gyroReadValueX = 0;
	float gyroActValueX = 0;
	int16_t gyroReadValueY = 0;
	float gyroActValueY = 0;
	int16_t gyroReadValueZ = 0;
	float gyroActValueZ = 0;
	
	
	//温度获取与转换
	//  TEMP_degC = ((TEMP_OUT – RoomTemp_Offset)/Temp_Sensitivity) + 25degC
	//  RoomTemp_Offset = 25°C
	//	Temp_Sensitivity = 326.8 LSB/°C
	tempReadValue = ICM_R_Temp();
	tempActValue =  ((tempReadValue - 25)/326.8) + 25;
	
	
	//加速计数值转换
	//15bit能表示的最大值为32767，本项目加速计量程设置为16
	//实际值的方程式为32767/16=ReadValue/ActValue,则ActValue=ReadValue/2047.93
	//加速计X获取与转换
	accelReadValueX = ICM_R_ACCEL_X();
	accelActValueX = accelReadValueX/2048;
	//加速计Y获取与转换
	accelReadValueY = ICM_R_ACCEL_Y();
	accelActValueY = accelReadValueY/2048;
	//加速计Z获取与转换
	accelReadValueZ = ICM_R_ACCEL_Z();
	accelActValueZ = accelReadValueZ/2048;
	
	
	//加速计数值转换
	//15bit能表示的最大值为32767，本项目陀螺仪量程设置为2000
	//实际值的方程式为32767/2000=ReadValue/ActValue,则ActValue=ReadValue/16.38
	//陀螺仪X获取与转换
	gyroReadValueX = ICM_R_GYRO_X();
	gyroActValueX = gyroReadValueX/16;
	//陀螺仪Y获取与转换
	gyroReadValueY = ICM_R_GYRO_Y();
	gyroActValueY = gyroReadValueY/16;
	//陀螺仪Z获取与转换
	gyroReadValueZ = ICM_R_GYRO_Z();
	gyroActValueZ = gyroReadValueZ/16;
	
	
	printf("tempReadValue = 0x%x\r\n",tempReadValue);
	printf("accelReadValue: X=0x%x, Y=0x%x, Z=0x%x\r\n",accelReadValueX,accelReadValueY,accelReadValueZ);
	printf("gyroReadValue: X=0x%x, Y=0x%x, Z=0x%x\r\n",gyroReadValueX,gyroReadValueY,gyroReadValueZ);
	printf("  \r\n");
	printf("tempActValue = %0.2fdegC\r\n",tempActValue);
	printf("accelActValue: X=%0.2f, Y=%0.2f, Z=%0.2f\r\n",accelActValueX,accelActValueY,accelActValueZ);
	printf("gyroActValue: X=%0.2f, Y=%0.2f, Z=%0.2f\r\n",gyroActValueX,gyroActValueY,gyroActValueZ);
	return;
}

//void icm20608_ParseData(int16_t AccX, int16_t AccY, int16_t AccZ, int16_t GyroX, int16_t GyroY, int16_t GyroZ, struct icm20608_data *result)
//{
//	if (result)
//	{
//		result->angle_x = (int32_t)(acos((double)((double)(AccX + ICM20608_X_ACCEL_OFFSET) / 16384.0)) * 57.29577);
//	}
//}

//创建陀螺仪队列函数
void creatICM20608DataQueue(void)
	{
		g_xICM20608DataQueue = xQueueCreate(g_xICM20608DataQueueSize, sizeof(struct icm20608_data) );//IRtimer中断用的环形buffer数据队列
	}

//创建陀螺仪中断和查询数据任务用的事件组函数
void createICM20608EventGroup(void)
	{
		g_xICM20608EventGroup = xEventGroupCreate();
	}

//解析陀螺仪旋转角度函数
void icm20608_ParseData(int16_t AccX, struct icm20608_data *result)
{
	if (result)
	{
		result->angle_x = (int32_t)(acos((double)((double)(AccX + ICM20608_X_ACCEL_OFFSET) / 16384.0)) * 57.29577);
	}
}

//获取本文件的全局static队列句柄
QueueHandle_t getICM20608DataQueue(void)
{
	return g_xICM20608DataQueue;
}

//定时解析陀螺仪数据并写队列任务
void ICM20608GetValue_Task(void *params)
{	
  int16_t AccX;
	struct icm20608_data result;
	while (1)
	{  
	//等待事件组
		xEventGroupWaitBits(g_xICM20608EventGroup , (1<<0) , pdTRUE , pdTRUE , portMAX_DELAY);
	/* 读数据 */
		do
		{
			AccX = ICM_R_ACCEL_X();
			if(AccX)
			{
				/* 解析数据 */
				icm20608_ParseData(AccX, &result);

				/* 写队列 */
				xQueueSend(g_xICM20608DataQueue, &result, 1);
				if (result.angle_x != 90)
				vTaskDelay(60);
			}			
		}while(result.angle_x != 90);
		

		/* delay */
		vTaskDelay(20);
	}
}

#if 0
void ICM20608Test(void)
{
	uint32_t AccX;
	struct icm20608_data result;
	while (1)
	{    
	/* 读数据 */
		AccX = ICM_R_ACCEL_X();
		if(AccX)
		{
			/* 解析数据 */
			icm20608_ParseData(AccX, &result);
			LCD_ShowNum(0,LINE(0),AccX);
			LCD_ShowNum(0,LINE(1),result.angle_x);
			ICM_Show_Value();
		}
		HAL_Delay(50);
	}
}
#endif

void EXTI1_IRQHandler(void)
{

  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == GPIO_PIN_1)
		{
			BaseType_t xHigherPriorityTaskWoken = pdFALSE;
			xEventGroupSetBitsFromISR(g_xICM20608EventGroup , (1<<0) , &xHigherPriorityTaskWoken);
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}
    return;
}

