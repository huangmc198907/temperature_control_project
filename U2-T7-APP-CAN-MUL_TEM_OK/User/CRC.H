#ifndef _CRC_H
#define _CRC_H

#include "stm32f10x.h"	

uint16_t UpdateCRC16(uint16_t crcIn, uint8_t byte);
uint16_t Cal_CRC16(const uint8_t* data, uint32_t size);

#endif /*_CRC_H */
