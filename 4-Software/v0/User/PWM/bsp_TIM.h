#ifndef __BSP_TIM_H
#define __BSP_TIM_H

#include "stm32f10x.h"


#define            BASIC_TIM                   TIM3
#define            BASIC_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            BASIC_TIM_CLK               RCC_APB1Periph_TIM3
#define            BASIC_TIM_Period            (1000-1)
#define            BASIC_TIM_Prescaler         71
#define            BASIC_TIM_IRQ               TIM3_IRQn
#define            BASIC_TIM_IRQHandler        TIM3_IRQHandler

void TIM3_Init(uint16_t Handler_Frequency);


#endif

