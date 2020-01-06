#ifndef _NTC_H
#define _NTC_H

#include "stdio.h"
#include "stm32f10x.h"	

extern const float table_10K_3435[];

unsigned char NTC_Temp_s(const float* tab, float mrs, char* s);

#endif /*_PT100_H */