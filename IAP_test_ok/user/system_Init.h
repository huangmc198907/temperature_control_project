#ifndef _SYSTEM_Init_H
#define _SYSTEM_Init_H

#include "stm32f10x.h"

void Init_system(void);
void Init_RCC(void);
void Init_NVIC(void);
void Close_it(void);//关闭外设，并禁止中断。
void Init_GPIO(void);

#endif /*_SYSTEM_Init_H */
