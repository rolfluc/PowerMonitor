#include "UART.h"
#include "stm32g0xx_hal.h"
static const uint32_t uartPinMode = GPIO_MODE_AF_PP;
static const uint32_t uartPinPull = GPIO_NOPULL;
static const uint32_t uartPinSpeed = GPIO_SPEED_FREQ_HIGH;
UART_HandleTypeDef UartPort;

void UART4_IRQHandler()
{
	HAL_UART_IRQHandler(&UartPort);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	
}


void InitUart()
{
	__HAL_RCC_USART1_CLK_ENABLE();
	UART_InitTypeDef init;
	UartPort.Instance = USART1;
	init.BaudRate = 115200;
	init.WordLength = USART_WORDLENGTH_8B;
	init.StopBits = USART_STOPBITS_1;
	init.Parity = UART_PARITY_NONE;
	init.Mode = UART_MODE_TX;
	init.HwFlowCtl = UART_HWCONTROL_NONE;
	init.OverSampling = UART_OVERSAMPLING_8;
	init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	init.ClockPrescaler = UART_PRESCALER_DIV1;
	
	GPIO_InitTypeDef gpioinit;
	gpioinit.Speed = uartPinSpeed;
	gpioinit.Mode = uartPinMode;
	gpioinit.Pull = uartPinPull;
	gpioinit.Alternate = GPIO_AF0_USART1;
	gpioinit.Pin = GPIO_PIN_9;
	HAL_GPIO_Init(GPIOA, &gpioinit);
	
	//HAL_NVIC_SetPriority(UART4_IRQn, 5, 0);
	//HAL_NVIC_EnableIRQ(UART4_IRQn);
}

void TransferFrame(uint8_t* bytes)
{
	HAL_UART_Transmit(&UartPort, bytes, UART_FRAME_SIZE_bytes, 50);	
}