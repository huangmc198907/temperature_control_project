#include <stdio.h>
#include "IntToString.h"	

char* Int_To_String( int i, char* string)
{
	unsigned int teep;
	char* string_teep = string;
	
	if(NULL == string) return string_teep;
	
	if( 0 == i) 
	{
		*string_teep = '0';
		*(string_teep+1) = NULL;
	}
	else
	{
		if(0 > i)
		{
			*string_teep = '-';
			i = -i;
			string_teep++;
		}
		
		for( teep = 1000000000; teep>0; teep /= 10)
		{
			*string_teep = '0'+((i/teep)%10);
			
			if('0' != *string_teep) 
			{
				string_teep++;
				teep /= 10;
				break;
			}
		}
		for( ; teep>0; teep /= 10)
		{
			*string_teep = '0'+((i/teep)%10);
			string_teep++;
		}
		
		*string_teep = NULL;
	}
	
	return string;
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
