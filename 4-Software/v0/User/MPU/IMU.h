#ifndef __IMU_H
#define __IMU_H

#include "stm32f10x.h"
#include "struct.h"


#define Kp 			1.0f    		// ��������
#define Ki 			0.001f  		// ���ֳ���
#define halfT 	0.0005f			// ������
#define T				0.001f  		// ����Ϊ1ms

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);
void Get_Eulerian_Angle(struct _out_angle *angle);

#endif