// board support package 
#include "bsp_led.h"

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// Clock Enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	//LED GPIOB5
	GPIO_InitStruct.GPIO_Pin = LED_Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED_GPIO, &GPIO_InitStruct);
}


