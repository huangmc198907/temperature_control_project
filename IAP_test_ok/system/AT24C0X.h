/*
该驱动支持器件：AT24C01C AT24C02C AT24C04C AT24C08C
*/

#ifndef _AT24C0X_H
#define _AT24C0X_H

#include <stdio.h>
#include "stm32f10x.h"

//选择其中一个器件
//#define AT24C01C
//#define AT24C02C
//#define AT24C04C
#define AT24C08C

#define AT24C0X_ADDRESS_STAR 0xa0
#define AT24C0X_ADDRESS 0x00


#define AT24C0X_WP GPIOB,GPIO_Pin_8
#define AT24C0X_PORT I2C1

#define AT24C0X_WP_H GPIO_SetBits(AT24C0X_WP)
#define AT24C0X_WP_L GPIO_ResetBits(AT24C0X_WP)


extern unsigned int out_time_IIC;

void Init_AT24C0X(void);
unsigned char Byte_Write_AT24C0X(unsigned char address, unsigned char date);

#endif /*_AT24C0X_H */
