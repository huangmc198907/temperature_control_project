#ifndef _USART_H
#define _USART_H

#define USED_USART2

#include "stm32f10x.h"


#define PUTBUFFS_USART1 0x0400
#define GETBUFFS_USART1 0x0400

#define PUTBUFFS_USART2 0x0800
#define GETBUFFS_USART2 0x0800

#define PUTBUFFS_USART3 0x0800
#define GETBUFFS_USART3 0x0800

#if defined USED_USART1
extern uint16_t puttop_USART1;
extern uint16_t putend_USART1;
extern uint16_t putall_USART1;
extern uint8_t putbuff_USART1[PUTBUFFS_USART1];
extern uint16_t gettop_USART1;
extern uint16_t getend_USART1;
extern uint16_t getall_USART1;
extern uint8_t getbuff_USART1[GETBUFFS_USART1];
#endif

#if defined USED_USART2
extern uint16_t puttop_USART2;
extern uint16_t putend_USART2;
extern uint16_t putall_USART2;
extern uint8_t putbuff_USART2[PUTBUFFS_USART2];
extern uint16_t gettop_USART2;
extern uint16_t getend_USART2;
extern uint16_t getall_USART2;
extern uint8_t getbuff_USART2[GETBUFFS_USART2];
#endif

#if defined USED_USART3
extern uint16_t puttop_USART3;
extern uint16_t putend_USART3;
extern uint16_t putall_USART3;
extern uint8_t putbuff_USART3[PUTBUFFS_USART3];
extern uint16_t gettop_USART3;
extern uint16_t getend_USART3;
extern uint16_t getall_USART3;
extern uint8_t getbuff_USART3[GETBUFFS_USART3];
#endif


#if defined USED_USART1
ErrorStatus Init_USART1(void);
void Clearput_USART1(void);
uint16_t Checkput_USART1(void);
ErrorStatus Put_USART1(uint8_t data);
void Clearget_USART1(void);
uint16_t Checkget_USART1(void);
uint8_t Get_USART1(void);
ErrorStatus printf_USART1(char* string);
#endif

#if defined USED_USART2
ErrorStatus Init_USART2(void);
void Clearput_USART2(void);
uint16_t Checkput_USART2(void);
ErrorStatus Put_USART2(uint8_t data);
void Clearget_USART2(void);
uint16_t Checkget_USART2(void);
uint8_t Get_USART2(void);
ErrorStatus printf_USART2(char* string);
#endif

#if defined USED_USART3
ErrorStatus Init_USART3(void);
void Clearput_USART3(void);
uint16_t Checkput_USART3(void);
ErrorStatus Put_USART3(uint8_t data);
void Clearget_USART3(void);
uint16_t Checkget_USART3(void);
uint8_t Get_USART3(void);
ErrorStatus printf_USART3(char* string);
#endif

#endif /*_USART_H */
