#ifndef _STRUCT_ALL_H_
#define _STRUCT_ALL_H_
#include "stm32f10x.h"

#include "Led.h"
#include "Uart.h" 
#include "Motor.h"
#include "Timer3.h"
#include "DMA_ADC.h"
#include "Protocol.h"
#include "I2C_MPU6050.h"
#include "SPI_NRF24L01.h"

/******************************************************************************
							宏定义
*******************************************************************************/
#define Lock_Mode (1<<0)//锁尾
#define Led_Mode  (1<<1)//夜间模式

/******************************************************************************
							全局函数声明
*******************************************************************************/ 
void Nvic_Init(void);

void EEPROM_INIT(void);
void EEPROM_SAVE_ACC_OFFSET(void);
void EEPROM_READ_ACC_OFFSET(void);
void EEPROM_SAVE_GYRO_OFFSET(void);
void EEPROM_READ_GYRO_OFFSET(void);
void EEPROM_SAVE_PID(void);
void EEPROM_READ_PID(void);
void PID_Reset(void);
/******************************************************************************
							结构体声明
*******************************************************************************/ 
/* MPU6050--加速度计结构体 */
struct _acc
{
	int16_t x;
	int16_t y;
	int16_t z;
};
extern struct _acc acc;
extern struct _acc filter_acc;
extern struct _acc offset_acc;

/* MPU6050--陀螺仪结构体 */
struct _gyro
{
	int16_t x;
	int16_t y;
	int16_t z;
};
extern struct _gyro gyro;
extern struct _gyro filter_gyro;
extern struct _gyro offset_gyro;

/* float结构体 */
struct _SI_float
{
	float x;
	float y;
	float z;
};
extern struct _SI_float SI_acc;	
extern struct _SI_float SI_gyro;

/* 姿态解算--角度值 */
struct _out_angle
{
	float yaw;
	float roll;
	float pitch;
};
extern struct _out_angle out_angle;

/* 遥控数据 */
struct _Rc
{
	uint16_t THROTTLE;
	uint16_t YAW;
	uint16_t PITCH;
	uint16_t ROLL;
	
	uint16_t AUX1;
	uint16_t AUX2;
	uint16_t AUX3;
	uint16_t AUX4;
};
extern struct _Rc Rc;

/* pid变量 */
struct _pid
{
	float kp;
	float ki;
	float kd;
	float integral;
	
	float output;
};
extern struct _pid pitch;
extern struct _pid roll;
extern struct _pid gyro_pitch;
extern struct _pid gyro_roll;
extern struct _pid gyro_yaw;

#endif