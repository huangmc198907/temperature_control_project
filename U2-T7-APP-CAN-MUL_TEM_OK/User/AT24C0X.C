#include <stdio.h>
#include "stm32f10x.h"
#include "AT24C0X.H"

#define TIME_DELAY_AT24C0X 200

unsigned int Time_AT24C0X;

void Init_AT24C0X(void)
{
	I2C_InitTypeDef I2C_InitStruct;
	
	I2C_DeInit(AT24C0X_PORT);
	I2C_InitStruct.I2C_ClockSpeed = 100000;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	
	I2C_Cmd(AT24C0X_PORT, ENABLE);
	I2C_Init(AT24C0X_PORT, &I2C_InitStruct);
	
	AT24C0X_WP_H;
}

void Byte_Write_AT24C0X(unsigned int address, unsigned char date)
{
	unsigned int device_address = (AT24C0X_ADDRESS_STAR<<7)|(AT24C0X_ADDRESS<<8)|address;
	
	AT24C0X_WP_L;
	
	I2C_GenerateSTART(AT24C0X_PORT, ENABLE);
	Time_AT24C0X = TIME_DELAY_AT24C0X;
	while((!I2C_CheckEvent(AT24C0X_PORT, I2C_EVENT_MASTER_MODE_SELECT))&&Time_AT24C0X) if(Time_AT24C0X) Time_AT24C0X--;
	I2C_Send7bitAddress(AT24C0X_PORT, device_address>>7, I2C_Direction_Transmitter);
	Time_AT24C0X = TIME_DELAY_AT24C0X;
	while((!I2C_CheckEvent(AT24C0X_PORT, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))&&Time_AT24C0X) if(Time_AT24C0X) Time_AT24C0X--;
	I2C_SendData(AT24C0X_PORT, device_address);
	Time_AT24C0X = TIME_DELAY_AT24C0X;
	while((!I2C_CheckEvent(AT24C0X_PORT, I2C_EVENT_MASTER_BYTE_TRANSMITTED))&&Time_AT24C0X) if(Time_AT24C0X) Time_AT24C0X--;
	I2C_SendData(AT24C0X_PORT, date);
	Time_AT24C0X = TIME_DELAY_AT24C0X;
	while((!I2C_CheckEvent(AT24C0X_PORT, I2C_EVENT_MASTER_BYTE_TRANSMITTED))&&Time_AT24C0X) if(Time_AT24C0X) Time_AT24C0X--;
	I2C_GenerateSTOP(AT24C0X_PORT, ENABLE);
	
	AT24C0X_WP_H;
}

//»Œ“‚µÿ÷∑∂¡
unsigned char Random_Read_AT24C0X(unsigned int address)
{
	unsigned int device_address = (AT24C0X_ADDRESS_STAR<<7)|(AT24C0X_ADDRESS<<8)|address;
	unsigned char teep;

	I2C_AcknowledgeConfig(AT24C0X_PORT, DISABLE);
	I2C_GenerateSTART(AT24C0X_PORT, ENABLE);
	Time_AT24C0X = TIME_DELAY_AT24C0X;
	while((!I2C_CheckEvent(AT24C0X_PORT, I2C_EVENT_MASTER_MODE_SELECT))&&Time_AT24C0X) if(Time_AT24C0X) Time_AT24C0X--;
	I2C_Send7bitAddress(AT24C0X_PORT, device_address>>7, I2C_Direction_Transmitter);
	Time_AT24C0X = TIME_DELAY_AT24C0X;
	while((!I2C_CheckEvent(AT24C0X_PORT, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))&&Time_AT24C0X) if(Time_AT24C0X) Time_AT24C0X--;
	I2C_SendData(AT24C0X_PORT, device_address);
	Time_AT24C0X = TIME_DELAY_AT24C0X;
	while((!I2C_CheckEvent(AT24C0X_PORT, I2C_EVENT_MASTER_BYTE_TRANSMITTED))&&Time_AT24C0X) if(Time_AT24C0X) Time_AT24C0X--;
	I2C_GenerateSTART(AT24C0X_PORT, ENABLE);
	Time_AT24C0X = TIME_DELAY_AT24C0X;
	while((!I2C_CheckEvent(AT24C0X_PORT, I2C_EVENT_MASTER_MODE_SELECT))&&Time_AT24C0X) if(Time_AT24C0X) Time_AT24C0X--;
	I2C_Send7bitAddress(AT24C0X_PORT, device_address>>7, I2C_Direction_Receiver);
	Time_AT24C0X = TIME_DELAY_AT24C0X;
	while((!I2C_CheckEvent(AT24C0X_PORT, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))&&Time_AT24C0X) if(Time_AT24C0X) Time_AT24C0X--;
	I2C_GenerateSTOP(AT24C0X_PORT, ENABLE);
	Time_AT24C0X = TIME_DELAY_AT24C0X;
	while(Time_AT24C0X) if(Time_AT24C0X) Time_AT24C0X--;
	teep = I2C_ReceiveData(AT24C0X_PORT);
	
	return teep;
}

unsigned char Byte_Write_check_AT24C0X(unsigned int address, unsigned char date)
{
	unsigned int teep;
	
	if(date == Random_Read_AT24C0X(address)) return date;
	
	Byte_Write_AT24C0X(address,date);
	
	for(teep=50000;teep;teep--);
	
	return Random_Read_AT24C0X(address);
}
