#ifndef __CONTROL_H
#define __CONTROL_H

#include "stm32f10x.h"
#include "Struct.h"
/******************************************************************************
							ȫ�ֱ�������
*******************************************************************************/ 
extern uint8_t Rc_Lock;//1������0����

/******************************************************************************
							ȫ�ֺ�������
*******************************************************************************/ 
void RC_LOCK(void);
void RC_Limit(struct _Rc *rc);
void Control_Angle(struct _out_angle *angle,struct _Rc *rc);
void Control_Gyro(struct _SI_float *gyro,struct _Rc *rc,uint8_t Lock);

#endif