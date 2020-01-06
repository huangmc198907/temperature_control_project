#include <stdio.h>
#include "stm32f10x.h"
#include "AT24C0X.H"

#define PUT_OUT_TIME_IIC 10

unsigned int out_time_IIC = 0;

void Init_AT24C0X(void)
{
	I2C_InitTypeDef I2C_InitStruct;
	
//	I2C_SoftwareResetCmd(AT24C0X_PORT, ENABLE);
	
	I2C_DeInit(AT24C0X_PORT);
	I2C_InitStruct.I2C_ClockSpeed = 400000;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	
	I2C_Cmd(AT24C0X_PORT, ENABLE);
	I2C_Init(AT24C0X_PORT, &I2C_InitStruct);
	
//	I2C_SoftwareResetCmd(AT24C0X_PORT, DISABLE);
	
	AT24C0X_WP_H;
}

unsigned char Byte_Write_AT24C0X(unsigned char address, unsigned char date)
{
	unsigned int device_address = (AT24C0X_ADDRESS_STAR<<7)|(AT24C0X_ADDRESS<<8)|address;
	
	AT24C0X_WP_L;
	
	I2C_AcknowledgeConfig(AT24C0X_PORT, ENABLE);
	for(out_time_IIC = PUT_OUT_TIME_IIC; out_time_IIC; IWDG_ReloadCounter())
	if(SET != I2C_GetFlagStatus(AT24C0X_PORT, I2C_FLAG_BUSY)) break;
	if(!out_time_IIC) return 1;
	
	I2C_GenerateSTART(AT24C0X_PORT, ENABLE);
	for(out_time_IIC = PUT_OUT_TIME_IIC; out_time_IIC; IWDG_ReloadCounter())
	if(I2C_CheckEvent(AT24C0X_PORT, I2C_EVENT_MASTER_MODE_SELECT)) break;
	if(!out_time_IIC) return 2;
	
	I2C_Send7bitAddress(AT24C0X_PORT, device_address>>7, I2C_Direction_Transmitter);
	for(out_time_IIC = PUT_OUT_TIME_IIC; out_time_IIC; IWDG_ReloadCounter())
	if(I2C_CheckEvent(AT24C0X_PORT, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) break;
	if(!out_time_IIC) return 3;
	
	I2C_SendData(AT24C0X_PORT, date);
	for(out_time_IIC = PUT_OUT_TIME_IIC; out_time_IIC; IWDG_ReloadCounter())
	if(I2C_CheckEvent(AT24C0X_PORT, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) break;
	if(!out_time_IIC) return 4;
	
	I2C_GenerateSTOP(AT24C0X_PORT, ENABLE);
	for(out_time_IIC = PUT_OUT_TIME_IIC; out_time_IIC; IWDG_ReloadCounter())
	if(SET != I2C_GetFlagStatus(AT24C0X_PORT, I2C_FLAG_BUSY)) break;
	if(!out_time_IIC) return 5;
	
	I2C_AcknowledgeConfig(AT24C0X_PORT, DISABLE);
	I2C_ClearFlag(AT24C0X_PORT, I2C_FLAG_AF);
	
	AT24C0X_WP_H;
	
	return 0;
}
