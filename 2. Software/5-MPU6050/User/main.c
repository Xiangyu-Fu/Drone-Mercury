#include "stm32f10x.h"
#include "stdio.h" 
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_adc.h"
#include "bsp_PWM.h"
#include "bsp_MPU.h"

static uint16_t Battery;
uint16_t ADC_Value[BUFFER_SIZE];
extern uint32_t MPU6050_Buffer[14];

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
	LED2_ON; // Chip Indicator LED
	
	USART_Config(115200);
	printf("\r\n /////////////// DRONE  MERCURY ///////////////////");
	Usart_SendString(USART1, "\r\n Drone Mercury USART Start Running .... \n");
	
	// ADCx_Init();
	I2C2_GPIO_Config();
	//MOTOR_Init();			
	
	InitMPU6050();
	while(1)
	{
		Debug1_H;
		delay(0xFFFFF);

    // Surrounding LEDs Blink
		LED3_ON;
		//delay(0xBFFFF);
		
		LED3_OFF;
		//delay(0xBFFFF);	
		
		// Voltage_Printf();
		MPU6050_SequenceRead();
		MPU6050_Compose();
		MPU6050_Print_USART();		
		Debug1_L;
		delay(0xFFFFF);
	}	
}
