#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include "stm32f10x.h"

#define ADC_DMA_CHANNEL 		DMA1_Channel1
#define ADCx						  ADC1

#define BUFFER_SIZE 32

void ADCx_Init(void);
void Voltage_Printf(void);

#endif
