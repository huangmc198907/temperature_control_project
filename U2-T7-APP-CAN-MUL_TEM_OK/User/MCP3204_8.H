/*
该驱动支持器件：MCP3204 MCP3208
*/

#ifndef _MCP3204_8_H
#define _MCP3204_8_H

#include <stdio.h>

//选择其中一个器件
//#define MCP3204
#define MCP3208

#define MCP3204_8_CS GPIOB,GPIO_Pin_12
#define MCP3204_8_PORT SPI2

#define MCP3204_8_CS_H GPIO_SetBits(MCP3204_8_CS)
#define MCP3204_8_CS_L GPIO_ResetBits(MCP3204_8_CS)

void Init_MCP3204_8(void);
unsigned int Read_MCP3204_8(unsigned char channel);

#endif /*MCP3204_8 */
