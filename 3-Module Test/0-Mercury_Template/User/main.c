#include "stm32f10x.h"
#include "bsp_led.h" 

void Delay(u32 count)
{
	u32 i;
	for(i=0; i<count;i++);
}

int main()
{	
	LED_Init();
	
	while(1)
	{
		LED_OFF;
		Delay(0xFFFFF);
		LED_ON;
		Delay(0xFFFFF);
	}
}
