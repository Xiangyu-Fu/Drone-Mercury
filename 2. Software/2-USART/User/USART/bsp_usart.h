#ifndef __BSP_USART_H
#define __BSP_USART_H


#include "stm32f10x.h"

#define USART_GPIO_CLK  (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)
#define USART_CLK        RCC_APB2Periph_USART1

#define  USART_TX_GPIO_PORT       GPIOA   
#define  USART_TX_GPIO_PIN        GPIO_Pin_9
#define  USART_RX_GPIO_PORT       GPIOA
#define  USART_RX_GPIO_PIN        GPIO_Pin_10


void USART_Config(u32 baudrate);
void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch);
void Usart_SendHalfWord(USART_TypeDef* pUSARTx, uint16_t data); 
void Usart_SendString(USART_TypeDef *pUSARTx, char *str);  

#endif
