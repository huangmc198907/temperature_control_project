#include <stdio.h>
#include "IntToString.h"	

char* Int_To_String( int i, char* string)
{
	unsigned int teep;
	char* string_teep = string;
	
	if(NULL == string) return string_teep;
	
	if( 0 == i) 
	{
		*string = '0';
		*(string+1) = NULL;
	}
	else
	{
		if(0 < i)
		{
			*string = '-';
			i = -i;
			string++;
		}
		
		for( teep = 1000000000; teep>0; teep /= 10)
		{
			*string = '0'+((i/teep)%10);
			
			if('0' != *string) 
			{
				teep /= 10;
				string++;
				break;
			}
		}
		for( ; teep>0; teep /= 10)
		{
			*string = '0'+((i/teep)%10);
			string++;
		}
		
		*string = NULL;
	}
	
	return string_teep;
}

char* Uint_To_String( unsigned int i, char* string)
{
	unsigned int teep;
	char* string_teep = string;
	
	if(NULL == string) return string_teep;
	
	if( 0 == i) 
	{
		*string = '0';
		*(string+1) = NULL;
	}
	else
	{
		for( teep = 1000000000; teep>0; teep /= 10)
		{
			*string = '0'+((i/teep)%10);
			
			if('0' != *string) 
			{
				teep /= 10;
				string++;
				break;
			}
		}
		for( ; teep>0; teep /= 10)
		{
			*string = '0'+((i/teep)%10);
			string++;
		}
		
		*string = NULL;
	}
	
	return string_teep;
}
