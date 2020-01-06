#include <stdio.h>
#include "stm32f10x.h"
#include "MCP3204_8.H"


void Init_MCP3204_8(void)
{
	SPI_InitTypeDef SPI_InitStruct;
	GPIO_InitTypeDef GPIO_init_structure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO|RCC_APB2Periph_SPI1, ENABLE);
	//sck mosi 
	GPIO_init_structure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_7;
	GPIO_init_structure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_init_structure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_init_structure);
	//MISO
//	GPIO_init_structure.GPIO_Pin = GPIO_Pin_6;
//	GPIO_init_structure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOA,&GPIO_init_structure);
	//nss
	GPIO_init_structure.GPIO_Pin = GPIO_Pin_4;
	GPIO_init_structure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&GPIO_init_structure);

	
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_InitStruct.SPI_CRCPolynomial= 7;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	
  SPI_Init(MCP3204_8_PORT,&SPI_InitStruct);
	
	SPI_Cmd(MCP3204_8_PORT,ENABLE);
	
	MCP3204_8_CS_H;
}


/*
unsigned int Read_MCP3204_8(unsigned char channel)
{
	unsigned int teep;
	
 	SPI_I2S_ReceiveData( MCP3204_8_PORT);
	MCP3204_8_CS_L;
//	SPI_I2S_SendData(SPI1, 0x18|(channel));
		SPI_I2S_SendData(SPI1, 0x05);
//	while(SPI_I2S_GetFlagStatus( MCP3204_8_PORT, SPI_I2S_FLAG_RXNE) != SET);
//	teep = SPI_I2S_ReceiveData( MCP3204_8_PORT);
//	teep <<= 8;
	SPI_I2S_SendData(SPI1, 0x00);
	while(SPI_I2S_GetFlagStatus( MCP3204_8_PORT, SPI_I2S_FLAG_RXNE) != SET);
	teep |= SPI_I2S_ReceiveData( MCP3204_8_PORT);
	teep <<= 8;
	SPI_I2S_SendData(SPI1, 0x00);
	while(SPI_I2S_GetFlagStatus( MCP3204_8_PORT, SPI_I2S_FLAG_RXNE) != SET);
	MCP3204_8_CS_H;
	teep |= SPI_I2S_ReceiveData( MCP3204_8_PORT);
	teep >>= 5;
	teep &= 0x0fff;
	
	return teep;
}
*/