#ifndef _SYSTEM_Init_H
#define _SYSTEM_Init_H

#include "stm32f10x.h"

void Init_system(void);
ErrorStatus Init_RCC(void);
ErrorStatus Init_NVIC(void);
ErrorStatus Close_it(void);//�ر����裬����ֹ�жϡ�
ErrorStatus Init_GPIO(void);

#endif /*_SYSTEM_Init_H */
