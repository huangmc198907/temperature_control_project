#include <stdio.h>
#include "stm32f10x.h"	 
#include "NTC.h"
#include "IntToString.h"	

unsigned char NTC_Temp_s(const float* tab, float mrs, char* s)
{
	unsigned int teep1 = 0, teep2 = 150;
	int temp1,temp2;
	char* buff = s;
	char buff1;
	
	if(mrs < table_10K_3435[teep2]) return 0;
	if(mrs > table_10K_3435[teep1]) return 0;
	
	while(1 != (teep2-teep1))
	{
		if(mrs < table_10K_3435[(teep1+teep2)/2]) teep1 = (teep1+teep2)/2;
		else 
			if(mrs > table_10K_3435[(teep1+teep2)/2]) teep2 = (teep1+teep2)/2;
			else 
			{
				teep1 = (teep1+teep2)/2;
				teep2 = teep1+1;
			}
	}
	
	temp1 = teep1*100 - 5000;
	//��ֵ
	temp1 += (unsigned int)(100*(mrs-table_10K_3435[teep1])/(table_10K_3435[teep2]-table_10K_3435[teep1]));
	if(5 <= (temp1%10)) temp1 += 10;
	temp1 /= 10;
	
	Int_To_String(temp1,buff);
	
	if('-' == *buff) buff++;
	
	if('0' != *buff) 
	{
		if(NULL != *(buff+1))
		{
			while(NULL != *buff) buff++;
			
			buff--; 
			buff1 = *buff;
			*buff = '.';
			*(buff+1) = buff1;
			*(buff+2) = NULL;
		}
		else
		{
			buff1 = *buff;
			*buff = '0';
			*(buff+1) = '.';
			*(buff+2) = buff1;
			*(buff+3) = NULL;
		}
	}
	
	buff = s;
	for(teep1 = 0; NULL != *buff; teep1++) buff++;
	
	return teep1;
}

const float table_10K_3435[]=										//-50~110(step:1)						
{ 
	329.5 , 310.9 , 293.5 , 277.2 , 262.0 , 247.7 , 234.3 , 221.7 , 209.9 , 198.9 ,
	188.5 , 178.5 , 169.0 , 160.2 , 151.9 , 144.1 , 136.7 , 129.8 , 123.3 , 117.1 ,
	111.3 , 105.7 , 100.5 , 95.52 , 90.84 , 86.43 , 82.26 , 78.33 , 74.61 , 71.10 ,
	67.77 , 64.57 , 61.54 , 58.68 , 55.97 , 53.41 , 50.98 , 48.68 , 46.50 , 44.43 ,
	42.47 , 40.57 , 38.77 , 37.06 , 35.44 , 33.90 , 32.44 , 31.05 , 29.73 , 28.48 ,
	27.28 , 26.13 , 25.03 , 23.99 , 23.00 , 22.05 , 21.15 , 20.30 , 19.48 , 18.70 ,
	17.96 , 17.24 , 16.56 , 15.90 , 15.28 , 14.69 , 14.12 , 13.58 , 13.06 , 12.56 ,
	12.09 , 11.63 , 11.20 , 10.78 , 10.38 , 10.00 , 9.632 , 9.281 , 8.944 , 8.622 ,
	8.313 , 8.014 , 7.728 , 7.454 , 7.192 , 6.940 , 6.699 , 6.467 , 6.245 , 6.032 ,
	5.827 , 5.629 , 5.438 , 5.255 , 5.080 , 4.911 , 4.749 , 4.593 , 4.443 , 4.299 ,
	4.160 , 4.026 , 3.896 , 3.771 , 3.651 , 3.536 , 3.425 , 3.318 , 3.215 , 3.116 ,
	3.020 , 2.927 , 2.838 , 2.751 , 2.668 , 2.588 , 2.511 , 2.436 , 2.364 , 2.295 ,
	2.228 , 2.163 , 2.100 , 2.039 , 1.980 , 1.924 , 1.869 , 1.816 , 1.765 , 1.716 ,
	1.668 , 1.622 , 1.577 , 1.533 , 1.492 , 1.451 , 1.412 , 1.373 , 1.336 , 1.301 ,
	1.266 , 1.232 , 1.200 , 1.168 , 1.137 , 1.108 , 1.079 , 1.051 , 1.024 , 0.9984 ,
	0.9731 , 0.9484 , 0.9246 , 0.9014 , 0.8789 , 0.8572 , 0.8360 , 0.8155 , 0.7956 , 0.7763 ,
	0.7576
};
