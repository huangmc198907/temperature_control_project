#include "stm32f10x.h"
#include "ymodem.h"
#include "USART.h"
#include "CRC.h"
#include "CAN.H"
#include "U2_CAN_agreement.h"

#define PUT_OUT_TIME 1000
#define GET_OUT_TIME 1000


#define Checkput_YMoDem() Checkput_CAN1()
#define Put_YMoDem(x) { \
												put_CAN_YMoDem.ID = CODE_MISO_CHANNEL;\
												put_CAN_YMoDem.number = 1;\
												put_CAN_YMoDem.dates[0] = x;\
                        Put_CAN1(&put_CAN_YMoDem); \
                      }
#define Checkget_YMoDem() ((number_get_buff-number_get_buff2)||CheckgetID_CAN1(CODE_MOSI_CHANNEL))
#define Get_YMoDem(x) { \
	                      if(number_get_buff2<number_get_buff)\
												{\
												  x=data_get_buff[number_get_buff2];\
													number_get_buff2++;\
												}\
												else \
												{\
													number_get_buff2=0;\
												  if(ERROR != Get_CAN1( &get_CAN_YMoDem))\
												  if(CODE_MOSI_CHANNEL == get_CAN_YMoDem.ID)\
													{\
													  number_get_buff = get_CAN_YMoDem.number;\
												    data_get_buff[0] = get_CAN_YMoDem.dates[0];\
												    data_get_buff[1] = get_CAN_YMoDem.dates[1];\
												    data_get_buff[2] = get_CAN_YMoDem.dates[2];\
												    data_get_buff[3] = get_CAN_YMoDem.dates[3];\
												    data_get_buff[4] = get_CAN_YMoDem.dates[4];\
												    data_get_buff[5] = get_CAN_YMoDem.dates[5];\
												    data_get_buff[6] = get_CAN_YMoDem.dates[6];\
												    data_get_buff[7] = get_CAN_YMoDem.dates[7];\
													}\
													if(number_get_buff2<number_get_buff)\
													{\
														x=data_get_buff[number_get_buff2];\
														number_get_buff2++;\
													}\
												}\
                      }

unsigned int number_get_buff=0;
unsigned int number_get_buff2=0;
unsigned char data_get_buff[8];
											
struct CAN_Mail get_CAN_YMoDem,put_CAN_YMoDem;
unsigned int out_time_YMoDem = 0;
struct data_YMoDem puts_date_YMoDem,gets_date_YMoDem;

uint8_t file_name_YMoDem[256];
uint32_t Size_YMoDem;

ErrorStatus Put_date_YMoDem(struct data_YMoDem* put_YMoDem)
{
	unsigned int number_teep;
	unsigned int crc_teep = 0;
	unsigned char* date_teep;
	
	switch(put_YMoDem->commed)
	{
		case MODEM_SOH : 
		case MODEM_STX : 
											for(out_time_YMoDem = PUT_OUT_TIME; out_time_YMoDem; IWDG_ReloadCounter())
												if(Checkput_YMoDem()) {Put_YMoDem(put_YMoDem->commed); break;}
											if(!out_time_YMoDem) return ERROR;
											for(out_time_YMoDem = PUT_OUT_TIME; out_time_YMoDem; IWDG_ReloadCounter())
												if(Checkput_YMoDem()) {Put_YMoDem(put_YMoDem->number); break;}
											if(!out_time_YMoDem) return ERROR;
											for(out_time_YMoDem = PUT_OUT_TIME; out_time_YMoDem; IWDG_ReloadCounter())
												if(Checkput_YMoDem()) {Put_YMoDem(0xff-put_YMoDem->number); break;}
											if(!out_time_YMoDem) return ERROR;
												
											date_teep = put_YMoDem->date;
											for(number_teep = 0; number_teep < ((put_YMoDem->commed==MODEM_SOH)?128:1024); number_teep++)
											{
												UpdateCRC16(crc_teep, *date_teep);
												
												for(out_time_YMoDem = PUT_OUT_TIME; out_time_YMoDem; IWDG_ReloadCounter())
													if(Checkput_YMoDem()) {Put_YMoDem(*date_teep); break;}
												if(!out_time_YMoDem) return ERROR;
													
												date_teep++;
											}
										  crc_teep = UpdateCRC16(crc_teep,0);
										  crc_teep = UpdateCRC16(crc_teep,0);
										  crc_teep = crc_teep&0xffffu;
											
											for(out_time_YMoDem = PUT_OUT_TIME; out_time_YMoDem; IWDG_ReloadCounter())
												if(Checkput_YMoDem()) {Put_YMoDem(crc_teep>>8); break;}
											if(!out_time_YMoDem) return ERROR;
											for(out_time_YMoDem = PUT_OUT_TIME; out_time_YMoDem; IWDG_ReloadCounter())
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
	for(out_time_YMoDem = GET_OUT_TIME; out_time_YMoDem; IWDG_ReloadCounter())
		if(Checkget_YMoDem()) 
    {
		  Get_YMoDem(get_YMoDem->commed); 
		  break;
		}
		
	if(!out_time_YMoDem) return 1;//³¬Ê±´íÎó

	switch(get_YMoDem->commed)
	{
		case MODEM_SOH : 
		case MODEM_STX : 
											for(out_time_YMoDem = GET_OUT_TIME; out_time_YMoDem; IWDG_ReloadCounter())
												if(Checkget_YMoDem()) {Get_YMoDem(get_YMoDem->number); break;}
											if(!out_time_YMoDem) 
												return 1;
											for(out_time_YMoDem = GET_OUT_TIME; out_time_YMoDem; IWDG_ReloadCounter())
												if(Checkget_YMoDem()) {Get_YMoDem(un_number); break;}
											if(!out_time_YMoDem) 
												return 1;
												
											date_teep = get_YMoDem->date;
											for(number_teep = 0; number_teep < ((get_YMoDem->commed==MODEM_SOH)?128:1024); number_teep++)
											{
												for(out_time_YMoDem = GET_OUT_TIME; out_time_YMoDem; IWDG_ReloadCounter())
													if(Checkget_YMoDem()) {Get_YMoDem(*date_teep); break;}
												if(!out_time_YMoDem) 
													return 1;
													
//												crc_teep = UpdateCRC16(crc_teep, *date_teep);
												date_teep++;
											}
											
											for(out_time_YMoDem = GET_OUT_TIME; out_time_YMoDem; IWDG_ReloadCounter())
												if(Checkget_YMoDem()) {Get_YMoDem(crcH_teep2); break;}
											if(!out_time_YMoDem) 
												return 1;
											for(out_time_YMoDem = GET_OUT_TIME; out_time_YMoDem; IWDG_ReloadCounter())
												if(Checkget_YMoDem()) {Get_YMoDem(crcL_teep2); break;}
											if(!out_time_YMoDem) 
												return 1;
											
											crc_teep = Cal_CRC16_2( get_YMoDem->date, ((get_YMoDem->commed==MODEM_SOH)?128:1024));
												
//											crc_teep = UpdateCRC16(crc_teep,0);
//										  crc_teep = UpdateCRC16(crc_teep,0);
//										  crc_teep = crc_teep&0xffffu;
											
											if((0xff == (get_YMoDem->number+un_number))&&(crc_teep == ((crcH_teep2<<8)|(crcL_teep2&0x00ff)))) 
												return 0;
											else return 2;//Ð£Ñé´íÎó
		case MODEM_EOT : 
		case MODEM_ACK : 
		case MODEM_NAK : 
		case MODEM_CAN : 
		case MODEM_C   : 
											return 0;
		default				 :	return 3;//Î´Öª´íÎó
	}
}
