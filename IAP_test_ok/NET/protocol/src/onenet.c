/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	onenet.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2017-05-08
	*
	*	�汾�� 		V1.1
	*
	*	˵���� 		��onenetƽ̨�����ݽ����ӿڲ�
	*
	*	�޸ļ�¼��	V1.0��Э���װ�������ж϶���ͬһ���ļ������Ҳ�ͬЭ��ӿڲ�ͬ��
	*				V1.1���ṩͳһ�ӿڹ�Ӧ�ò�ʹ�ã����ݲ�ͬЭ���ļ�����װЭ����ص����ݡ�
	************************************************************
	************************************************************
	************************************************************
**/

//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//�����豸
#include "esp8266.h"

//Э���ļ�
#include "onenet.h"
#include "edpkit.h"

//Ӳ������
#include "usart.h"
#include "delay.h"
#include "led.h"

//C��
#include <string.h>
#include <stdio.h>


EdpPacket *send_pkg;	//Э���


#define DEVID	"6580246"

#define APIKEY	"iUZqKW6xIjpGngggBH=66VCGzqg="

extern unsigned char esp8266_buf[128];


//==========================================================
//	�������ƣ�	OneNet_EDPKitCmd
//
//	�������ܣ�	EDPЭ���������
//
//	��ڲ�����	data��ƽ̨�·�������
//
//	���ز�����	��
//
//	˵����		��ʱֻ�������ӽ���
//==========================================================
_Bool OneNet_EDPKitCmd(unsigned char *data)
{

	if(data[0] == CONNRESP) //������Ӧ
	{
		UsartPrintf(USART_DEBUG, "DevLink: %d\r\n", data[3]);
		
		//0		���ӳɹ�
		//1		��֤ʧ�ܣ�Э�����
		//2		��֤ʧ�ܣ��豸ID��Ȩʧ��
		//3		��֤ʧ�ܣ�������ʧ��
		//4		��֤ʧ�ܣ��û�ID��Ȩʧ��
		//5		��֤ʧ�ܣ�δ��Ȩ
		//6		��֤ʧ�ܣ�������δ����
		//7		��֤ʧ�ܣ����豸�ѱ�����
		//8		��֤ʧ�ܣ��ظ��������������
		//9		��֤ʧ�ܣ��ظ��������������
		
		if(data[3] == 0)
			return 1;
		else
			return 0;
	}
	
	return 0;

}

//==========================================================
//	�������ƣ�	OneNet_DevLink
//
//	�������ܣ�	��onenet��������
//
//	��ڲ�����	��
//
//	���ز�����	1-�ɹ�	0-ʧ��
//
//	˵����		��onenetƽ̨��������
//==========================================================
_Bool OneNet_DevLink(void)
{

	char *dataPtr;
	_Bool status = 0;
	
	UsartPrintf(USART_DEBUG, "OneNet_DevLink\r\n"
                        "DEVID: %s,     APIKEY: %s\r\n"
                        , DEVID, APIKEY);

	send_pkg = PacketConnect1(DEVID, APIKEY);						//����devid �� apikey��װЭ���
	
	ESP8266_Clear();												//��ջ���
	ESP8266_SendData(send_pkg->_data, send_pkg->_write_pos);		//�ϴ�ƽ̨
	
	DelayXms(100);
	
	dataPtr = strchr((char *)esp8266_buf, ':');
	if(dataPtr != NULL)
	{
		dataPtr++;
		status = OneNet_EDPKitCmd((unsigned char *)dataPtr);
		
		UsartPrintf(USART_DEBUG, "����ɹ�\r\n");
	}
	else
		UsartPrintf(USART_DEBUG, "����ʧ��\r\n");
	
	DeleteBuffer(&send_pkg);										//ɾ��
	
	ESP8266_Clear();
	
	return status;

}

void OneNet_FillBuf(char *buf)
{
	
	char text[16];
	
	memset(text, 0, sizeof(text));
	
	strcpy(buf, "{");
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"Red_Led\":%d,", ledStatus.Led5Sta);
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"Green_Led\":%d,", ledStatus.Led4Sta);
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"Yellow_Led\":%d,", ledStatus.Led3Sta);
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"Blue_Led\":%d", ledStatus.Led2Sta);
	strcat(buf, text);
	
	strcat(buf, "}");

}

//==========================================================
//	�������ƣ�	OneNet_SendData
//
//	�������ܣ�	�ϴ����ݵ�ƽ̨
//
//	��ڲ�����	type���������ݵĸ�ʽ
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void OneNet_SendData(char *buf, unsigned short size)
{
	
//	OneNet_FillBuf(buf);														//��װ������
	
//	send_pkg = PacketSaveJson(NULL, buf, kTypeSimpleJsonWithoutTime);			//���-Type3
		
//	ESP8266_SendData(send_pkg->_data, send_pkg->_write_pos);					//�ϴ����ݵ�ƽ̨
	esp8266_connectip();
	ESP8266_SendData((unsigned char *)buf, size);					//�ϴ����ݵ�ƽ̨
		
//	DeleteBuffer(&send_pkg);													//ɾ��
	
}
