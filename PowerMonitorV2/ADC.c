#include "ADC.h"
#include "stm32g0xx.h"

ADC_HandleTypeDef adcHandle;

static void SelectVoltage()
{
	ADC_ChannelConfTypeDef sConfig = { 0 };
	sConfig.Channel = ADC_CHANNEL_2;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_12CYCLES_5;
	if (HAL_ADC_ConfigChannel(&adcHandle, &sConfig) != HAL_OK)
	{
		//Error_Handler();
	}
}

static void SelectCurrent()
{
	ADC_ChannelConfTypeDef sConfig = { 0 };
	sConfig.Channel = ADC_CHANNEL_2;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_12CYCLES_5;
	if (HAL_ADC_ConfigChannel(&adcHandle, &sConfig) != HAL_OK)
	{
		//Error_Handler();
	}
}

void InitADCs()
{
	GPIO_InitTypeDef init;
	
	adcHandle.Instance = ADC1;
	adcHandle.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;
	adcHandle.Init.Resolution = ADC_RESOLUTION_12B;
	adcHandle.Init.ScanConvMode = ADC_SCAN_DISABLE;
	adcHandle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	adcHandle.Init.LowPowerAutoWait = DISABLE;
	adcHandle.Init.ContinuousConvMode = DISABLE;
	adcHandle.Init.NbrOfConversion = 1;
	adcHandle.Init.DiscontinuousConvMode = DISABLE;
	adcHandle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	adcHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	adcHandle.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	adcHandle.Init.OversamplingMode = DISABLE;
	if (HAL_ADC_Init(&adcHandle) != HAL_OK)
	{
		//Error_Handler();
	}
	
	HAL_ADC_MspInit(&adcHandle);
	
	init.Mode = GPIO_MODE_ANALOG;
	init.Pull = GPIO_NOPULL;
	init.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOA, &init);
	
	init.Pin = GPIO_PIN_1;
	HAL_GPIO_Init(GPIOA, &init);
}

void ReadADCS(uint16_t* current_counts, uint16_t* voltage_counts)
{
	SelectVoltage();
	HAL_ADC_Start(&adcHandle);
	HAL_ADC_PollForConversion(&adcHandle, 10000);
	*current_counts = (uint16_t)HAL_ADC_GetValue(&adcHandle);
	HAL_ADC_Stop(&adcHandle);
	
	SelectCurrent();
	HAL_ADC_Start(&adcHandle);
	HAL_ADC_PollForConversion(&adcHandle, 10000);
	*voltage_counts = (uint16_t)HAL_ADC_GetValue(&adcHandle);
	HAL_ADC_Stop(&adcHandle);
}