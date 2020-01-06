#ifndef __MCP3208_H__
#define __MCP3208_H__

#include "stm32f10x_spi.h"

#define SPI1_NSS		4			//PA1
#define SPI2_NSS		12			//PB12

#define SPI1_NSS_OFF()				(GPIOA->BSRR =1<<(SPI1_NSS))										
#define SPI1_NSS_ON()				(GPIOA->BRR  =1<<(SPI1_NSS))
#define SPI2_NSS_OFF()				(GPIOB->BSRR =1<<(SPI2_NSS))										
#define SPI2_NSS_ON()				(GPIOB->BRR  =1<<(SPI2_NSS))



void mcp3208_spi_init(void);
uint16_t MCP320x_Config(uint8_t CHN_Num);

#endif