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


//��ʼ������Դ����,ʱ��ѡ�񣬲ɼ���ѡ������ѡ�񣬹ر��жϣ��ر�FIFO��ȫ�ɼ�ģʽ
#define ICM_PWR_MGMT_1												0x6B		//ʱ��ѡ���˯��ģʽѡ��оƬ�¶Ȳ������� 				 		    
#define ICM_PWR_MGMT_2												0x6C		//�����ǣ����ټƿ���																  	
#define ICM_SMPLRT_DIV												0x19    //����������Ϊ0x00Ϊ���
#define ICM_CONFIG														0x1A    //�����ǵ�DLPF_CFG�˲�������Ϊ4
#define ICM_GYRO_CONFIG												0x1B    //�������̶ܿȺ�FCHOICE_B������FCHOICE_B����Ϊ00�������ʵļ��㹫ʽ��DLPF_CFG�˲����ò�������
#define ICM_ACCEL_CONFIG											0x1C		//���ü��ټƵ��̶ܿ�
#define ICM_ACCEL_CONFIG2											0x1D		//����ACCEL_FCHOICE_B�رգ�����A_DLPF_CFG����Ϊ4    	 
#define ICM_LP_MODE_CFG												0x1E		//�͹���ģʽ���������																	
#define ICM_FIFO_EN														0x23		//ʧ��FIFO		
#define ICM_INT_ENABLE												0x38		//�ж�  																							
#define ACCEL_WOM_THR													0x1F	 	//���ٶȼƻ����˶��жϵ���ֵ														
#define ACCEL_INTEL_CTRL											0x69   //���ټ����ܿ���  
#define INT_PIN_CFG														0x37   //�ж�pin����


//���ټ����ݶ�ȡ
#define ICM_ACCEL_XOUT_H											0x3B
#define ICM_ACCEL_XOUT_L											0x3C
#define ICM_ACCEL_YOUT_H											0x3D
#define ICM_ACCEL_YOUT_L											0x3E
#define ICM_ACCEL_ZOUT_H											0x3F
#define ICM_ACCEL_ZOUT_L											0x40


//оƬ�¶����ݶ�ȡ
#define ICM_TEMP_OUT_H												0x41
#define ICM_TEMP_OUT_L												0x42


//���������ݶ�ȡ
#define ICM_GYRO_XOUT_H												0x43
#define ICM_GYRO_XOUT_L												0x44
#define ICM_GYRO_YOUT_H												0x45
#define ICM_GYRO_YOUT_L												0x46
#define ICM_GYRO_ZOUT_H												0x47
#define ICM_GYRO_ZOUT_L												0x48

/* ��������������ֵ������оƬ�̶�������Ӳ�����е����� */
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
