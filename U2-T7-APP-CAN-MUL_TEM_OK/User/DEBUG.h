#ifndef _DEBUG_H
#define _DEBUG_H

#include "stm32f10x.h"

#define DEBUG_Port USART1

void Init_DEBUG(void);
void myprintf_string(char * string);
void my_debug_printf(unsigned char number, char * string);

#endif /*_DEBUG_H */
