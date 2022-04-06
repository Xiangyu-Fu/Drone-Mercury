#ifndef __BSP_PWM_H
#define __BSP_PWM_H

#include "stm32f10x.h"

#define MOTOR_GPIO  				GPIOA
#define MOTOR_GPIO_PIN 			GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3
#define MOTOR_GPIO_CLK			RCC_APB2Periph_GPIOA

#define MOTOR_TIM						TIM2

void MOTOR_Init(void);

#endif 
