#ifndef _JUMPAPP_H
#define _JUMPAPP_H

#include "stm32f10x.h"

#define WRP0                  0x1ffff808
#define BootloaderAddress     0x08000000
#define ApplicationAddress    0x08006000
#define AllFlash              0x08026000

typedef  void (*pFunction)(void);

void JumpAPP(uint32_t Address);

#endif /*_JUMPAPP_H */
