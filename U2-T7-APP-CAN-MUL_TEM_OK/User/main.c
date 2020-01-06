#include "stdio.h"
#include <string.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Library includes. */
#include "stm32f10x.h"	 
#include "stm32f10x_it.h"

#include "USART.h"
#include "CAN.H"
#include "IntToString.h"	

#include "main.h"
#include "system_Init.h"
#include "MCP3204_8.H"
#include "AT24C0X.H"
#include "U2_CAN_agreement.h"	 
#include "PT100.h"
#include "NTC.h"
#include "EEPROM_Management.H"
#include "quicksort.h"

char* Printf_buff_date;
#define Printf_buff_date(x)   \
							{\
								Printf_buff_date = x; \
							  xQueueSendToBack( Printf_Queue_Handle, &Printf_buff_date, 0);\
							}
unsigned char ADC_switch = 0x01;//MCP3208ADC工作开关，1~8通道对应0~7位;
float TEMP_a[8] = {0,0.44185,};
float TEMP_b[8] = {0,-41,};

struct ADC_data//ADC数据结构体
{
	unsigned char ID;//ADC数据通道
	unsigned int value;//ADC数据值
};


xQueueHandle Printf_Queue_Handle;//打印机队列句柄
xQueueHandle Put_CAN_Queue_Handle;//发送CAN队列句柄
xQueueHandle HeartBeat_CAN_Queue_Handle;//心跳队列句柄
xQueueHandle ADC_data_Queue_Handle;//ADC值队列句柄
xQueueHandle CAN_afreement_Queue_Handle;//CAN协议队列句柄


void vTask_Printf( void *pvParameters );//打印任务
void vTask_Put_CAN( void *pvParameters );//CAN发送任务
void vTask_CAN_afreement( void *pvParameters );//CAN协议任务
void vTask_HeartBeat( void *pvParameters );//心跳监测任务
void vTask_ADC( void *pvParameters );//ADC任务

void Load_EEPROM_Parameter(void);


int main(void)
{
	Init_system();
	IWDG_ReloadCounter();
	Load_EEPROM_Parameter();
	IWDG_ReloadCounter();
	
	Printf_Queue_Handle = xQueueCreate( 16, sizeof(char*));
	Put_CAN_Queue_Handle = xQueueCreate( 8, sizeof(struct CAN_Mail));
	HeartBeat_CAN_Queue_Handle = xQueueCreate( 4, sizeof(struct CAN_Mail));
	ADC_data_Queue_Handle = xQueueCreate( 36, sizeof(struct ADC_data));
	
	xTaskCreate( vTask_Printf, "Task Printf", 100, NULL, 4, NULL );
	xTaskCreate( vTask_Put_CAN, "Task Put_CAN", 100, NULL, 4, NULL );
	xTaskCreate( vTask_CAN_afreement, "Task CAN_afreement", 100, NULL, 4, NULL );
	xTaskCreate( vTask_HeartBeat, "Task HeartBea", 100, NULL, 2, NULL );
	xTaskCreate( vTask_ADC, "Task ADC", 600, NULL, 1, NULL );
	
	Printf_buff_date("This is app!\n");
	vTaskStartScheduler();
	
	while(1);
}


void vTask_Printf( void *pvParameters )
{
	char* sting_teep;
			
	for(;;)
	{
		if(pdPASS == xQueueReceive( Printf_Queue_Handle, &sting_teep, portMAX_DELAY))
		{
				printf_USART2(sting_teep);
		}
	}
	
	vTaskDelete(NULL);
}

void vTask_Put_CAN( void *pvParameters )
{
	struct CAN_Mail put_CAN_teep;
	
	for(;;)
	{
		if(pdPASS == xQueueReceive( Put_CAN_Queue_Handle, &put_CAN_teep, portMAX_DELAY))
		{
			Put_CAN1(&put_CAN_teep);
		}
	}
	
	vTaskDelete(NULL);
}

void vTask_CAN_afreement( void *pvParameters )
{
	struct CAN_Mail get_CAN_teep,put_CAN_teep;
	struct ADC_data ADC_datas;
	unsigned int ADC_data_buff[8] = {0,};
	float teep,teep2;
	int temp;
	unsigned int buff1,buff2;
	
	for(;;)
	{
		if(pdPASS == xQueueReceive( ADC_data_Queue_Handle, &ADC_datas, 0))
		{
			ADC_data_buff[ADC_datas.ID] = ADC_datas.value;
		}
		
		while(ERROR != Get_CAN1( &get_CAN_teep))
		{
			if(BROADCAST_CHANNEL == get_CAN_teep.ID)
			{
				break;
			}
			if(IAP_ORDER_MOSI_CHANNEL == get_CAN_teep.ID)
			{
				if(1 == get_CAN_teep.number)
				{
					if(0x01 == get_CAN_teep.dates[0])
					{
						put_CAN_teep.ID = IAP_ORDER_MISO_CHANNEL;
						put_CAN_teep.number = 2;
						put_CAN_teep.dates[0] = 0x01;
						put_CAN_teep.dates[1] = 0x02;
						xQueueSendToBack( Put_CAN_Queue_Handle, &put_CAN_teep, 10);
						Printf_buff_date("This is APP!\n");
						break;
					}
					if(0x02 == get_CAN_teep.dates[0])
					{
						SystemReset();
						break;
					}
				  break;
				}
				
				break;
			}
			if(APP_ORDER_MOSI_CHANNEL == get_CAN_teep.ID)
			{
				if(0x00 == get_CAN_teep.dates[0])
				{
					if(0xa1 == get_CAN_teep.dates[1])
					{
						Byte_Write_AT24C0X((get_CAN_teep.dates[2]<<8)|get_CAN_teep.dates[3], get_CAN_teep.dates[4]);
						break;
					}
					if(0xa2 == get_CAN_teep.dates[1])
					{
						put_CAN_teep.ID = APP_ORDER_MISO_CHANNEL;
						put_CAN_teep.number = 5;
						put_CAN_teep.dates[0] = get_CAN_teep.dates[0];
						put_CAN_teep.dates[1] = get_CAN_teep.dates[1];
						put_CAN_teep.dates[2] = get_CAN_teep.dates[2];
						put_CAN_teep.dates[3] = get_CAN_teep.dates[3];
						put_CAN_teep.dates[4] = Random_Read_AT24C0X((get_CAN_teep.dates[2]<<8)|get_CAN_teep.dates[3]);
						xQueueSendToBack( Put_CAN_Queue_Handle, &put_CAN_teep, 10);
						break;
					}
					break;
				}
				if(0x11 == get_CAN_teep.dates[0])
				{
					if(('?' == get_CAN_teep.dates[2])||(':' == get_CAN_teep.dates[2]))
					{
						switch(get_CAN_teep.dates[1])
						{
							case 0x11:	buff1 = ADC_SWITCH_BASE; buff2 = ADC_SWITCH_LONG; break;
							case 0x21:	buff1 = ADC1_SAMP1_BASE; buff2 = ADC1_SAMP1_LONG; break;
							case 0x22:	buff1 = ADC2_SAMP1_BASE; buff2 = ADC2_SAMP1_LONG; break;
							case 0x23:	buff1 = ADC3_SAMP1_BASE; buff2 = ADC3_SAMP1_LONG; break;
							case 0x24:	buff1 = ADC4_SAMP1_BASE; buff2 = ADC4_SAMP1_LONG; break;
							case 0x25:	buff1 = ADC5_SAMP1_BASE; buff2 = ADC5_SAMP1_LONG; break;
							case 0x26:	buff1 = ADC6_SAMP1_BASE; buff2 = ADC6_SAMP1_LONG; break;
							case 0x27:	buff1 = ADC7_SAMP1_BASE; buff2 = ADC7_SAMP1_LONG; break;
							case 0x31:	buff1 = ADC1_SAMP2_BASE; buff2 = ADC1_SAMP2_LONG; break;
							case 0x32:	buff1 = ADC2_SAMP2_BASE; buff2 = ADC2_SAMP2_LONG; break;
							case 0x33:	buff1 = ADC3_SAMP2_BASE; buff2 = ADC3_SAMP2_LONG; break;
							case 0x34:	buff1 = ADC4_SAMP2_BASE; buff2 = ADC4_SAMP2_LONG; break;
							case 0x35:	buff1 = ADC5_SAMP2_BASE; buff2 = ADC5_SAMP2_LONG; break;
							case 0x36:	buff1 = ADC6_SAMP2_BASE; buff2 = ADC6_SAMP2_LONG; break;
							case 0x37:	buff1 = ADC7_SAMP2_BASE; buff2 = ADC7_SAMP2_LONG; break;
							default  :  buff1 = 0; buff2 = 0; break;
						}
						if(':' == get_CAN_teep.dates[2])
						{
							Random_EEPROM_Write(buff1, &get_CAN_teep.dates[3], buff2);
						}
						put_CAN_teep.ID = APP_ORDER_MISO_CHANNEL;
						put_CAN_teep.number = 3+buff2;
						put_CAN_teep.dates[0] = get_CAN_teep.dates[0];
						put_CAN_teep.dates[1] = get_CAN_teep.dates[1];
						put_CAN_teep.dates[2] = ':';
						Random_EEPROM_Read(buff1, &put_CAN_teep.dates[3], buff2);
						xQueueSendToBack( Put_CAN_Queue_Handle, &put_CAN_teep, 10);
						break;
					}
					break;
				}
				break;
			}
			if(APP_MOSI_CHANNEL == get_CAN_teep.ID)
			{
				if(2 == get_CAN_teep.number)
				{
					if(0x00 == get_CAN_teep.dates[0])
					{
						xQueueSendToBack( HeartBeat_CAN_Queue_Handle, &get_CAN_teep, 10);
						break;
					}
					if(0x01 == get_CAN_teep.dates[0])
					{
//						if(1 == get_CAN_teep.dates[1])
//						{
							teep = ADC_data_buff[get_CAN_teep.dates[1]-1];
//							teep = teep*TEMP_a[1]+TEMP_b[1]-99.54;
							teep = teep*TEMP_a[get_CAN_teep.dates[1]-1]+TEMP_b[get_CAN_teep.dates[1]-1];
							temp = PT100_Temp_s(teep, &put_CAN_teep.dates[2]);
							
							put_CAN_teep.ID = APP_MISO_CHANNEL;
							put_CAN_teep.number = 2+temp;
							put_CAN_teep.dates[0] = get_CAN_teep.dates[0];
							put_CAN_teep.dates[1] = get_CAN_teep.dates[1];
							xQueueSendToBack( Put_CAN_Queue_Handle, &put_CAN_teep, 10);
								
					    break;
//						}
/*						if((2 <= get_CAN_teep.dates[1])&&(7 >= get_CAN_teep.dates[1]))
						{
							teep = ADC_data_buff[get_CAN_teep.dates[1]-1];
							teep = teep/10;
							teep = ((teep+TEMP_b[get_CAN_teep.dates[1]-1])*TEMP_a[get_CAN_teep.dates[1]-1])/(0xfff-(teep+TEMP_b[get_CAN_teep.dates[1]-1]));
							temp = NTC_Temp_s(table_10K_3435, teep, &put_CAN_teep.dates[2]);
							
							put_CAN_teep.ID = APP_MISO_CHANNEL;
							put_CAN_teep.number = 2+temp;
							put_CAN_teep.dates[0] = get_CAN_teep.dates[0];
							put_CAN_teep.dates[1] = get_CAN_teep.dates[1];
							xQueueSendToBack( Put_CAN_Queue_Handle, &put_CAN_teep, 10);
								
					    break;
						}
*/						
					  break;
					}
					if(0x03 == get_CAN_teep.dates[0])
					{
						if((1 <= get_CAN_teep.dates[1])&&(8 >= get_CAN_teep.dates[1]))
						{
							put_CAN_teep.ID = APP_MISO_CHANNEL;
							put_CAN_teep.number = 5;
							put_CAN_teep.dates[0] = get_CAN_teep.dates[0];
							put_CAN_teep.dates[1] = get_CAN_teep.dates[1];
							put_CAN_teep.dates[2] = ADC_data_buff[get_CAN_teep.dates[1]-1]>>16;
							put_CAN_teep.dates[3] = ADC_data_buff[get_CAN_teep.dates[1]-1]>>8;
							put_CAN_teep.dates[4] = ADC_data_buff[get_CAN_teep.dates[1]-1];
							xQueueSendToBack( Put_CAN_Queue_Handle, &put_CAN_teep, 10);
							
							break;
						}
						break;
					}
					break;
				}
				break;
			}
			if(CODE_MOSI_CHANNEL == get_CAN_teep.ID)
			{
				break;
			}
		}
		vTaskDelay(1);
	}
	
	vTaskDelete(NULL);
}

void vTask_HeartBeat( void *pvParameters )
{
	struct CAN_Mail HeartBeat, HeartBeat_teep;
	unsigned char Serial_number = 0;
	unsigned char Record_error = 0;
	
	HeartBeat.ID = APP_MISO_CHANNEL;
	HeartBeat.number = 2;
	HeartBeat.dates[0] = 0x00;
	HeartBeat.dates[1] = 0;
	
	for(;;)
	{
		IWDG_ReloadCounter();
		if(pdPASS == xQueueReceive( HeartBeat_CAN_Queue_Handle, &HeartBeat_teep, 1000))
		{
			if(APP_MOSI_CHANNEL == HeartBeat_teep.ID)
				if(2 == HeartBeat_teep.number)
					if(0x00 == HeartBeat_teep.dates[0])
					{
						if(Serial_number == HeartBeat_teep.dates[1])
						{
							Serial_number++;
							Record_error = 0;
							
							HeartBeat.dates[1] = HeartBeat_teep.dates[1];
							xQueueSendToBack( Put_CAN_Queue_Handle, &HeartBeat, 10);
						}
						else
						{
							Serial_number = HeartBeat_teep.dates[1];
							Serial_number++;
							Record_error = 2;
						}
					}
		}
		else
		{
			Record_error = 1;
		}
	}
	
	vTaskDelete(NULL);
}

void vTask_ADC( void *pvParameters ) 
{  
	unsigned int i,j;
	unsigned int dates[8]={0};
	unsigned char ADC_switch_teep;
	struct ADC_data ADC_datas;
	struct CAN_Mail put_CAN_teep;
	unsigned int data[200];
	portTickType xLastWakeTime;
	
	for(;;)
	{

#if 0		
		for( i=0; i<200; i++)
		{
			ADC_switch_teep = ADC_switch;
			for( j=0; j<8; j++)
			{
				if(ADC_switch_teep&0x01){
					if(0 == i)
						dates[j] = Read_MCP3204_8(0x80|((j)<<4));
					else
						dates[j] += Read_MCP3204_8(0x80|((j)<<4));
				}
				ADC_switch_teep >>= 1;
				
			}
			IWDG_ReloadCounter();
			vTaskDelayUntil( &xLastWakeTime, 5/portTICK_RATE_MS);
		}
		
		ADC_switch_teep = ADC_switch;
		for( i=0; i<8; i++)
		{
			if(ADC_switch_teep&0x01)
			{
				ADC_datas.ID = i;
				dates[i] /= 20;
			  if((20/2)<=(dates[i]%20)) dates[i]++;
				ADC_datas.value = dates[i];
				
				xQueueSendToBack( ADC_data_Queue_Handle, &ADC_datas, 10);
			}
			ADC_switch_teep >>= 1;
		}
#else		
		ADC_switch_teep = ADC_switch;
		vTaskDelayUntil( &xLastWakeTime, 5/portTICK_RATE_MS);
		for( j=0; j<8; j++){
			if(ADC_switch_teep&0x01){
				for( i=0; i<200; i++){
					data[i] = Read_MCP3204_8(0x80|((j)<<4));

				}
				Bubble_2(data,200);
				dates[j] = 0;
				for(i=50;i<150;i++){
					dates[j] += data[i];
				}
				IWDG_ReloadCounter();
				vTaskDelayUntil( &xLastWakeTime, 5/portTICK_RATE_MS);
				ADC_datas.ID = j;
				dates[j] /= 10;
				if((10/2)<=(dates[j]%10)) dates[j]++;
				ADC_datas.value = dates[j];
				xQueueSendToBack( ADC_data_Queue_Handle, &ADC_datas, 10);	
				
			}
//			IWDG_ReloadCounter();
//			vTaskDelayUntil( &xLastWakeTime, 5/portTICK_RATE_MS);
				
			ADC_switch_teep >>= 1;
			
		}
#endif
		{
			char string[32];
			float teep;
			sprintf(string,"in0=%d,in1=%d,in2=%d,in3=%d,in4=%d,in5=%d,in6=%d,in7=%d\n",dates[0],dates[1],dates[2],dates[3],dates[4],dates[5],dates[6],dates[7]);
			Printf_buff_date(string);
//			teep = dates[0];
//			teep = teep*TEMP_a[0]+TEMP_b[0];
			for(i=0;i<8;i++)
			{
				teep = TEMP_a[i]*dates[i]+TEMP_b[i];
				PT100_Temp_s(teep, string);
				Printf_buff_date("chx= ");
				Printf_buff_date(string); 
				
			}
			Printf_buff_date(", \r\n"); 
//			PT100_Temp_s(teep, string);
//			Printf_buff_date(", ch0= ");
//			Printf_buff_date(string); 
//			teep = dates[1];
//			teep = teep*TEMP_a[1]+TEMP_b[1]-99.54;
//			PT100_Temp_s(teep, string);
//			Printf_buff_date(", ch1="); 
//			Printf_buff_date(string);
			
		}
	}
	
	vTaskDelete(NULL);
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
void Load_EEPROM_Parameter(void)
{
	unsigned char buff[8];
	float rm1, rm2, rm3, addady1, addady2, addady3;
	int teep, teep2;
	
	Random_EEPROM_Read(ADC_SWITCH_BASE, buff, ADC_SWITCH_LONG);IWDG_ReloadCounter();
	if((0xff != buff[0])||(0xff != buff[1])||(0xff != buff[2])||(0xff != buff[3]))
		ADC_switch = buff[3];
	else
		ADC_switch = 0xff;
	
	Random_EEPROM_Read(ADC1_SAMP1_BASE, buff, ADC1_SAMP1_LONG);IWDG_ReloadCounter();
	if((0xff != buff[0])||(0xff != buff[1])||(0xff != buff[2])||(0xff != buff[3]))
	{
		teep = (buff[0]<<24)|(buff[1]<<16)|(buff[2]<<8)|buff[3];
		
		rm1 = (buff[0]<<8)|buff[1];
		addady1 = (buff[2]<<8)|buff[3];
		
		Random_EEPROM_Read(ADC1_SAMP2_BASE, buff, ADC1_SAMP2_LONG);IWDG_ReloadCounter();
		if((0xff != buff[0])||(0xff != buff[1])||(0xff != buff[2])||(0xff != buff[3]))
		{
			teep = (buff[0]<<24)|(buff[1]<<16)|(buff[2]<<8)|buff[3];
			
			rm2 = (buff[0]<<8)|buff[1];
			addady2 = (buff[2]<<8)|buff[3];
		}
		else
		{
			rm1 = 120;
			addady1 = 0x0ee6*10;
			rm2 = 20;
			addady2 = 0x027c*10;
		}
	}
	else
	{
		rm1 = 120;
		addady1 = 0x0ee6*10;
		rm2 = 20;
		addady2 = 0x027c*10;
	}
	rm1 *= 100;
	rm2 *= 100;
	
	TEMP_a[0] = (rm1-rm2)/(addady1-addady2);
	TEMP_b[0] = (rm2*addady1-rm1*addady2)/(addady1-addady2);
	
	for(teep2=1; teep2<=6; teep2++)
	{
		IWDG_ReloadCounter();
		switch(teep2)
		{
			case 1:	Random_EEPROM_Read(ADC2_SAMP1_BASE, buff, ADC2_SAMP1_LONG); break;
			case 2:	Random_EEPROM_Read(ADC3_SAMP1_BASE, buff, ADC3_SAMP1_LONG); break;
			case 3:	Random_EEPROM_Read(ADC4_SAMP1_BASE, buff, ADC4_SAMP1_LONG); break;
			case 4:	Random_EEPROM_Read(ADC5_SAMP1_BASE, buff, ADC5_SAMP1_LONG); break;
			case 5:	Random_EEPROM_Read(ADC6_SAMP1_BASE, buff, ADC6_SAMP1_LONG); break;
			case 6:	Random_EEPROM_Read(ADC7_SAMP1_BASE, buff, ADC7_SAMP1_LONG); break;
		}
		if((0xff != buff[0])||(0xff != buff[1])||(0xff != buff[2])||(0xff != buff[3]))
		{
			teep = (buff[0]<<24)|(buff[1]<<16)|(buff[2]<<8)|buff[3];
			
			rm1 = (buff[0]<<8)|buff[1];
			addady1 = (buff[2]<<8)|buff[3];
		}
		else
		{
			rm1 = 10000;
			addady1 = 0x5000;
		}
		addady1 /= 10;
		
		TEMP_a[teep2] = (((0xfff-(float)addady1)/addady1)*rm1)/1000;
		TEMP_b[teep2] = 0;
	}
	TEMP_a[0] = 0.694467;
	TEMP_b[0] = -362;
	TEMP_a[1] = 0.694467;
	TEMP_b[1] = -355;
	TEMP_a[2] = 0.6993235;
	TEMP_b[2] = -354.65;
	
	TEMP_a[3] = 0.698093;
	TEMP_b[3] = -366.7;
	TEMP_a[4] = 0.69689;
	TEMP_b[4] = -367;
	TEMP_a[5] = 0.696887;
	TEMP_b[5] = -360;	
	TEMP_a[6] = 0.697248;
	TEMP_b[6] = -359.5;
	TEMP_a[7] = 0.696887;
	TEMP_b[7] = -353;

	
	
}
