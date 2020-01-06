#include "CAN.H"
#include "stm32f10x.h"
#include <stdio.h>

#if defined USED_CAN1
uint16_t Cob_ID_CAN1 = 0;
uint16_t puttop_CAN1;
uint16_t putend_CAN1;
uint16_t putall_CAN1;
struct CAN_Mail  putbuff_CAN1[PUTBUFF_CAN1];
uint16_t gettop_CAN1;
uint16_t getend_CAN1;
uint16_t getall_CAN1;
struct CAN_Mail  getbuff_CAN1[GETBUFF_CAN1];
#endif


#if defined USED_CAN1
ErrorStatus Init_CAN1(void)
{
	CAN_InitTypeDef CAN_InitStruct;
	CAN_FilterInitTypeDef CAN_FilterInitStruct;
	CanTxMsg TxMessage;
	
	Clearput_CAN1();
	Clearget_CAN1();

	/*≥ı ºªØ*/
	CAN_FilterInitStruct.CAN_FilterNumber = 1;
	CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStruct.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStruct.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStruct.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStruct.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
	CAN_FilterInitStruct.CAN_FilterActivation = ENABLE;
//	CAN_FilterInitStruct.CAN_FilterActivation = DISABLE;
	CAN_FilterInit(&CAN_FilterInitStruct);
	
	CAN_StructInit( &CAN_InitStruct);
	CAN_InitStruct.CAN_TTCM = DISABLE;
	CAN_InitStruct.CAN_ABOM = ENABLE;
	CAN_InitStruct.CAN_AWUM = DISABLE;
	CAN_InitStruct.CAN_NART = DISABLE;
	CAN_InitStruct.CAN_RFLM = ENABLE;
	CAN_InitStruct.CAN_TXFP = ENABLE;
	CAN_InitStruct.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStruct.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStruct.CAN_BS1 = CAN_BS1_9tq;
	CAN_InitStruct.CAN_BS2 = CAN_BS2_8tq;
	CAN_InitStruct.CAN_Prescaler = 2;
	CAN_Init( CAN1, &CAN_InitStruct);
	
	CAN_DBGFreeze( CAN1, ENABLE);
	CAN_ITConfig( CAN1, CAN_IT_TME, ENABLE);
	CAN_ITConfig( CAN1, CAN_IT_FMP0, ENABLE);

	TxMessage.StdId = 0x0000;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.Data[0] = 0;
	TxMessage.DLC = 0;
	CAN_Transmit( CAN1, &TxMessage);

	return SUCCESS;
}

void Clearput_CAN1(void)
{
	unsigned int teep,teep1;
	
	puttop_CAN1 = 0;
	putend_CAN1 = 0;
	putall_CAN1 = 0;
	
	for(teep = 0; teep < PUTBUFF_CAN1; teep++)
	{
		putbuff_CAN1[teep].ID = 0;
		putbuff_CAN1[teep].number = 0;
		
		for(teep1=0; teep1<8; teep1++)
			putbuff_CAN1[teep].dates[teep1] = 0;
	}
}

uint16_t Checkput_CAN1(void)
{
	return (PUTBUFF_CAN1-putall_CAN1);	
}

ErrorStatus Put_CAN1(struct CAN_Mail* put_CAN1)
{
	unsigned int teep;
	
	if( putall_CAN1 < PUTBUFF_CAN1) 
	{
		CAN_ITConfig( CAN1, CAN_IT_TME, DISABLE);
		
		putbuff_CAN1[ puttop_CAN1].ID = put_CAN1->ID;
		putbuff_CAN1[ puttop_CAN1].number = put_CAN1->number;
		for(teep=0; teep<put_CAN1->number; teep++)
			putbuff_CAN1[ puttop_CAN1].dates[teep] = put_CAN1->dates[teep];
		
		putall_CAN1++;
		if( puttop_CAN1 < ( PUTBUFF_CAN1 - 1)) puttop_CAN1++;
		else puttop_CAN1 = 0;
		
		CAN_ITConfig( CAN1, CAN_IT_TME, ENABLE);
		
		return SUCCESS;
	}
	
	return ERROR;
}

void Clearget_CAN1(void)
{
	unsigned int teep,teep1;
	
	gettop_CAN1 = 0;
	getend_CAN1 = 0;
	getall_CAN1 = 0;
	
	for(teep = 0; teep < GETBUFF_CAN1; teep++)
	{
		getbuff_CAN1[teep].ID = 0;
		getbuff_CAN1[teep].number = 0;
		
		for(teep1=0; teep1<8; teep1++)
			getbuff_CAN1[teep].dates[teep1] = 0;
	}
}

uint16_t Checkget_CAN1(void)
{
	return getall_CAN1; 
}

ErrorStatus CheckgetID_CAN1(unsigned int id)
{
	uint8_t teep=0;

	while( getall_CAN1 > 0) 
	{
		CAN_ITConfig( CAN1, CAN_IT_FMP0, DISABLE);
		
		if(id == getbuff_CAN1[ getend_CAN1].ID) return SUCCESS;
		
		getall_CAN1--;
		if(getend_CAN1 < ( GETBUFF_CAN1 - 1)) getend_CAN1++;
		else getend_CAN1 = 0;
		
		CAN_ITConfig( CAN1, CAN_IT_FMP0, ENABLE);
	}
	
	return ERROR;
}

ErrorStatus Get_CAN1(struct CAN_Mail* get_CAN1)
{
	uint8_t teep=0;

	if( getall_CAN1 > 0) 
	{
		CAN_ITConfig( CAN1, CAN_IT_FMP0, DISABLE);
		
		get_CAN1->ID = getbuff_CAN1[ getend_CAN1].ID;
		get_CAN1->number = getbuff_CAN1[ getend_CAN1].number;
		for(teep=0; teep<getbuff_CAN1[ getend_CAN1].number; teep++)
			get_CAN1->dates[teep] = getbuff_CAN1[ getend_CAN1].dates[teep];
		
		getall_CAN1--;
		if(getend_CAN1 < ( GETBUFF_CAN1 - 1)) getend_CAN1++;
		else getend_CAN1 = 0;
		
		CAN_ITConfig( CAN1, CAN_IT_FMP0, ENABLE);
		
		return SUCCESS;
	}
	
	get_CAN1 = NULL;
	return ERROR;
}
#endif
