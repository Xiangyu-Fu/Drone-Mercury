#include <stdio.h>
#include <stdarg.h>
#include "bsp_utility.h"

uint8_t Bsp_Init_OK = 0;
uint32_t Sys_Clock = 0;
uint32_t Timer3_Count = 0, Count_1ms = 0, Count_2ms = 0, Count_4ms = 0;

void Delay(uint32_t count)
{
	u32 i,j;
	for(i=0;i<count;i++)
	{
	 for(j=0;j<8500;j++);
	}
	
}