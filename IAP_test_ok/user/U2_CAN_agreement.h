#ifndef _U2_CAN_AGREEMENT_H
#define _U2_CAN_AGREEMENT_H

//V104

#include <stdio.h>	
#include "stm32f10x.h"


#define BROADCAST_CHANNEL ((0x0001<<7)|0x00)

#define IAP_ORDER_MISO_CHANNEL ((0x0003<<7)|Cob_ID_CAN1)
#define IAP_ORDER_MOSI_CHANNEL ((0x0004<<7)|Cob_ID_CAN1)

#define APP_ORDER_MISO_CHANNEL ((0x0005<<7)|Cob_ID_CAN1)
#define APP_ORDER_MOSI_CHANNEL ((0x0006<<7)|Cob_ID_CAN1)

#define APP_MISO_CHANNEL ((0x0007<<7)|Cob_ID_CAN1)
#define APP_MOSI_CHANNEL ((0x0008<<7)|Cob_ID_CAN1)

#define CODE_MISO_CHANNEL ((0x0009<<7)|Cob_ID_CAN1)
#define CODE_MOSI_CHANNEL ((0x000a<<7)|Cob_ID_CAN1)

#define DEBUG_MISO_CHANNEL ((0x000b<<7)|Cob_ID_CAN1)
#define DEBUG_MOSI_CHANNEL ((0x000c<<7)|Cob_ID_CAN1)


ErrorStatus Init_U2_CAN(void);
ErrorStatus U2_Put_CAN(unsigned int id, char string);
ErrorStatus U2_Printf_CAN(unsigned int id, char* string);

#endif /*_U2_CAN_AGREEMENT_H */
