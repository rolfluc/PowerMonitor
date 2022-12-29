#include "LEDs.h"
#include "stm32g0xx_hal.h"
static bool isHBON = false;
	
void InitLEDs()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	
	GPIO_InitStructure.Pin = GPIO_PIN_15;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void TurnHB(bool isOn)
{
	isHBON = isOn;
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, isOn ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void ToggleHB()
{
	isHBON = !isHBON;
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, isHBON ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void TurnMode(bool isOn)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, isOn ? GPIO_PIN_SET : GPIO_PIN_RESET);
}