#ifndef __BSP_PWM_H
#define __BSP_PWM_H

#include "stm32f10x.h"

#define MOTOR_GPIO  				GPIOA
#define MOTOR_GPIO_PIN 			GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3
#define MOTOR_GPIO_CLK			RCC_APB2Periph_GPIOA

#define MOTOR_TIM						TIM2

void MOTOR_Init(void);
void Motor_Out(int16_t duty1,int16_t duty2,int16_t duty3,int16_t duty4);

#endif 
