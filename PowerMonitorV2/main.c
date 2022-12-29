#include <STM32G0xx_hal.h>
#include <stm32_hal_legacy.h>
#include "LEDs.h"
#include "UART.h"
#include "SPI.h"
#include "ADC.h"
#include "External/SST25VF080B/SST25VF080B.h"
#include "Sizing.h"

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void Delay_ms(uint32_t ms)
{
	HAL_Delay(ms);
}

static bool DetermineOperating_ReadData()
{
	//TODO This needs to be able to determine if it is pluigged into a PC, or if connected to a 
	//battery.
	return false;
}

extern void SetupGPIOs()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	
	GPIO_InitStructure.Pin = GPIO_PIN_7; //~Hold
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = GPIO_PIN_11; //~WP
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

extern void SetHold(bool turnHigh)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, turnHigh ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

extern void SetWP(bool turnHigh)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, turnHigh ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static void PerformPCMode()
{
	//First, erase the whole chip.
	SS25_Erase(0, Size_Chip);
	uint16_t sampleBuffer[2] = { 0, 0 };
	//8mb, = 1MB, each sample is 4 bytes.
	//262,144 samples, @ 50hz means it fills up in 87 minutes.
	for (uint32_t addr = 0; addr < NUMBER_READS_PER_DEVICE; addr += 4)
	{
		ReadADCS(&sampleBuffer[0], &sampleBuffer[1]);
		SS25_Write(addr, (uint8_t*)sampleBuffer, sizeof(sampleBuffer));
		HAL_Delay(49);
	}
}

static void PrintData(uint16_t voltageCount, uint16_t currentCount)
{
	uint8_t writeBuffer[UART_FRAME_SIZE_bytes];
	writeBuffer[0] = (uint8_t)(voltageCount >> 8);
	writeBuffer[0] = (uint8_t)(voltageCount);
	writeBuffer[0] = ',';
	writeBuffer[0] = (uint8_t)(currentCount >> 8);
	writeBuffer[0] = (uint8_t)(currentCount);
	writeBuffer[0] = ',';
	TransferFrame(writeBuffer);
}

static void PerformReadMode()
{
	static uint16_t ReadBuffer[2] = {0,0};
	int newlineCounter = 0;
	//Reading 4 bytes at a time, and print it out.
	for (uint32_t addr = 0; addr < (1 << 12); addr += 4)
	{
		SS25_Read(addr, (uint8_t*)ReadBuffer, sizeof(ReadBuffer));
		PrintData(ReadBuffer[1], ReadBuffer[0]);
		newlineCounter += 1;
		//Just need some newlines. Likely tune a bit.
		if (newlineCounter % 16 == 0)
		{
			FinishLine();
		}
	}
	
	bool modetoggle = false;
	for (;;)
	{
		HAL_Delay(100);
		ToggleHB();
		TurnMode(modetoggle);
		modetoggle = !modetoggle;
	}
}

int main(void)
{
	static bool IsPCConnected = false;
	HAL_Init();
	__GPIOA_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();
	__GPIOC_CLK_ENABLE();
	__GPIOD_CLK_ENABLE();
	InitSPI();
	InitUart();
	SS25_Init();
	InitADCs();
	InitLEDs();
	
	IsPCConnected = DetermineOperating_ReadData();
	TurnMode(IsPCConnected);
	
	if (IsPCConnected)
	{
		PerformPCMode();
	}
	else
	{
		PerformReadMode();
	}
}
