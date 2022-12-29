#pragma once
#include <stdint.h>
void InitADCs();
void ReadADCS(uint16_t* current_counts, uint16_t* voltage_counts);
