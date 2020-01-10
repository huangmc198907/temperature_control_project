#include "USART.h"

#if defined USED_USART1
uint16_t puttop_USART1;
uint16_t putend_USART1;
uint16_t putall_USART1;
uint8_t putbuff_USART1[PUTBUFF_USART1];
uint16_t gettop_USART1;
uint16_t getend_USART1;
uint16_t getall_USART1;
uint8_t getbuff_USART1[GETBUFF_USART1];
#endif

#if defined USED_USART2
uint16_t puttop_USART2;
uint16_t putend_USART2;
uint16_t putall_USART2;
uint8_t putbuff_USART2[PUTBUFF_USART2];
uint16_t gettop_USART2;
uint16_t getend_USART2;
uint16_t getall_USART2;
uint8_t getbuff_USART2[GETBUFF_USART2];
#endif

#if defined USED_USART3
uint16_t puttop_USART3;
uint16_t putend_USART3;
uint16_t putall_USART3;
uint8_t putbuff_USART3[PUTBUFF_USART3];
uint16_t gettop_USART3;
uint16_t getend_USART3;
uint16_t getall_USART3;
uint8_t getbuff_USART3[GETBUFF_USART3];
#endif


#if defined USED_USART1
ErrorStatus Init_USART1(void)
{
	USART_InitTypeDef  USART_InitStruct;

	Clearput_USART1();
	Clearget_USART1();

	/*初始化*/
	USART_StructInit( &USART_InitStruct);
	USART_InitStruct.USART_BaudRate = 115200;
	USART_Init( USART1, &USART_InitStruct);
	USART_Cmd( USART1, ENABLE);
	USART_ITConfig( USART1, USART_IT_TXE, ENABLE);
	USART_ITConfig( USART1, USART_IT_RXNE, ENABLE);
	//USART_SendData( USART1, 0);
	USART1->SR |= (uint16_t)USART_FLAG_TXE;

	return SUCCESS;
}

void Clearput_USART1(void)
{
	unsigned int teep;
	
	puttop_USART1 = 0;
	putend_USART1 = 0;
	putall_USART1 = 0;
	
	for(teep = 0; teep < PUTBUFF_USART1; teep++)
		putbuff_USART1[teep] = 0;
}

uint16_t Checkput_USART1(void)
{
	return (PUTBUFF_USART1-putall_USART1);	
}

ErrorStatus Put_USART1(uint8_t data)
{
	USART_ITConfig( USART1, USART_IT_TXE, DISABLE);

	if( putall_USART1 < PUTBUFF_USART1) 
	{
		putbuff_USART1[ puttop_USART1] = data;
		putall_USART1++;
		if( puttop_USART1 < ( PUTBUFF_USART1 - 1)) puttop_USART1++;
		else puttop_USART1 = 0;
	}

	USART_ITConfig( USART1, USART_IT_TXE, ENABLE);

	return SUCCESS;
}

void Clearget_USART1(void)
{
	unsigned int teep;
	
	gettop_USART1 = 0;
	getend_USART1 = 0;
	getall_USART1 = 0;
	
	for(teep = 0; teep < GETBUFF_USART1; teep++)
		getbuff_USART1[teep] = 0;
}

uint16_t Checkget_USART1(void)
{
	return getall_USART1; 
}

uint8_t Get_USART1(void)
{
	uint8_t teep=0;

	USART_ITConfig( USART1, USART_IT_RXNE, DISABLE);

	if( getall_USART1 > 0) 
	{
		teep = getbuff_USART1[ getend_USART1];
		getall_USART1--;
		if(getend_USART1 < ( GETBUFF_USART1 - 1)) getend_USART1++;
		else getend_USART1 = 0;
	}

	USART_ITConfig( USART1, USART_IT_RXNE, ENABLE);

	return teep;
}

ErrorStatus printf_USART1(char* string)
{
	unsigned char number_teep;
	char* string_teep;
	
	string_teep = string;
	
	for(number_teep=0; number_teep<0xff; string_teep++)
	{
		if(*string_teep) number_teep++;
		else break;
	}
	if(0xff > number_teep)
	if((PUTBUFF_USART1-putall_USART1) >= number_teep)
	{
		string_teep = string;
		
		while(*string_teep) 
		{
			Put_USART1(*string_teep);
			string_teep++;
		}
		return SUCCESS;
	}
	
	return ERROR;
}
#endif


#if defined USED_USART2
ErrorStatus Init_USART2(void)
{
	USART_InitTypeDef  USART_InitStruct;

	Clearput_USART2();
	Clearget_USART2();

	/*初始化*/
	USART_StructInit( &USART_InitStruct);
	USART_InitStruct.USART_BaudRate = 115200;
	USART_Init( USART2, &USART_InitStruct);
	USART_Cmd( USART2, ENABLE);
	USART_ITConfig( USART2, USART_IT_TXE, ENABLE);
	USART_ITConfig( USART2, USART_IT_RXNE, ENABLE);
	
	USART2->SR |= (uint16_t)USART_FLAG_TXE;

	return SUCCESS;
}

void Clearput_USART2(void)
{
	unsigned int teep;
	
	puttop_USART2 = 0;
	putend_USART2 = 0;
	putall_USART2 = 0;
	
	for(teep = 0; teep < PUTBUFF_USART2; teep++)
		putbuff_USART2[teep] = 0;
}

uint16_t Checkput_USART2(void)
{
	return (PUTBUFF_USART2-putall_USART2);	
}

ErrorStatus Put_USART2(uint8_t data)
{
	USART_ITConfig( USART2, USART_IT_TXE, DISABLE);

	if( putall_USART2 < PUTBUFF_USART2) 
	{
		putbuff_USART2[ puttop_USART2] = data;
		putall_USART2++;
		if( puttop_USART2 < ( PUTBUFF_USART2 - 1)) puttop_USART2++;
		else puttop_USART2 = 0;
	}

	USART_ITConfig( USART2, USART_IT_TXE, ENABLE);

	return SUCCESS;
}

void Clearget_USART2(void)
{
	unsigned int teep;
	
	gettop_USART2 = 0;
	getend_USART2 = 0;
	getall_USART2 = 0;
	
	for(teep = 0; teep < GETBUFF_USART2; teep++)
		getbuff_USART2[teep] = 0;
}

uint16_t Checkget_USART2(void)
{
	return getall_USART2; 
}

uint8_t Get_USART2(void)
{
	uint8_t teep=0;

	USART_ITConfig( USART2, USART_IT_RXNE, DISABLE);

	if( getall_USART2 > 0) 
	{
		teep = getbuff_USART2[ getend_USART2];
		getall_USART2--;
		if(getend_USART2 < ( GETBUFF_USART2 - 1)) getend_USART2++;
		else getend_USART2 = 0;
	}

	USART_ITConfig( USART2, USART_IT_RXNE, ENABLE);

	return teep;
}

#if 0
#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))
#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000

ErrorStatus printf_USART2(char* string)
{
	while(*string){
	  if (DEMCR & TRCENA){
			while (ITM_Port32(0) == 0);
			ITM_Port8(0) = *string;
			string++;
		}
	}
	return SUCCESS;
}
#else
ErrorStatus printf_USART2(char* string)
{
	unsigned char number_teep;
	char* string_teep;
	
	string_teep = string;
	
	for(number_teep=0; number_teep<0xff; string_teep++)
	{
		if(*string_teep) number_teep++;
		else break;
	}
	if(0xff > number_teep)
	if((PUTBUFF_USART2-putall_USART2) >= number_teep)
	{
		string_teep = string;
		
		while(*string_teep) 
		{
			Put_USART2(*string_teep);
			string_teep++;
		}
		return SUCCESS;
	}
	
	return ERROR;
}
#endif
#endif


#if defined USED_USART3
ErrorStatus Init_USART3(void)
{
	USART_InitTypeDef  USART_InitStruct;

	Clearput_USART3();
	Clearget_USART3();

	/*初始化*/
	USART_StructInit( &USART_InitStruct);
	USART_InitStruct.USART_BaudRate = 115200;
	USART_Init( USART3, &USART_InitStruct);
	USART_Cmd( USART3, ENABLE);
	USART_ITConfig( USART3, USART_IT_TXE, ENABLE);
	USART_ITConfig( USART3, USART_IT_RXNE, ENABLE);
	
	USART2->SR |= (uint16_t)USART_FLAG_TXE;

	return SUCCESS;
}

void Clearput_USART3(void)
{
	unsigned int teep;
	
	puttop_USART3 = 0;
	putend_USART3 = 0;
	putall_USART3 = 0;
	
	for(teep = 0; teep < PUTBUFF_USART3; teep++)
		putbuff_USART3[teep] = 0;
}

uint16_t Checkput_USART3(void)
{
	return (PUTBUFF_USART3-putall_USART3);	
}

ErrorStatus Put_USART3(uint8_t data)
{
	USART_ITConfig( USART3, USART_IT_TXE, DISABLE);

	if( putall_USART3 < PUTBUFF_USART3) 
	{
		putbuff_USART3[ puttop_USART3] = data;
		putall_USART3++;
		if( puttop_USART3 < ( PUTBUFF_USART3 - 1)) puttop_USART3++;
		else puttop_USART3 = 0;
	}

	USART_ITConfig( USART3, USART_IT_TXE, ENABLE);

	return SUCCESS;
}

void Clearget_USART3(void)
{
	unsigned int teep;
	
	gettop_USART3 = 0;
	getend_USART3 = 0;
	getall_USART3 = 0;
	
	for(teep = 0; teep < GETBUFF_USART3; teep++)
		getbuff_USART3[teep] = 0;
}

uint16_t Checkget_USART3(void)
{
	return getall_USART3; 
}

uint8_t Get_USART3(void)
{
	uint8_t teep=0;

	USART_ITConfig( USART3, USART_IT_RXNE, DISABLE);

	if( getall_USART3 > 0) 
	{
		teep = getbuff_USART3[ getend_USART3];
		getall_USART3--;
		if(getend_USART3 < ( GETBUFF_USART3 - 1)) getend_USART3++;
		else getend_USART3 = 0;
	}

	USART_ITConfig( USART3, USART_IT_RXNE, ENABLE);

	return teep;
}

ErrorStatus printf_USART3(char* string)
{
	unsigned char number_teep;
	char* string_teep;
	
	string_teep = string;
	
	for(number_teep=0; number_teep<0xff; string_teep++)
	{
		if(*string_teep) number_teep++;
		else break;
	}
	if(0xff > number_teep)
	if((PUTBUFF_USART3-putall_USART3) >= number_teep)
	{
		string_teep = string;
		
		while(*string_teep) 
		{
			Put_USART3(*string_teep);
			string_teep++;
		}
		return SUCCESS;
	}
	
	return ERROR;
}
#endif
