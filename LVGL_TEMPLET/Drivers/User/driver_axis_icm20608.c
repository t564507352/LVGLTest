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
//6�ᴫ������ר�ö�д������
//��һ�������ǶԼĴ����Ƕ�����д����ʱ���Ϻ궨��,�ڶ���������ָ������Ҫ���Ĵ����ʹ���dummy
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
//��ʼ��
void ICM_Init(void)
{
	//RESET
	ICM_20608_RW(ICM_PWR_MGMT_1|ICM_WRITE,0x80);
	HAL_Delay(100);
	//ѡ��ʱ�ӣ��ر�˯��ģʽ,��оƬ�¶Ȳ�������
	ICM_20608_RW(ICM_PWR_MGMT_1|ICM_WRITE,0x00);
	HAL_Delay(50);
	//FIFO_EN��ʧ��FIFO
	ICM_20608_RW(ICM_FIFO_EN|ICM_WRITE,0x00);
	HAL_Delay(50);
	//���������ǵ�3db�˲�20HZ
	ICM_20608_RW(ICM_CONFIG|ICM_WRITE,0x04);
	HAL_Delay(50);
	//���������ǵ�������Ϊ����2000dps��FCHOICE_BΪ00
	ICM_20608_RW(ICM_GYRO_CONFIG|ICM_WRITE,0x18);
	HAL_Delay(50);
	//���ü��ټ�����Ϊ����16g
	ICM_20608_RW(ICM_ACCEL_CONFIG|ICM_WRITE,0x18);
	HAL_Delay(50);
	//���ü��ټ�FCHOICE_BΪ00��3db��ͨ�˲�����Ϊ21.2
	ICM_20608_RW(ICM_ACCEL_CONFIG2|ICM_WRITE,0x04);
	HAL_Delay(50);
	//���ò�����
	ICM_20608_RW(ICM_SMPLRT_DIV|ICM_WRITE,0x00);
	HAL_Delay(50);
	//ʧ�������ж�
	ICM_20608_RW(ICM_INT_ENABLE|ICM_WRITE,0x00);
	HAL_Delay(50);
	//�����Ǽ��ټ�6��ȫ��
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
	//===============����Ϊ�ֲ�Ҫ��Ĳ���==================
	//ѡ��ʱ�ӣ��ر�˯��ģʽ,��оƬ�¶Ȳ�������
	ICM_20608_RW(ICM_PWR_MGMT_1|ICM_WRITE,0x00);
	HAL_Delay(50);
	//�����ǹ�  ���ټƿ�
	ICM_20608_RW(ICM_PWR_MGMT_2|ICM_WRITE,0x07);
	HAL_Delay(50);
	//���ü��ټ�FCHOICE_BΪ01
	ICM_20608_RW(ICM_ACCEL_CONFIG2|ICM_WRITE,0x09);
	HAL_Delay(50);
	//ʹ���˶��ж�
	ICM_20608_RW(ICM_INT_ENABLE|ICM_WRITE,0xE0);
	HAL_Delay(50);
	//�˶��ж���ֵ����
	ICM_20608_RW(ACCEL_WOM_THR|ICM_WRITE,0x40);//���о��£�����ֵ
	HAL_Delay(50);
	//�������ټ������жϣ���������һ��ֵ�Ա�
	ICM_20608_RW(ACCEL_INTEL_CTRL|ICM_WRITE,0xC0);
	HAL_Delay(50);
	//����
	//���ò�����1000/1+3=250
	ICM_20608_RW(ICM_SMPLRT_DIV|ICM_WRITE,0x03);
	HAL_Delay(50);	
	
	//����оƬ��Ƶ�����
	ICM_20608_RW(ICM_LP_MODE_CFG|ICM_WRITE,0x0B);//0x0B->���  0x09->100��
	HAL_Delay(50);
	//���ü��ټƿ���ѭ��ģʽ
	ICM_20608_RW(ICM_PWR_MGMT_1|ICM_WRITE,0x20);//0x20����ѭ��  0x00�ر�
	HAL_Delay(50);
	//================����Ϊ�ֲ�Ҫ��Ĳ���================
	
//	//�����ж�����
//	ICM_20608_RW(INT_PIN_CFG|ICM_WRITE,0x10);//0x10,�жϺ����ŵ�ƽ����50us�Զ��ָ��ߣ�0x30 �ж�״̬����ȡ���ڻָ��ߵ�ƽ
//	HAL_Delay(50);	
	//���ü��ټ�����Ϊ����16g
//	ICM_20608_RW(ICM_ACCEL_CONFIG|ICM_WRITE,0x18);
//	HAL_Delay(50);	
//	//FIFO_EN��ʧ��FIFO
//	ICM_20608_RW(ICM_FIFO_EN|ICM_WRITE,0x00);
//	HAL_Delay(50);
//	//���������ǵ�3db�˲�20HZ
//	ICM_20608_RW(ICM_CONFIG|ICM_WRITE,0x04);
//	HAL_Delay(50);
//	//���������ǵ�������Ϊ����2000dps��FCHOICE_BΪ00
//	ICM_20608_RW(ICM_GYRO_CONFIG|ICM_WRITE,0x18);
//	HAL_Delay(50);

//	//���ò�����
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
	
	
	//�¶Ȼ�ȡ��ת��
	//  TEMP_degC = ((TEMP_OUT �C RoomTemp_Offset)/Temp_Sensitivity) + 25degC
	//  RoomTemp_Offset = 25��C
	//	Temp_Sensitivity = 326.8 LSB/��C
	tempReadValue = ICM_R_Temp();
	tempActValue =  ((tempReadValue - 25)/326.8) + 25;
	
	
	//���ټ���ֵת��
	//15bit�ܱ�ʾ�����ֵΪ32767������Ŀ���ټ���������Ϊ16
	//ʵ��ֵ�ķ���ʽΪ32767/16=ReadValue/ActValue,��ActValue=ReadValue/2047.93
	//���ټ�X��ȡ��ת��
	accelReadValueX = ICM_R_ACCEL_X();
	accelActValueX = accelReadValueX/2048;
	//���ټ�Y��ȡ��ת��
	accelReadValueY = ICM_R_ACCEL_Y();
	accelActValueY = accelReadValueY/2048;
	//���ټ�Z��ȡ��ת��
	accelReadValueZ = ICM_R_ACCEL_Z();
	accelActValueZ = accelReadValueZ/2048;
	
	
	//���ټ���ֵת��
	//15bit�ܱ�ʾ�����ֵΪ32767������Ŀ��������������Ϊ2000
	//ʵ��ֵ�ķ���ʽΪ32767/2000=ReadValue/ActValue,��ActValue=ReadValue/16.38
	//������X��ȡ��ת��
	gyroReadValueX = ICM_R_GYRO_X();
	gyroActValueX = gyroReadValueX/16;
	//������Y��ȡ��ת��
	gyroReadValueY = ICM_R_GYRO_Y();
	gyroActValueY = gyroReadValueY/16;
	//������Z��ȡ��ת��
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

//���������Ƕ��к���
void creatICM20608DataQueue(void)
	{
		g_xICM20608DataQueue = xQueueCreate(g_xICM20608DataQueueSize, sizeof(struct icm20608_data) );//IRtimer�ж��õĻ���buffer���ݶ���
	}

//�����������жϺͲ�ѯ���������õ��¼��麯��
void createICM20608EventGroup(void)
	{
		g_xICM20608EventGroup = xEventGroupCreate();
	}

//������������ת�ǶȺ���
void icm20608_ParseData(int16_t AccX, struct icm20608_data *result)
{
	if (result)
	{
		result->angle_x = (int32_t)(acos((double)((double)(AccX + ICM20608_X_ACCEL_OFFSET) / 16384.0)) * 57.29577);
	}
}

//��ȡ���ļ���ȫ��static���о��
QueueHandle_t getICM20608DataQueue(void)
{
	return g_xICM20608DataQueue;
}

//��ʱ�������������ݲ�д��������
void ICM20608GetValue_Task(void *params)
{	
  int16_t AccX;
	struct icm20608_data result;
	while (1)
	{  
	//�ȴ��¼���
		xEventGroupWaitBits(g_xICM20608EventGroup , (1<<0) , pdTRUE , pdTRUE , portMAX_DELAY);
	/* ������ */
		do
		{
			AccX = ICM_R_ACCEL_X();
			if(AccX)
			{
				/* �������� */
				icm20608_ParseData(AccX, &result);

				/* д���� */
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
	/* ������ */
		AccX = ICM_R_ACCEL_X();
		if(AccX)
		{
			/* �������� */
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

