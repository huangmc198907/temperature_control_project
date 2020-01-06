#ifndef _PT100_H
#define _PT100_H

#include "stdio.h"
#include "stm32f10x.h"	

extern const unsigned int table_pt100[];

int PT100_Temp(float mr);
float PT100_Temp_f(float mrs);
unsigned char PT100_Temp_s(float mrs, char* s);

#endif /*_PT100_H */
