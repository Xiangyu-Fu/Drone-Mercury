#ifndef _MATHS_H_
#define _MATHS_H_
#include "stm32f10x.h"
#include "struct.h"

/******************************************************************************
							宏定义
*******************************************************************************/ 
#define Pi									 3.1415927f
#define Radian_to_Angle	   	57.2957795f
#define RawData_to_Angle		 0.0610351f	//以下参数对应2000度每秒
#define RawData_to_Radian		 0.0010653f

/******************************************************************************
							全局函数声明
*******************************************************************************/ 
void Get_Radian(struct _gyro *Gyro_in,struct _SI_float *Gyro_out);	
uint8_t Get_Checksum(uint8_t mydata[]);
float invSqrt(float x);

#endif
