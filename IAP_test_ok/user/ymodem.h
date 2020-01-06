#ifndef _YMODEM_H
#define _YMODEM_H

#include "stm32f10x.h"	

#define MODEM_SOH  0x01     //128
#define MODEM_STX  0x02     //1024
#define MODEM_EOT  0x04     //结束
#define MODEM_ACK  0x06     //应答
#define MODEM_NAK  0x15     //错误
#define MODEM_CAN  0x18     //取消
#define MODEM_NUL  0x00     //空字符
#define MODEM_C    0x43     //启动

extern unsigned int out_time_YMoDem;
extern uint8_t file_name_YMoDem[256];
extern uint32_t Size_YMoDem;
extern struct data_YMoDem puts_date_YMoDem,gets_date_YMoDem;

struct data_YMoDem
{
	unsigned char commed;
	unsigned char number;
	unsigned char date[1024];
};

ErrorStatus Put_date_YMoDem(struct data_YMoDem* put_YMoDem);
unsigned char Get_date_YMoDem(struct data_YMoDem* get_YMoDem);

#endif /*_YMODEM_H */
