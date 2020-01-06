#include <stdio.h>
#include "stm32f10x.h"
#include "EEPROM_Management.H"

/*Ó²¼þÏà¹Ø*/
#include "AT24C0X.H"
#define EEPROM_Read(x) Random_Read_AT24C0X(x)
#define EEPROM_Write(x,y) Byte_Write_AT24C0X(x,y)

ErrorStatus Random_EEPROM_Read(unsigned int address, unsigned char* dates, unsigned int length)
{
	unsigned char buff;
	unsigned int length_teep;
	
	for(length_teep = 0; length_teep < length; length_teep++)
	{
		dates[length_teep] = EEPROM_Read(address+length_teep);
		buff = EEPROM_Read(address+length+length_teep);
		buff = ~buff;
		if(dates[length_teep] != buff)
		{
			for(length_teep = 0; length_teep < length; length_teep++)
				dates[length_teep] = 0xff;
			return ERROR;
		}
	}
	
	return SUCCESS;
}

ErrorStatus Random_EEPROM_Write(unsigned int address, unsigned char* dates, unsigned int length)
{
	unsigned int length_teep;
	unsigned int teep;
	
	for(teep=50000;teep;teep--);
	for(length_teep = 0; length_teep < length; length_teep++)
	{
		EEPROM_Write(address+length_teep, dates[length_teep]);
		for(teep=50000;teep;teep--);
		EEPROM_Write(address+length+length_teep, ~dates[length_teep]);
		for(teep=50000;teep;teep--);
	}
	
	return SUCCESS;
}
