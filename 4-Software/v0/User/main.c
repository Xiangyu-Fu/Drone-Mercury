#include "stm32f10x.h"
#include "stdio.h" 
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_adc.h"
#include "bsp_PWM.h"
#include "bsp_MPU.h"
#include "bsp_NRF24.h"


extern uint32_t MPU6050_Buffer[14];
static uint8_t data[32];

static void delay(uint32_t count)
{
	u32 i,j;
	for(i=0;i<count;i++)
	{
	 for(j=0;j<8500;j++);
	}
	
}

int main()
{
  LED_Init();
	LED2_ON; // Chip Indicator LED
	
	
	USART_Config(115200);
	printf("\r\n /////////////// DRONE  MERCURY ///////////////////");
	printf("\r\n USART	Initialisation finished ... ");
	
	ADCx_Init();            
	MOTOR_Init();
	delay(10);
	InitMPU6050();
	delay(10);
	SPI_NRF_Init();
	delay(10);	
	NRF24L01_Init(40, TX);
	NRF24L01_Check();
	
	while(1)
	{
    // Surrounding LEDs Blink
		LED3_ON;
		Voltage_Printf();
		MPU6050_SequenceRead();
		MPU6050_Compose();
		MPU6050_Print_USART();
		NRF_Send_TX(data, 32);
		delay(30);
		
		LED3_OFF;
		delay(30);
	}	
}
