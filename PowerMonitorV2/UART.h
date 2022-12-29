#pragma once
#include <stdint.h>
#include "Sizing.h"
#define UART_FRAME_SIZE_bytes (2 + 2 + 2) //uint16,uint16,
void InitUart();
void TransferFrame(uint8_t* bytes);
void FinishLine();