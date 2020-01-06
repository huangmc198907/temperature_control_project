/**
	************************************************************
	************************************************************
	************************************************************
	*	文件名： 	onenet.c
	*
	*	作者： 		张继瑞
	*
	*	日期： 		2017-05-08
	*
	*	版本： 		V1.1
	*
	*	说明： 		与onenet平台的数据交互接口层
	*
	*	修改记录：	V1.0：协议封装、返回判断都在同一个文件，并且不同协议接口不同。
	*				V1.1：提供统一接口供应用层使用，根据不同协议文件来封装协议相关的内容。
	************************************************************
	************************************************************
	************************************************************
**/

//单片机头文件
#include "stm32f10x.h"

//网络设备
#include "esp8266.h"

//协议文件
#include "onenet.h"
#include "edpkit.h"

//硬件驱动
#include "usart.h"
#include "delay.h"
#include "led.h"

//C库
#include <string.h>
#include <stdio.h>


EdpPacket *send_pkg;	//协议包


#define DEVID	"6580246"

#define APIKEY	"iUZqKW6xIjpGngggBH=66VCGzqg="

extern unsigned char esp8266_buf[128];


//==========================================================
//	函数名称：	OneNet_EDPKitCmd
//
//	函数功能：	EDP协议命令解析
//
//	入口参数：	data：平台下发的数据
//
//	返回参数：	无
//
//	说明：		暂时只做了连接解析
//==========================================================
_Bool OneNet_EDPKitCmd(unsigned char *data)
{

	if(data[0] == CONNRESP) //连接响应
	{
		UsartPrintf(USART_DEBUG, "DevLink: %d\r\n", data[3]);
		
		//0		连接成功
		//1		验证失败：协议错误
		//2		验证失败：设备ID鉴权失败
		//3		验证失败：服务器失败
		//4		验证失败：用户ID鉴权失败
		//5		验证失败：未授权
		//6		验证失败：激活码未分配
		//7		验证失败：该设备已被激活
		//8		验证失败：重复发送连接请求包
		//9		验证失败：重复发送连接请求包
		
		if(data[3] == 0)
			return 1;
		else
			return 0;
	}
	
	return 0;

}

//==========================================================
//	函数名称：	OneNet_DevLink
//
//	函数功能：	与onenet创建连接
//
//	入口参数：	无
//
//	返回参数：	1-成功	0-失败
//
//	说明：		与onenet平台建立连接
//==========================================================
_Bool OneNet_DevLink(void)
{

	char *dataPtr;
	_Bool status = 0;
	
	UsartPrintf(USART_DEBUG, "OneNet_DevLink\r\n"
                        "DEVID: %s,     APIKEY: %s\r\n"
                        , DEVID, APIKEY);

	send_pkg = PacketConnect1(DEVID, APIKEY);						//根据devid 和 apikey封装协议包
	
	ESP8266_Clear();												//清空缓存
	ESP8266_SendData(send_pkg->_data, send_pkg->_write_pos);		//上传平台
	
	DelayXms(100);
	
	dataPtr = strchr((char *)esp8266_buf, ':');
	if(dataPtr != NULL)
	{
		dataPtr++;
		status = OneNet_EDPKitCmd((unsigned char *)dataPtr);
		
		UsartPrintf(USART_DEBUG, "接入成功\r\n");
	}
	else
		UsartPrintf(USART_DEBUG, "接入失败\r\n");
	
	DeleteBuffer(&send_pkg);										//删包
	
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
//	函数名称：	OneNet_SendData
//
//	函数功能：	上传数据到平台
//
//	入口参数：	type：发送数据的格式
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OneNet_SendData(char *buf, unsigned short size)
{
	
//	OneNet_FillBuf(buf);														//封装数据流
	
//	send_pkg = PacketSaveJson(NULL, buf, kTypeSimpleJsonWithoutTime);			//封包-Type3
		
//	ESP8266_SendData(send_pkg->_data, send_pkg->_write_pos);					//上传数据到平台
	esp8266_connectip();
	ESP8266_SendData((unsigned char *)buf, size);					//上传数据到平台
		
//	DeleteBuffer(&send_pkg);													//删包
	
}
