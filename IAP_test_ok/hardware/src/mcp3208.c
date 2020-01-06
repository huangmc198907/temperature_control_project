#include "mcp3208.h"

//-----------------------------------
void mcp3208_spi_init(void)
{
/*	
	SPI_InitTypeDef SPI_Structure;	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);	 //Enable SPI2	
//---SPI2 Config---
	SPI_Structure.SPI_Direction			= SPI_Direction_2Lines_FullDuplex  ;	//2 Line FullDuplex
	SPI_Structure.SPI_Mode				= SPI_Mode_Master;		  //Master Mode
	SPI_Structure.SPI_DataSize			= SPI_DataSize_8b;
	SPI_Structure.SPI_CPOL				= SPI_CPOL_Low;
	SPI_Structure.SPI_CPHA				= SPI_CPHA_1Edge;//ÏÂ½µÑØ
	SPI_Structure.SPI_NSS				= SPI_NSS_Soft ;
	SPI_Structure.SPI_BaudRatePrescaler	= SPI_BaudRatePrescaler_64;
	SPI_Structure.SPI_FirstBit			= SPI_FirstBit_MSB;
//	SPI_Structure.SPI_CRCPolynomial		= SPI_CRCPolynomial
	SPI_Init(SPI2,&SPI_Structure);
	SPI_Cmd(SPI2,ENABLE);
*/
	
	
	} 	



//----------------------------------- 
void delay(u32 cnt)
{
	u32 i = 0;
	for(i=cnt; i>0; i--){}	
}

uint16_t MCP320x_Config(uint8_t CHN_Num)
{
	uint16_t Data_Deal=0;
	SPI2_NSS_OFF();
	delay(5);
	SPI2_NSS_ON();
	SPI_I2S_SendData(SPI2,0x18+CHN_Num);
	if(SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_RXNE) == RESET)
	{;}
	delay(1000);
	SPI_I2S_SendData(SPI2,0x00);
	if(SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_RXNE) == RESET)
	{;}
	Data_Deal	= SPI2->DR;
	delay(1000);
	SPI_I2S_SendData(SPI2,0x00);
	if(SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_RXNE) == RESET)
	{;}
	Data_Deal	= (((Data_Deal<<8)|SPI2->DR)>>2)&0x0FFF;
//	Data_Deal	= ((SPI1->DR<<8)&0x0F00)|Data_Deal;	 
	delay(1000);
	return Data_Deal;
}

