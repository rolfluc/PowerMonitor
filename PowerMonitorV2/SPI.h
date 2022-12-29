#pragma once
#include <stdint.h>
void InitSPI();
int SPI_Write(uint8_t* bytes, uint32_t len);
int SPI_Read(uint8_t* bytes, uint32_t len);
int SPI_Transfer(uint8_t* writeBytes, uint8_t* readBytes, uint32_t len);