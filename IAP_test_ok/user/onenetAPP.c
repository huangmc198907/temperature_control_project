/**
	************************************************************
	************************************************************
	************************************************************
	*	文件名： 	onenetAPP.c
	*
	*	作者： 		张继瑞
	*
	*	日期： 		2017-02-27
	*
	*	版本： 		V1.0
	*
	*	说明： 		onenet平台下发数据的处理
	*
	*	修改记录：	
	************************************************************
	************************************************************
	************************************************************
**/

//协议层
#include "onenet.h"

//应用
#include "onenetAPP.h"

//网络设备驱动
#include "esp8266.h"

//硬件驱动
#include "usart.h"
#include "led.h"

//C库
#include <string.h>
#include <stdlib.h>






//==========================================================
//	函数名称：	OneNet_Replace
//
//	函数功能：	替换平台下发命令中的结束符
//
//	入口参数：	res：平台下发的命令
//
//	返回参数：	无
//
//	说明：		平台开关、旋钮下发的命令会有固定4个结束符，替换为其他不常用字符，以方便string库解析
//==========================================================
void OneNet_Replace(unsigned char *res, unsigned char num)
{
	
	unsigned char count = 0;

	while(count < num)
	{
		if(*res == '\0')		//找到结束符
		{
			*res = '~';			//替换
			count++;
		}
		
		res++;
	}

}


//==========================================================
//	函数名称：	OneNet_Event
//
//	函数功能：	平台返回数据检测
//
//	入口参数：	dataPtr：平台返回的数据
//
//	返回参数：	无
//
//	说明：		
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
	if(cmd[0] == 0xA0)									//命令下发功能接收的数据
		OneNet_Replace(cmd, 4);
	else
		return;
		
	dataPtr = strstr((const char *)cmd, "}");			//搜索'}'
	
	if(dataPtr != NULL)									//如果找到了
	{
		dataPtr++;
		
		while(*dataPtr >= '0' && *dataPtr <= '9')		//判断是否是下发的命令控制数据
		{
			numBuf[num++] = *dataPtr++;
		}
		
		num = atoi((const char *)numBuf);				//转为数值形式
		
		if(strstr((char *)cmd, "redled"))				//搜索"redled"
		{
			if(num == 1)								//控制数据如果为1，代表开
			{
				Led5_Set(LED_ON);
			}
			else if(num == 0)							//控制数据如果为0，代表关
			{
				Led5_Set(LED_OFF);
			}
		}
														//下同
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
	
	ESP8266_Clear();									//清空缓存

}
