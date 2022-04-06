#include "stm32f10x.h"
#include "stdio.h" 
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_adc.h"

static uint16_t Battery;
uint16_t ADC_Value[BUFFER_SIZE];

static void delay(uint32_t count)
{
	u32 i;
	for(i=0;i<count;i++);
}

void Voltage_Printf(void)
{
	Battery = (float)(2.0f * ADC_Value[0]);
	printf("\r\n The current Voltage times 100 : %d V", Battery);
}

int main()
{
  LED_Init();	
	USART_Config(115200);
	ADCx_Init();
			
	Usart_SendString(USART1, "Drone Mercury USART testing .... \n");
	while(1)
	{
		LED2_ON;
		
		LED3_ON;
		delay(0xBFFFF);
		
		LED2_OFF;
		
		LED3_OFF;
		delay(0xBFFFF);	
		
		Voltage_Printf();
	}	
}
