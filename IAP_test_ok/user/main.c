#include "stdio.h"
#include <string.h>
#include "stm32f10x.h"	 
#include "main.h"
#include "system_Init.h"
#include "USART.h"
#include "IntToString.h"
#include "JumpAPP.H"
#include "CAN.H"
#include "U2_CAN_agreement.h"
#include "ymodem.h"
#include "AT24C0X.H"


//#define printf_getbuff getbuff_USART1
//#define printf_Checkget Checkget_USART1
//#define printf_Clearget Clearget_USART1
//#define printf_Get Get_USART1

#define WAIT_TIME 10 //小于等于

unsigned int time_1ms_main;
unsigned char error_number = 0;
unsigned char CAN_DEBUG = 0;

ErrorStatus Ymodem(void); 
void SystemReset(void);
void printf_string(char* string);															
void printf_Put(char string);

int main(void)
{
	unsigned int time_1ms_main_teep = 0;
	struct CAN_Mail get_CAN_teep,put_CAN_teep;
	char string_buff[128];
	
	Init_system();
	IWDG_ReloadCounter();

	while(1) 
	{
		IWDG_ReloadCounter();
		
		printf_string("Reset   : There is IAP.\n");
		printf_string("Version : V10.2\n");
		printf_string("Time    : 20190623\n");
		printf_string("*************************************************\n");
		printf_string("CAN ID: ");
		printf_Put(Cob_ID_CAN1+'0');
		printf_string(" \n");
		FLASH_Lock();
		for(time_1ms_main = WAIT_TIME*1000; time_1ms_main; )
		{
			IWDG_ReloadCounter();
			
			if(time_1ms_main_teep != time_1ms_main)
			{
				time_1ms_main_teep = time_1ms_main;
				
				if(!(time_1ms_main%1000)) 
				{
					Uint_To_String( time_1ms_main/1000,string_buff);
					printf_string(string_buff);
					printf_string("S\n");
				}
			}
			
			if(ERROR != Get_CAN1( &get_CAN_teep))
			{
				if(IAP_ORDER_MOSI_CHANNEL == get_CAN_teep.ID)
				{
					if(1 == get_CAN_teep.number)
					{
						if(0x01 == get_CAN_teep.dates[0])
						{
							put_CAN_teep.ID = IAP_ORDER_MISO_CHANNEL;
							put_CAN_teep.number = 2;
							put_CAN_teep.dates[0] = 0x01;
							put_CAN_teep.dates[1] = 0x01;
							Put_CAN1(&put_CAN_teep);
						}
						if(0x02 == get_CAN_teep.dates[0])
						{
							SystemReset();
						}
						if(0x03 == get_CAN_teep.dates[0])
						{
							printf_string("Start UpDate.\n");
							for(time_1ms_main = 1000; time_1ms_main; IWDG_ReloadCounter());
							Ymodem();
						}
						if(0x04 == get_CAN_teep.dates[0])
						{
							break;
						}
					}
					
				  time_1ms_main = WAIT_TIME*1000;
				}
				
				if(DEBUG_MOSI_CHANNEL == get_CAN_teep.ID)
				{
					if(2 == get_CAN_teep.number)
					{
						if(0x01 == get_CAN_teep.dates[0])
						{
							if(!!get_CAN_teep.dates[1]) CAN_DEBUG = 1;
							else  CAN_DEBUG = 0;
							
							if(CAN_DEBUG) printf_string("Open CAN DEBUG!\n");
							else printf_string("Close CAN DEBUG!\n");
						}
					}
					
					time_1ms_main = WAIT_TIME*1000;
				}
			}
		}
		
		if ((0x20000000 <= (*(__IO uint32_t*)ApplicationAddress))&&(0x20005000 > (*(__IO uint32_t*)ApplicationAddress)))//判断是否已经有程序
		{
			printf_string("Run APP.\n");
			printf_string("\n");
			printf_string("\n");
			for(time_1ms_main = 100; time_1ms_main; IWDG_ReloadCounter());
//			IWDG_ReloadCounter();
			Close_it();
			JumpAPP(ApplicationAddress);
		}
		else
		{
			put_CAN_teep.ID = IAP_ORDER_MISO_CHANNEL;
			put_CAN_teep.number = 1;
			put_CAN_teep.dates[0] = 0x05;
			Put_CAN1(&put_CAN_teep);
			
			printf_string("No APP,Reset again.\n");
			printf_string("\n");
			printf_string("\n");
			for(time_1ms_main = 1000; time_1ms_main; );
			Close_it();
			JumpAPP(BootloaderAddress);
		}
	}
}

ErrorStatus Ymodem(void)
{
	unsigned int teep;
	unsigned char YMoDem_number_teep;
	uint32_t Size_YMoDem_teep;
	uint32_t UserMemoryMask = 0;
	unsigned char error_number = 0;
	unsigned int first_word = 0;
	unsigned int flash_word_w = 0;
	unsigned int flash_address_r = 0;
	
	
	puts_date_YMoDem.commed = MODEM_C;
	Put_date_YMoDem(&puts_date_YMoDem);
	error_number = Get_date_YMoDem(&gets_date_YMoDem);
	if(0 == error_number)
	{
		IWDG_ReloadCounter();
		printf_string("YMoDem Unlock.\n");
		FLASH_Unlock();//FLASH解锁
		
		puts_date_YMoDem.commed = MODEM_ACK;
		Put_date_YMoDem(&puts_date_YMoDem);
		if(0x00 == gets_date_YMoDem.number)
		{
			printf_string("YMoDem film name: ");
			printf_string((char*)&gets_date_YMoDem.date[0]);
			printf_string(".\n");
			for(teep = 0; teep < ((gets_date_YMoDem.commed==MODEM_SOH)?128:1024); teep++)
				if(!gets_date_YMoDem.date[teep]) break;
			
			printf_string("YMoDem film number: ");
			printf_string((char*)&gets_date_YMoDem.date[teep+1]);
			printf_string("Byte.\n");
			
			Size_YMoDem = 0;
			Size_YMoDem_teep = Size_YMoDem;
			for(teep++; teep < ((gets_date_YMoDem.commed==MODEM_SOH)?128:1024); teep++)
			{
				if(('0' <= gets_date_YMoDem.date[teep])&&('9' >= gets_date_YMoDem.date[teep]))
				{
					Size_YMoDem = Size_YMoDem*10+(gets_date_YMoDem.date[teep]-'0');
					Size_YMoDem_teep = Size_YMoDem;
					
					if((AllFlash-ApplicationAddress) < Size_YMoDem)
					{
						error_number = 4;
						goto ERROR_Ymodem;
					}
				}
				else 
					if((AllFlash-ApplicationAddress) < Size_YMoDem)
					{
						error_number = 4;
						goto ERROR_Ymodem;
					}
					else break;
			}
			
			if(0 == Size_YMoDem)
			{
				error_number = 5;
				goto ERROR_Ymodem;
			}
				
			YMoDem_number_teep = 1;
			puts_date_YMoDem.commed = MODEM_C;
			Put_date_YMoDem(&puts_date_YMoDem);
			while(Size_YMoDem_teep)
			{
				IWDG_ReloadCounter();
				error_number = Get_date_YMoDem(&gets_date_YMoDem);
				if(0 == error_number)
				{
					if(gets_date_YMoDem.number == YMoDem_number_teep++)
					{
						switch(gets_date_YMoDem.commed)
						{
							case MODEM_SOH : 
							case MODEM_STX : 
																UserMemoryMask = ((uint32_t)(1<<((ApplicationAddress+(Size_YMoDem-Size_YMoDem_teep) - 0x08000000) >> 12)));
																if ((FLASH_GetWriteProtectionOptionByte() & UserMemoryMask) == UserMemoryMask)
																{
																	//这里写入Flash
																	if(!((ApplicationAddress+(Size_YMoDem-Size_YMoDem_teep))%0x0400))
																	{	
																		while(SET == FLASH_GetFlagStatus(FLASH_FLAG_BSY));
																		FLASH_ErasePage(ApplicationAddress+(Size_YMoDem-Size_YMoDem_teep));
																	}
																	for(teep=0; teep < (((gets_date_YMoDem.commed==MODEM_SOH)?128:1024)/4); teep++)
																	{
																		IWDG_ReloadCounter();
																		while(SET == FLASH_GetFlagStatus(FLASH_FLAG_BSY));
																		
																		flash_address_r = ApplicationAddress+(Size_YMoDem-Size_YMoDem_teep)+teep*4;
																		flash_word_w = gets_date_YMoDem.date[teep*4]|(gets_date_YMoDem.date[teep*4+1]<<8)|(gets_date_YMoDem.date[teep*4+2]<<16)|(gets_date_YMoDem.date[teep*4+3]<<24);
																		if((0x20000000 <= first_word)&&(0x20005000 > first_word))
																		{
																			FLASH_ProgramWord(flash_address_r, flash_word_w);
																			while(SET == FLASH_GetFlagStatus(FLASH_FLAG_BSY));
																			if(flash_word_w != (*(__IO uint32_t*)flash_address_r))
																			{
																				error_number = 9;
																				goto ERROR_Ymodem;
																			}
																	  }
																		else 
																		{
																			first_word = flash_word_w;
																			if((0x20000000 <= first_word)&&(0x20005000 > first_word));
																			else 
																			{
																				error_number = 7;
																				goto ERROR_Ymodem;
																			}
																		}
																	}
																	if(Size_YMoDem_teep >= ((gets_date_YMoDem.commed==MODEM_SOH)?128:1024)) 
																		Size_YMoDem_teep -= (gets_date_YMoDem.commed==MODEM_SOH)?128:1024;
																	else
																	{
																		Size_YMoDem_teep = 0;
																	}
																}
																else 
																{
																	error_number = 8;
														      goto ERROR_Ymodem;
																}
																break;
							case MODEM_EOT : 
							case MODEM_ACK : 
							case MODEM_NAK : 
							case MODEM_CAN : 
							case MODEM_C   :  
							default				 :	
																error_number = 7;
																goto ERROR_Ymodem;
//																break;
						}
//						if(Size_YMoDem_teep)
						{
							puts_date_YMoDem.commed = MODEM_ACK;
							Put_date_YMoDem(&puts_date_YMoDem);
						}
					}
					else 
					{
						error_number = 6;
						goto ERROR_Ymodem;
					}
				}
				else 
				{
					goto ERROR_Ymodem;
				}
			}
			FLASH_ProgramWord(ApplicationAddress, first_word);
			while(SET == FLASH_GetFlagStatus(FLASH_FLAG_BSY));
			if(first_word != (*(__IO uint32_t*)ApplicationAddress))
			{
				error_number = 9;
				goto ERROR_Ymodem;
			}
			
			error_number = Get_date_YMoDem(&gets_date_YMoDem);
			if(0 == error_number)
			{
				switch(gets_date_YMoDem.commed)
				{
					case MODEM_SOH : 
					case MODEM_STX : 
														error_number = 7;
														goto ERROR_Ymodem;
//														break;
					case MODEM_EOT : 
														puts_date_YMoDem.commed = MODEM_NAK;
														Put_date_YMoDem(&puts_date_YMoDem);
														break;
					case MODEM_ACK : 
					case MODEM_NAK : 
					case MODEM_CAN : 
					case MODEM_C   :  
					default				 :	error_number = 7;
														goto ERROR_Ymodem;
//														break;
				}
			}
			else 
			{
				goto ERROR_Ymodem;
			}
			
			error_number = Get_date_YMoDem(&gets_date_YMoDem);
			if(0 == error_number)
			{
				switch(gets_date_YMoDem.commed)
				{
					case MODEM_SOH : 
					case MODEM_STX : 
														error_number = 7;
														goto ERROR_Ymodem;
//														break;
					case MODEM_EOT : 
														puts_date_YMoDem.commed = MODEM_ACK;
														Put_date_YMoDem(&puts_date_YMoDem);
														break;
					case MODEM_ACK : 
					case MODEM_NAK : 
					case MODEM_CAN : 
					case MODEM_C   :  
					default				 :	error_number = 7;
														goto ERROR_Ymodem;
//														break;
				}
			}
			else 
			{
				goto ERROR_Ymodem;
			}
		
			puts_date_YMoDem.commed = MODEM_C;
			Put_date_YMoDem(&puts_date_YMoDem);
			error_number = Get_date_YMoDem(&gets_date_YMoDem);
			if(0 == error_number)
			{
				switch(gets_date_YMoDem.commed)
				{
					case MODEM_SOH : 
					case MODEM_STX : 
														puts_date_YMoDem.commed = MODEM_ACK;
														Put_date_YMoDem(&puts_date_YMoDem);
														break;
					case MODEM_EOT : 
					case MODEM_ACK : 
					case MODEM_NAK : 
					case MODEM_CAN : 
					case MODEM_C   :  
					default				 :	error_number = 7;
														goto ERROR_Ymodem;
//														break;
				}
			}
			else 
			{
				goto ERROR_Ymodem;
			}
		}
		
		while(SET == FLASH_GetFlagStatus(FLASH_FLAG_BSY));
		FLASH_Lock();
		printf_string("YMoDem Lock.\n");
	}
	else 
	{
		if(1 == error_number) printf_string("YMoDem time out error.\n");
		else
		if(2 == error_number) printf_string("YMoDem check error.\n");
		else
		if(3 == error_number) printf_string("YMoDem unknown error.\n");
		else {printf_string("YMoDem other unknown error.\n");}
		
		goto ERROR_Ymodem_End;
	}
		
	printf_string("YMoDem Success.\n");
	return SUCCESS;
		
ERROR_Ymodem:
	switch(error_number)
	{
		case 1:   printf_string("YMoDem time out error.\n");
							goto ERROR_Ymodem_Erase;
//							break;
		case 2:   printf_string("YMoDem check error.\n");
							goto ERROR_Ymodem_Erase;
//							break;
		case 3:   printf_string("YMoDem unknown error.\n");
							goto ERROR_Ymodem_Erase;
//							break;
		case 4:   printf_string("YMoDem Data too much.\n");
		          goto ERROR_Ymodem_Lock;
//							break;
		case 5:   printf_string("YMoDem No Data.\n");
							goto ERROR_Ymodem_Lock;
//							break;
		case 6:   printf_string("YMoDem number error.\n");
							goto ERROR_Ymodem_Erase;
//							break;
		case 7:   printf_string("YMoDem Data error.\n");
							goto ERROR_Ymodem_Erase;
//							break;
		case 8:   printf_string("Flash can't write.\n");
							goto ERROR_Ymodem_Erase;
//							break;
		case 9:   printf_string("Flash write error.\n");
							goto ERROR_Ymodem_Erase;
//							break;
		default:  printf_string("YMoDem other unknown error.\n");
							goto ERROR_Ymodem_Erase;
//							break;
	}
ERROR_Ymodem_Erase:
	while(SET == FLASH_GetFlagStatus(FLASH_FLAG_BSY));
	FLASH_ErasePage(ApplicationAddress);
	printf_string("YMoDem Erase.\n");
ERROR_Ymodem_Lock:	
	while(SET == FLASH_GetFlagStatus(FLASH_FLAG_BSY));
	FLASH_Lock();
	printf_string("YMoDem Lock.\n");
ERROR_Ymodem_End:	
	return ERROR;
}

__asm void SystemReset(void)
{
 MOV R0, #1           //; 
 MSR FAULTMASK, R0    //;
 LDR R0, =0xE000ED0C  //;
 LDR R1, =0x05FA0004  //; 
 STR R1, [R0]         //;
 
deadloop
    B deadloop        //;
}

void printf_string(char* string) 			
{
	printf_USART2(string); 
	if(CAN_DEBUG) U2_Printf_CAN(DEBUG_MISO_CHANNEL, string); 
}
															
void printf_Put(char string)
{
	Put_USART2(string); 
	if(CAN_DEBUG) U2_Put_CAN(DEBUG_MISO_CHANNEL, string);
}
