#include "bsp_adc.h"

extern uint16_t ADC_Value[BUFFER_SIZE];

static void ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_GPIOB, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

static void ADC1_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	/*-----DMA Channel 1 Configuration-----*/
	DMA_DeInit(ADC_DMA_CHANNEL);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_Value;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);
	
	DMA_Cmd(ADC_DMA_CHANNEL, ENABLE);
		
	/*-----ADC1 Configuration-----*/
	ADC_DeInit(ADCx);
	ADC_TempSensorVrefintCmd(ENABLE);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = BUFFER_SIZE;
	ADC_Init(ADCx, &ADC_InitStructure);
	
	// RCC_ADCCLKConfig(RCC_PCLK2_Div8);          CLK?
	
	// ADC Sampling Order Configuration
	ADC_RegularChannelConfig(ADCx, ADC_Channel_9, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADCx, ADC_Channel_9, 1, ADC_SampleTime_239Cycles5);
	
	// Enable ADC
	ADC_DMACmd(ADCx, ENABLE);	
	//ADC_ITConfig(ADCx, ADC_IT_EOC, ENABLE);
	ADC_Cmd(ADCx, ENABLE);
	ADC_ResetCalibration(ADCx);
	
	// Reset and Set ADC
	while(ADC_GetResetCalibrationStatus(ADCx));
	
	ADC_StartCalibration(ADCx);
	
	while(ADC_GetCalibrationStatus(ADCx));
	
	ADC_SoftwareStartConvCmd(ADCx, ENABLE);
}


void ADCx_Init(void)
{
	ADC1_GPIO_Config();
  ADC1_Mode_Config();
}
