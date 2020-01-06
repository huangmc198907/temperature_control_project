#ifndef _CAN_H
#define _CAN_H

#define USED_CAN1

#include "stm32f10x.h"

#define PUTBUFF_CAN1 0x0010
#define GETBUFF_CAN1 0x0080

struct CAN_Mail
{
	unsigned int ID;
	unsigned char number;
	unsigned char dates[8];
};

#if defined USED_CAN1
extern uint16_t Cob_ID_CAN1;
extern uint16_t puttop_CAN1;
extern uint16_t putend_CAN1;
extern uint16_t putall_CAN1;
extern struct CAN_Mail putbuff_CAN1[PUTBUFF_CAN1];
extern uint16_t gettop_CAN1;
extern uint16_t getend_CAN1;
extern uint16_t getall_CAN1;
extern struct CAN_Mail getbuff_CAN1[GETBUFF_CAN1];
#endif


#if defined USED_CAN1
ErrorStatus Init_CAN1(void);
void Clearput_CAN1(void);
uint16_t Checkput_CAN1(void);
ErrorStatus Put_CAN1(struct CAN_Mail* put_CAN1);
void Clearget_CAN1(void);
uint16_t Checkget_CAN1(void);
ErrorStatus Get_CAN1(struct CAN_Mail* get_CAN1);
#endif

#endif /*_CAN_H */
