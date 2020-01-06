#ifndef _JUMPAPP_H
#define _JUMPAPP_H

#include "stm32f10x.h"


#define BootloaderAddress     0x8000000
#define ApplicationAddress    0x8002000

typedef  void (*pFunction)(void);

void JumpAPP(uint32_t Address);

#endif /*_JUMPAPP_H */
