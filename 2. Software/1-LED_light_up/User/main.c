#include "stm32f10x.h" 
#include "bsp_led.h"


static void delay(uint32_t count)
{
	u32 i;
	for(i=0;i<count;i++);
}

int main()
{
  LED_Init();
	while(1)
	{
		  LED2_ON;
			delay(0xFFFFF);
			
			LED3_ON;
			delay(0xFFFFF);
			
			LED2_OFF;
			delay(0xFFFFF);
			
			LED3_OFF;
			delay(0xFFFFF);			
	}
	
}
