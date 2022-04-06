#include "bsp_PWM.h"
#include "stdio.h"

static void TIM2_Init_Config(void)
{
	
	// Open TIM Clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	/*-----Time Base structure Initialisation------*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 1000 - 1; // ARR
	TIM_TimeBaseStructure.TIM_Prescaler = 4 - 1; // PSC
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(MOTOR_TIM, &TIM_TimeBaseStructure);
	
	/*-----Output Comparision structure Initialisation------*/
	
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	
	TIM_OCInitStructure.TIM_Pulse = 00;
	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	// TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	// TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	
	TIM_OC1Init(MOTOR_TIM, &TIM_OCInitStructure);	
	TIM_OC1PreloadConfig(MOTOR_TIM, TIM_OCPreload_Enable);
	
	TIM_OC2Init(MOTOR_TIM, &TIM_OCInitStructure);	
	TIM_OC2PreloadConfig(MOTOR_TIM, TIM_OCPreload_Enable);
	
	TIM_OC3Init(MOTOR_TIM, &TIM_OCInitStructure);	
	TIM_OC3PreloadConfig(MOTOR_TIM, TIM_OCPreload_Enable);
	
	TIM_OC4Init(MOTOR_TIM, &TIM_OCInitStructure);	
	TIM_OC4PreloadConfig(MOTOR_TIM, TIM_OCPreload_Enable);
	
	/*-----break and dead-time register structure Initialisation------*/
//	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
//	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
//  TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
//  TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;
//	
//	TIM_BDTRInitStructure.TIM_DeadTime = 20;  // dead-time configure
//	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
//	
//	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
//	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
//	TIM_BDTRConfig(MOTOR_TIM, &TIM_BDTRInitStructure);
	
	
	// Enable the Counter
	TIM_Cmd(MOTOR_TIM, ENABLE);
}

static void TIM2_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// Enable clock
	RCC_APB2PeriphClockCmd(MOTOR_GPIO_CLK, ENABLE);
	
	// GPIO configure
	GPIO_InitStructure.GPIO_Pin = MOTOR_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(MOTOR_GPIO, &GPIO_InitStructure);
}

void MOTOR_Init(void)
{
	TIM2_GPIO_Config();
	TIM2_Init_Config();
	printf("\r\n PWM	Initialisation finished ...");
}
