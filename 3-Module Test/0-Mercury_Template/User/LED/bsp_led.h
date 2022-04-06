#ifndef __BSP_LED_H  
#define __BSP_LED_H

#include "stm32f10x.h"

#define LED_GPIO 	GPIOB
#define LED_Pin    GPIO_Pin_3

#define LED_ON 		LED_GPIO->BRR	= LED_Pin
#define LED_OFF    LED_GPIO->BSRR = LED_Pin
               

void LED_Init(void);

#endif /*__BSP_LED_H*/

