#ifndef __BSP_UTILITY_H
#define __BSP_UTILITY_H

#include "stm32f10x.h"

typedef int stm_err_t;


int16_t Battery_Fly;

#define STM_OK          0      
#define STM_FAIL        -1  

#define Debug1_Pin		GPIO_Pin_5
#define Debug1_H			GPIOB->BSRR = 	Debug1_Pin
#define Debug1_L			GPIOB->BRR 	= 	Debug1_Pin

#define Debug2_Pin		GPIO_Pin_9
#define Debug2_H			GPIOB->BSRR = 	Debug2_Pin
#define Debug2_L			GPIOB->BRR 	= 	Debug2_Pin

#define Debug3_Pin		GPIO_Pin_8
#define Debug3_H			GPIOB->BSRR = 	Debug3_Pin
#define Debug3_L			GPIOB->BRR 	= 	Debug3_Pin

void Delay(uint32_t count);

#endif 
