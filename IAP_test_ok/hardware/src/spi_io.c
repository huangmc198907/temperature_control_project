#include "spi_io.h"
#include "stm32f10x.h"
void Init_MCP3204_8(void)
{
	GPIO_InitTypeDef gpio_init_structure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	gpio_init_structure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;
	gpio_init_structure.GPIO_Mode =  GPIO_Mode_Out_PP;
	gpio_init_structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio_init_structure);

	gpio_init_structure.GPIO_Pin = GPIO_Pin_6;
	gpio_init_structure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;
	gpio_init_structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio_init_structure);	
	
	CS_H;
	SCLK_H;
	MOSI_L;
	
}


void delay_us(unsigned short i)
{
		while(i--) ;
}

unsigned short Read_MCP3204_8(unsigned char channel)
{
	unsigned char i;
	unsigned short temp = 0x0600; //0X0600-CH0 0X0640-CH1
	unsigned short result=0;
	
	CS_L;
	temp = temp | (channel << 6);
	SCLK_L;
	delay_us(200);
	for(i=0;i<10;i++){
		SCLK_H;
		delay_us(200);
		SCLK_L;
		if(temp&0x8000)
			MOSI_H;
		else
			MOSI_L;
		delay_us(200);
		temp <<= 1;
	}
	
	SCLK_H;
	delay_us(200);
	SCLK_L;
	delay_us(200);
	SCLK_H;
	delay_us(200);
	SCLK_L;
	delay_us(200);
	
	for(i=0;i<12;i++){
		result <<= 1;
		SCLK_H;
		if(MISO)
			result |= 0x01;
		delay_us(200);
		SCLK_L;
		delay_us(200);
	}
	CS_H;
	SCLK_H;
	MOSI_L;
	
	return result;
	
}
