#ifndef __BSP_MPU_H
#define __BSP_MPU_H

#include "stm32f10x.h"

#define SCL_Pin  			GPIO_Pin_10
#define SDA_Pin 			GPIO_Pin_11


#define SCL_H 				GPIOB->BSRR =  	SCL_Pin
#define SCL_L 				GPIOB->BRR 	=  	SCL_Pin
#define SDA_H 				GPIOB->BSRR =  	SDA_Pin
#define SDA_L 				GPIOB->BRR 	=  	SDA_Pin
#define SDA_Read 			GPIOB->IDR 	& 	SDA_Pin


#define Debug1_Pin		GPIO_Pin_5
#define Debug1_H			GPIOB->BSRR = 	Debug1_Pin
#define Debug1_L			GPIOB->BRR 	= 	Debug1_Pin

#define Debug2_Pin		GPIO_Pin_9
#define Debug2_H			GPIOB->BSRR = 	Debug2_Pin
#define Debug2_L			GPIOB->BRR 	= 	Debug2_Pin

#define Debug3_Pin		GPIO_Pin_8
#define Debug3_H			GPIOB->BSRR = 	Debug3_Pin
#define Debug3_L			GPIOB->BRR 	= 	Debug3_Pin


#endif 