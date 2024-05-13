#ifndef __DRIVER_IRDA_H
#define __DRIVER_IRDA_H

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "typedefs.h"

#define g_xIRRxDataQueueSize 128
//irda��
//����IRDA���ݽṹ��
typedef struct
{
//    uint8_t pilot_code;  // �������־
//    uint8_t serial_code; // �������־
//    uint16_t period;     // ��ǰһ�ε���������
    uint32_t value;      // ��ַ��+��ַ����+������+�����(32 Bit) 
}IRDA_DATA;

//���尴����Ӧ������ֵ(��Դң��������)
typedef enum
{
    OPEN = 0x45,
    MENU = 0x47,
    TEST = 0x44,
    INCREASE = 0x40,
    BACK = 0x43,
    GOBACK = 0x07,
    PLAY = 0x15,
    FASTFORWARD = 0x09,
    ZERO = 0x16,
    DECREASE = 0x19,
    CANCEL = 0x0D,
    ONE = 0x0C,
    TWO = 0x18,
    THREE = 0x5E,
    FOUR = 0x08,
    FIVE = 0x1C,
    SIX = 0x5A,
    SEVEN = 0x42,
    EIGHT = 0x52,
    NINE = 0x4A
}KEY_CODE;


//����ң����Ū�����
//typedef enum
//{
//		OK = 0x15,
//    UP = 0x16,
//		DOWN = 0x17,
//		RIGHT = 0x18,
//		LEFT = 0x19,
//		SWITCH = 0x0d
//}KEY_CODE;
void IRReceiver_Test(void);
void registerDispatchQueue(QueueHandle_t queue);
void creatIRRxDataQueue(void);
QueueHandle_t getIRRxDataQueue(void);
//��ʼ����IR�ź�
void startIRInputCapture(void);

#endif /* __DRIVER_IRDA_H */
