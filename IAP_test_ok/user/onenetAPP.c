/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	onenetAPP.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2017-02-27
	*
	*	�汾�� 		V1.0
	*
	*	˵���� 		onenetƽ̨�·����ݵĴ���
	*
	*	�޸ļ�¼��	
	************************************************************
	************************************************************
	************************************************************
**/

//Э���
#include "onenet.h"

//Ӧ��
#include "onenetAPP.h"

//�����豸����
#include "esp8266.h"

//Ӳ������
#include "usart.h"
#include "led.h"

//C��
#include <string.h>
#include <stdlib.h>






//==========================================================
//	�������ƣ�	OneNet_Replace
//
//	�������ܣ�	�滻ƽ̨�·������еĽ�����
//
//	��ڲ�����	res��ƽ̨�·�������
//
//	���ز�����	��
//
//	˵����		ƽ̨���ء���ť�·���������й̶�4�����������滻Ϊ�����������ַ����Է���string�����
//==========================================================
void OneNet_Replace(unsigned char *res, unsigned char num)
{
	
	unsigned char count = 0;

	while(count < num)
	{
		if(*res == '\0')		//�ҵ�������
		{
			*res = '~';			//�滻
			count++;
		}
		
		res++;
	}

}


//==========================================================
//	�������ƣ�	OneNet_Event
//
//	�������ܣ�	ƽ̨�������ݼ��
//
//	��ڲ�����	dataPtr��ƽ̨���ص�����
//
//	���ز�����	��
//
//	˵����		
//==========================================================
extern unsigned char esp8266_buf[128];
void OneNet_App(void)
{

	char *dataPtr;
	char numBuf[10];
	int num = 0;
	unsigned char *cmd;
	
	cmd = (unsigned char *)strchr((char *)esp8266_buf, ':');
	if(cmd == NULL)
		return;
	
	cmd++;
	if(cmd[0] == 0xA0)									//�����·����ܽ��յ�����
		OneNet_Replace(cmd, 4);
	else
		return;
		
	dataPtr = strstr((const char *)cmd, "}");			//����'}'
	
	if(dataPtr != NULL)									//����ҵ���
	{
		dataPtr++;
		
		while(*dataPtr >= '0' && *dataPtr <= '9')		//�ж��Ƿ����·��������������
		{
			numBuf[num++] = *dataPtr++;
		}
		
		num = atoi((const char *)numBuf);				//תΪ��ֵ��ʽ
		
		if(strstr((char *)cmd, "redled"))				//����"redled"
		{
			if(num == 1)								//�����������Ϊ1������
			{
				Led5_Set(LED_ON);
			}
			else if(num == 0)							//�����������Ϊ0�������
			{
				Led5_Set(LED_OFF);
			}
		}
														//��ͬ
		else if(strstr((char *)cmd, "greenled"))
		{
			if(num == 1)
			{
				Led4_Set(LED_ON);
			}
			else if(num == 0)
			{
				Led4_Set(LED_OFF);
			}
		}
		else if(strstr((char *)cmd, "yellowled"))
		{
			if(num == 1)
			{
				Led3_Set(LED_ON);
			}
			else if(num == 0)
			{
				Led3_Set(LED_OFF);
			}
		}
		else if(strstr((char *)cmd, "blueled"))
		{
			if(num == 1)
			{
				Led2_Set(LED_ON);
			}
			else if(num == 0)
			{
				Led2_Set(LED_OFF);
			}
		}
	}
	
	ESP8266_Clear();									//��ջ���

}
