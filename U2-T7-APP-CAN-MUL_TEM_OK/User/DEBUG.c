#include "DEBUG.h"
#include "USART.h"


#ifdef _MY_DEBUG	

void Init_DEBUG(void)
{
	Init_DEBUG_Port();
}

void myprintf_string(char * string)
{
	char i;

	for(i=64;i>0;i--)
	if(*string == 0x00) break;
	else 
	{
		Put_DEBUG_Port(*string); string++;
	}
}

void my_debug_printf(unsigned char number, char * string)
{
	static unsigned char number_buf=0;
	
	if(number)
	if(number_buf == number) return;	
	else number_buf = number;
	else ;

	myprintf_string(string); 
	Put_DEBUG_Port(0x0d);
}

#endif /*_MY_DEBUG*/
