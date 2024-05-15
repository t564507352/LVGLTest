#ifndef __XPT2046_H
#define __XPT2046_H
#include <stdbool.h>
#include "main.h"
#include <stdio.h>  
#include <string.h>
//#include "../lvgl/lvgl.h" //BENTONG CHANGE

#if 0
//bentong do it
typedef int16_t lv_coord_t;
typedef uint8_t lv_indev_state_t;
typedef struct {
    lv_coord_t x;
    lv_coord_t y;
} lv_point_t;

typedef struct {
    lv_point_t point; /**< For LV_INDEV_TYPE_POINTER the currently pressed point*/
    uint32_t key;     /**< For LV_INDEV_TYPE_KEYPAD the currently pressed key*/
    uint32_t btn_id;  /**< For LV_INDEV_TYPE_BUTTON the currently pressed button*/
    int16_t enc_diff; /**< For LV_INDEV_TYPE_ENCODER number of steps since the previous read*/

    lv_indev_state_t state; /**< LV_INDEV_STATE_REL or LV_INDEV_STATE_PR*/
} lv_indev_data_t;
enum { LV_INDEV_STATE_REL = 0, LV_INDEV_STATE_PR };

# endif

/******************************* XPT2046 引脚函数定义 ***************************/
/* XPT2046 触摸屏触摸信号指示引脚定义(不使用中断)*/
#define    XPT2046_PENIRQ_GPIO_PORT         GPIOF
#define    XPT2046_PENIRQ_GPIO_PIN          GPIO_PIN_6

//触屏信号有效电平
#define   XPT2046_PENIRQ_ActiveLevel        0
#define   XPT2046_PENIRQ_Read()             HAL_GPIO_ReadPin ( XPT2046_PENIRQ_GPIO_PORT, XPT2046_PENIRQ_GPIO_PIN )



/*XPT2046 触摸屏模拟SPI引脚定义*/
#define    XPT2046_SPI_CS_PIN		        GPIO_PIN_6
#define    XPT2046_SPI_CS_PORT		        GPIOE

#define	   XPT2046_SPI_CLK_PIN	            GPIO_PIN_10
#define    XPT2046_SPI_CLK_PORT	            GPIOF

#define	   XPT2046_SPI_MOSI_PIN	            GPIO_PIN_8
#define	   XPT2046_SPI_MOSI_PORT	        GPIOF

#define	   XPT2046_SPI_MISO_PIN	            GPIO_PIN_7
#define	   XPT2046_SPI_MISO_PORT	        GPIOF


#define   XPT2046_CS_ENABLE()               HAL_GPIO_WritePin(XPT2046_SPI_CS_PORT, XPT2046_SPI_CS_PIN, GPIO_PIN_RESET)    
#define   XPT2046_CS_DISABLE()              HAL_GPIO_WritePin(XPT2046_SPI_CS_PORT, XPT2046_SPI_CS_PIN, GPIO_PIN_SET)   
            
#define   XPT2046_CLK_HIGH()                HAL_GPIO_WritePin(XPT2046_SPI_CLK_PORT, XPT2046_SPI_CLK_PIN, GPIO_PIN_SET)   
#define   XPT2046_CLK_LOW()                 HAL_GPIO_WritePin(XPT2046_SPI_CLK_PORT, XPT2046_SPI_CLK_PIN, GPIO_PIN_RESET)  
            
#define   XPT2046_MOSI_1()                  HAL_GPIO_WritePin(XPT2046_SPI_MOSI_PORT, XPT2046_SPI_MOSI_PIN, GPIO_PIN_SET)
#define   XPT2046_MOSI_0()                  HAL_GPIO_WritePin(XPT2046_SPI_MOSI_PORT, XPT2046_SPI_MOSI_PIN, GPIO_PIN_RESET)  
            
#define   XPT2046_MISO()                    HAL_GPIO_ReadPin( XPT2046_SPI_MISO_PORT, XPT2046_SPI_MISO_PIN )



/******************************* XPT2046 触摸屏参数定义 ***************************/
//校准触摸屏时触摸坐标的AD值相差门限 
#define XPT2046_THRESHOLD_CalDiff           2               
                
#define XPT2046_CHANNEL_X 	                0x90 	          //通道X+的选择控制字	
#define XPT2046_CHANNEL_Y 	                0xd0	          //通道Y+的选择控制字

/*************** 触摸屏 结构体定义 ***************/
typedef struct
{
    float An;
    float Bn;
    float Cn;
    float Dn;
    float En;
    float Fn;
    float Divider;
}_touch_calibration;    /* 触摸屏校准因子 */

typedef struct
{
    uint32_t para_flag;
    uint32_t para_force;
    float dX_X;
    float dX_Y;
    float dX;
    float dY_X;
    float dY_Y;
    float dY;
}_touch_para;    /* 触摸屏校准参数 */

typedef struct
{
    // 负数表示没有新数据
    int16_t x;      /* 最新触摸坐标参数 */
    int16_t y;
    int16_t pre_x;  /* 上次触摸坐标参数 */
    int16_t pre_y;
}_touch_xy;         /* 触摸屏坐标参数 */

typedef enum
{
    TOUCH_RELEASE = 0,
    TOUCH_WAITING = 1,
    TOUCH_PRESSED = 2,
}_touch_state;     /* 触摸屏状态：释放、等待释放、按下 */

typedef enum
{
    NOT_FINISHED = 0,
    FINISHED = 1,
    
    NOT_FORCED = 2,
    FORCED = 3,
}_para_flag;

#define DURIATION_TIME                  (2)         // 触摸屏抖动消除阈值
#define TOUCH_DET_PRESSED 				1
#define TOUCH_DET_NOT_PRESSED			0

extern _touch_para                    touch_para[6];

extern void XPT2046_Init(void);
#if 0
extern bool xpt2046_read(/*lv_indev_drv_t * indev_drv, */lv_indev_data_t * data);//bentong do it
#endif
extern uint8_t XPT2046_Touch_Calibrate(uint8_t LCD_Mode);
bool xpt2046_is_pressed(void);

#endif /* __XPT2046_H */

