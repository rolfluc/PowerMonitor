#pragma once
#include <stdint.h>
#include "Sizing.h"
#define UART_FRAME_SIZE_bytes NUMBER_BYTES_PER_PAGE
void InitUart();
void TransferFrame(uint8_t* bytes);