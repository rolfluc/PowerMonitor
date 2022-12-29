#include "SPI.h"
#include "stm32g0xx.h"
static const uint32_t spiPinMode = GPIO_MODE_AF_PP;
static const uint32_t spiPinPull = GPIO_NOPULL;
static const uint32_t spiPinSpeed = GPIO_SPEED_FREQ_LOW;
SPI_HandleTypeDef hspi1;
static SPI_InitTypeDef init1;

int SPI_Write(uint8_t* bytes, uint32_t len)
{
	HAL_SPI_Transmit(&hspi1, bytes, len, 100);
	//TODO 
	return 0;
}

int SPI_Read(uint8_t* bytes, uint32_t len)
{
	HAL_SPI_Receive(&hspi1, bytes, len, 100);
	//TODO 
	return 0;
}

int SPI_Transfer(uint8_t* writeBytes, uint8_t* readBytes, uint32_t len)
{
	HAL_SPI_TransmitReceive(&hspi1, writeBytes, readBytes, len, 100);
	//TODO 
	return 0;
}


static void InitializeSPIPins()
{
	GPIO_InitTypeDef init;
	init.Speed = spiPinSpeed;
	init.Mode = spiPinMode;
	init.Pull = spiPinPull;
	
	init.Pin = GPIO_PIN_12; //CS
	HAL_GPIO_Init(GPIOB, &init);
	
	init.Alternate = GPIO_AF5_SPI1;
	init.Pin = GPIO_PIN_2; //MOSI
	HAL_GPIO_Init(GPIOA, &init);
	init.Pin = GPIO_PIN_5; //SCK
	HAL_GPIO_Init(GPIOA, &init);
	init.Pin = GPIO_PIN_6; //MISO
	HAL_GPIO_Init(GPIOA, &init);
}

void InitSPI()
{
	InitializeSPIPins();
	__SPI1_CLK_ENABLE();
	init1.Mode = SPI_MODE_MASTER;
	init1.Direction = SPI_DIRECTION_2LINES;
	init1.DataSize = SPI_DATASIZE_8BIT;
	init1.NSS = SPI_NSS_SOFT;
	init1.FirstBit = SPI_FIRSTBIT_MSB;
	init1.TIMode = SPI_TIMODE_DISABLE;
	init1.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	init1.CRCPolynomial = 0x0;
	init1.NSSPMode = SPI_NSS_PULSE_ENABLE;
	init1.CLKPolarity = SPI_POLARITY_HIGH;
	init1.CLKPhase = SPI_PHASE_2EDGE; 
	init1.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32; //TODO try to target 25 Mhz, maybe 24Mhz
	
	hspi1.Instance = SPI1;
	hspi1.Init = init1;
	if (HAL_SPI_Init(&hspi1) != HAL_OK)
	{
		//Error_Handler();
	}
}
