#include "stm32f10x.h"
#include "ymodem.h"
#include "USART.h"
#include "CRC.h"
#include "CAN.H"

#define PUT_OUT_TIME 100
#define GET_OUT_TIME 100

#define Checkput_YMoDem() Checkput_CAN1()
#define Put_YMoDem(x) { while(!Checkput_YMoDem()); Put_CAN1(x);}
#define Printf_YMoDem(x) { printf_CAN1(x);}
#define Checkget_YMoDem() Checkget_CAN1()
#define Get_YMoDem(x) { while(!Checkget_YMoDem()); x=Get_CAN1();}


unsigned int out_time_YMoDem = 0;
struct data_YMoDem puts_date_YMoDem,gets_date_YMoDem;

uint8_t file_name_YMoDem[256];
uint16_t Size_YMoDem;

ErrorStatus Put_date_YMoDem(struct data_YMoDem* put_YMoDem)
{
	unsigned int number_teep;
	unsigned int crc_teep = 0;
	unsigned char* date_teep;
	
	switch(put_YMoDem->commed)
	{
		case MODEM_SOH : 
		case MODEM_STX : 
											for(out_time_YMoDem = PUT_OUT_TIME; out_time_YMoDem; )
												if(Checkput_YMoDem()) {Put_YMoDem(put_YMoDem->commed); break;}
											if(!out_time_YMoDem) return ERROR;
											for(out_time_YMoDem = PUT_OUT_TIME; out_time_YMoDem; )
												if(Checkput_YMoDem()) {Put_YMoDem(put_YMoDem->number); break;}
											if(!out_time_YMoDem) return ERROR;
											for(out_time_YMoDem = PUT_OUT_TIME; out_time_YMoDem; )
												if(Checkput_YMoDem()) {Put_YMoDem(0xff-put_YMoDem->number); break;}
											if(!out_time_YMoDem) return ERROR;
												
											date_teep = put_YMoDem->date;
											for(number_teep = 0; number_teep < ((put_YMoDem->commed==MODEM_SOH)?128:1024); number_teep++)
											{
												UpdateCRC16(crc_teep, *date_teep);
												
												for(out_time_YMoDem = PUT_OUT_TIME; out_time_YMoDem; )
													if(Checkput_YMoDem()) {Put_YMoDem(*date_teep); break;}
												if(!out_time_YMoDem) return ERROR;
													
												date_teep++;
											}
										  crc_teep = UpdateCRC16(crc_teep,0);
										  crc_teep = UpdateCRC16(crc_teep,0);
										  crc_teep = crc_teep&0xffffu;
											
											for(out_time_YMoDem = PUT_OUT_TIME; out_time_YMoDem; )
												if(Checkput_YMoDem()) {Put_YMoDem(crc_teep>>8); break;}
											if(!out_time_YMoDem) return ERROR;
											for(out_time_YMoDem = PUT_OUT_TIME; out_time_YMoDem; )
												if(Checkput_YMoDem()) {Put_YMoDem(crc_teep); break;}
											if(!out_time_YMoDem) return ERROR;
											
											break;
		case MODEM_EOT : 
		case MODEM_ACK : 
		case MODEM_NAK : 
		case MODEM_CAN : 
		case MODEM_C   : 
											Put_YMoDem(put_YMoDem->commed); 
											break;
		default				 :	return ERROR;
	}
	
	return SUCCESS;
}

unsigned char Get_date_YMoDem(struct data_YMoDem* get_YMoDem)
{
	unsigned char un_number = 0;
	unsigned int number_teep;
	unsigned int crc_teep = 0;
	unsigned int crcH_teep2 = 0;
	unsigned int crcL_teep2 = 0;
	unsigned char* date_teep;
	
	get_YMoDem->commed = 0;
	for(out_time_YMoDem = GET_OUT_TIME; out_time_YMoDem; )
		if(Checkget_YMoDem()) {Get_YMoDem(get_YMoDem->commed); break;}
	if(!out_time_YMoDem) return 1;//³¬Ê±´íÎó

	switch(get_YMoDem->commed)
	{
		case MODEM_SOH : 
		case MODEM_STX : 
											for(out_time_YMoDem = GET_OUT_TIME; out_time_YMoDem; )
												if(Checkget_YMoDem()) {Get_YMoDem(get_YMoDem->number); break;}
											if(!out_time_YMoDem) return 1;
											for(out_time_YMoDem = GET_OUT_TIME; out_time_YMoDem; )
												if(Checkget_YMoDem()) {Get_YMoDem(un_number); break;}
											if(!out_time_YMoDem) return 1;
												
											date_teep = get_YMoDem->date;
											for(number_teep = 0; number_teep < ((get_YMoDem->commed==MODEM_SOH)?128:1024); number_teep++)
											{
												for(out_time_YMoDem = GET_OUT_TIME; out_time_YMoDem; )
													if(Checkget_YMoDem()) {Get_YMoDem(*date_teep); break;}
												if(!out_time_YMoDem) return 1;
													
												crc_teep = UpdateCRC16(crc_teep, *date_teep);
												date_teep++;
											}
											
											for(out_time_YMoDem = GET_OUT_TIME; out_time_YMoDem; )
												if(Checkget_YMoDem()) {Get_YMoDem(crcH_teep2); break;}
											if(!out_time_YMoDem) return 1;
											for(out_time_YMoDem = GET_OUT_TIME; out_time_YMoDem; )
												if(Checkget_YMoDem()) {Get_YMoDem(crcL_teep2); break;}
											if(!out_time_YMoDem) return 1;
												
											crc_teep = UpdateCRC16(crc_teep,0);
										  crc_teep = UpdateCRC16(crc_teep,0);
										  crc_teep = crc_teep&0xffffu;
											
											if((0xff == (get_YMoDem->number+un_number))&&(crc_teep == ((crcH_teep2<<8)|(crcL_teep2&0x00ff)))) 
												return 0;
											else return 2;//Ğ£Ñé´íÎó
		case MODEM_EOT : 
		case MODEM_ACK : 
		case MODEM_NAK : 
		case MODEM_CAN : 
		case MODEM_C   : 
											return 0;
		default				 :	return 3;//Î´Öª´íÎó
	}
}
