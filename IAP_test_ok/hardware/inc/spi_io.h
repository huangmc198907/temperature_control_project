#ifndef __SPI_IO_H__
#define __SPI_IO_H__

#define MCP3204_8_CS GPIOA,GPIO_Pin_4
#define MCP3204_8_PORT SPI1


#define MOSI_H GPIO_SetBits(GPIOA, GPIO_Pin_7)
#define MOSI_L GPIO_ResetBits(GPIOA, GPIO_Pin_7)
#define SCLK_H GPIO_SetBits(GPIOA, GPIO_Pin_5)
#define SCLK_L GPIO_ResetBits(GPIOA, GPIO_Pin_5)
#define CS_H GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define CS_L GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define MISO GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6)


void Init_MCP3204_8(void);
unsigned short Read_MCP3204_8(unsigned char channel);


#endif


