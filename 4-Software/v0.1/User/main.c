#include "stm32f10x.h"
#include "stdio.h" 
#include "bsp_NRF24.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_adc.h"
#include "bsp_PWM.h"
#include "bsp_MPU.h"


static uint16_t Battery;
uint16_t ADC_Value[BUFFER_SIZE];
extern uint32_t MPU6050_Buffer[14];
uint8_t NRF24L01_RXDATA[32];
//static uint8_t data[32];

static void delay(uint32_t count)
{
	u32 i,j;
	for(i=0;i<count;i++)
	{
	 for(j=0;j<8500;j++);
	}
	
}


void Voltage_Printf(void)
{
	Battery = (uint16_t)(2.0f*ADC_Value[0]/ADC_Value[1]*1.2f*100);
	printf("\r\n The current Voltage times 100 : %d V", Battery);
}

int main()
{
  LED_Init();
	LED2_ON; // Chip Indicator LED
	
	USART_Config(115200);
	printf("\r\n /////////////// DRONE  MERCURY ///////////////////");
	Usart_SendString(USART1, "\r\n Drone Mercury USART Start Running .... \n");
	
	ADCx_Init();
	MOTOR_Init();
	
	I2C2_GPIO_Config();
	InitMPU6050();
	
	SPI_NRF_Init();
	NRF24L01_Init(40, RX);
	NRF24L01_Check();
	
	while(1)
	{
		Debug1_H;
		delay(20);

    // Surrounding LEDs Blink
		LED3_ON;
		//delay(0xBFFFF);
		
		
		//delay(0xBFFFF);	
		
		// Voltage_Printf();
		//MPU6050_SequenceRead();
		//MPU6050_Compose();
		//MPU6050_Print_USART();
		Debug1_L;

		NRF24L01_Test();
		//NRF24L01_print_reg();
		delay(20);
		LED3_OFF;

		
	}	
}
