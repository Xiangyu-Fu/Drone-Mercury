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

uint16_t I2C_Erro = 0;

static const char *TAG = "main";


void BSP_Init(void)
{
	TIM3_Init(1000);																// Enable Timer
	USART_Config(115200);														// Initialise UART
	STM_LOGI(TAG, "Drone Mercury Start ...");

  LED_Init();																			// Enable onboard LED Indicator
	ADCx_Init();  																	// Initialise battery voltage measure     
	MOTOR_Init();																		// Initialise motor PWM driver	
	SPI_NRF24L01_Init(40, TX);											// Initialise and configure NRF24L01 		
	while(InitMPU6050() == 0);											// Initialise MPU6050 and waiting for response

	//Cal_FilteringCoe();

	//FLASH_Unlock();
	//EEPROM_Init();
	
	Bsp_Init_OK = 1;  															// Sign of all Initialisation finished
}

void Task_1000HZ(void)
{
	Debug1_H;
	if(MPU6050_SequenceRead == 0)
	{
		I2C_Erro ++;
		// MPU6050_SingleRead();
	}
	MPU6050_Compose();
	

	Debug1_L;
}

void Task_500HZ(void)
{
	Debug2_H;

	Debug2_L;
}

void Task_250HZ(void)
{
	Debug3_H;

	Debug3_L;
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
		Delay(200);
		STM_LOGI(TAG, "TEST");
		
		LED3_OFF;
		Delay(200);
	}	
}
