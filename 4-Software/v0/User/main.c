#include "stm32f10x.h"
#include "stdio.h" 
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_adc.h"
#include "bsp_PWM.h"
#include "bsp_MPU.h"
#include "bsp_NRF24.h"
#include "bsp_utility.h"
#include "bsp_log.h"
#include "bsp_TIM.h"

extern uint32_t MPU6050_Buffer[14];
static uint8_t data[32];
extern uint8_t Bsp_Init_OK;
extern uint32_t Sys_Clock;
extern uint32_t Timer3_Count, Count_1ms, Count_2ms, Count_4ms;

static const char *TAG = "main";

static void delay(uint32_t count)
{
	u32 i,j;
	for(i=0;i<count;i++)
	{
	 for(j=0;j<8500;j++);
	}
	
}

void BSP_Init(void)
{
	// Initialise UART
	USART_Config(115200);
	STM_LOGI(TAG, "Drone Mercury Start ...");

	TIM3_Init(1000);

	// Enable onboard LED Indicator
  LED_Init();

	// Initialise battery voltage measure
	ADCx_Init();  
          
	// Initialise motor PWM driver
	MOTOR_Init();

	// Initialise and configure NRF24L01 
	SPI_NRF24L01_Init(40, TX);
	
	// Initialise MPU6050 and waiting for response
	while(InitMPU6050() == 0);

	//Cal_FilteringCoe();

	//FLASH_Unlock();
	//EEPROM_Init();
	
	Bsp_Init_OK = 1;
}

int main()
{
  BSP_Init();
	while(1)
	{
    // Surrounding LEDs Blink
		LED3_ON;
		//Voltage_Printf();
		MPU6050_SequenceRead();
		MPU6050_Compose();
		//MPU6050_Print_USART();
		NRF_Send_TX(data, 32);
		delay(200);
		STM_LOGI(TAG, "TEST");
		
		LED3_OFF;
		delay(200);
	}	
}
