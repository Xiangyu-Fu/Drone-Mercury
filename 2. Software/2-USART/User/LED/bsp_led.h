#ifndef __BSP_LED_H  
#define __BSP_LED_H

#include "stm32f10x.h"

#define LED2_GPIO 	GPIOB
#define LED2_Pin    GPIO_Pin_3
#define LED3_GPIO		GPIOC
#define LED3_Pin		GPIO_Pin_13

#define LED2_ON 		LED2_GPIO->BRR	= LED2_Pin
#define LED2_OFF    LED2_GPIO->BSRR = LED2_Pin
#define LED3_ON 		LED3_GPIO->BSRR	= LED3_Pin
#define LED3_OFF    LED3_GPIO->BRR  = LED3_Pin                  

void LED_Init(void);

#endif /*__BSP_LED_H*/

