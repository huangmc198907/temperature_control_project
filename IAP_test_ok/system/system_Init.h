#ifndef _SYSTEM_Init_H
#define _SYSTEM_Init_H

#include "stm32f10x.h"

void Init_system(void);
ErrorStatus Init_RCC(void);
ErrorStatus Init_NVIC(void);
ErrorStatus Close_it(void);//关闭外设，并禁止中断。
ErrorStatus Init_GPIO(void);

#endif /*_SYSTEM_Init_H */
