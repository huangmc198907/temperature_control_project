#include <stdio.h>	
#include "stm32f10x.h"
#include "U2_CAN_agreement.h"
#include "CAN.H"


ErrorStatus Init_U2_CAN(void)
{
	CAN_InitTypeDef CAN_InitStruct;
	CAN_FilterInitTypeDef CAN_FilterInitStruct;
	CanTxMsg TxMessage;
	
	Clearput_CAN1();
	Clearget_CAN1();
	
	
//  Cob_ID_CAN1 = (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)?0x01:0x00)|
//	              (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)?0x02:0x00)|
//								(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)?0x04:0x00)|
//								(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)?0x08:0x00)|
//								(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)?0x10:0x00);
	Cob_ID_CAN1 = 0x06;
	/*³õÊ¼»¯*/
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
	
	
	CAN1->FMR |= 0x00000001;
	CAN1->FM1R = 0x00000001;
	CAN1->FS1R = 0x00000000;
	CAN1->FA1R = 0x00000001;
	CAN1->sFilterRegister[0].FR1 = ((((IAP_ORDER_MOSI_CHANNEL<<5)|0x0000)<<16)|(((APP_ORDER_MOSI_CHANNEL<<5)|0x0000)&0x0000ffff));
	CAN1->sFilterRegister[0].FR2 = ((((CODE_MOSI_CHANNEL<<5)|0x0000)<<16)|(((DEBUG_MOSI_CHANNEL<<5)|0x0000)&0x0000ffff));
	CAN1->FMR &= ~0x00000001;
	
	
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

	TxMessage.StdId = IAP_ORDER_MISO_CHANNEL;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.Data[0] = 1;
	TxMessage.Data[1] = 1;
	TxMessage.DLC = 2;
	
	CAN_Transmit( CAN1, &TxMessage);

	return SUCCESS;
}

ErrorStatus U2_Put_CAN(unsigned int id, char string)
{
	struct CAN_Mail put_CAN_teep;
	
	put_CAN_teep.ID = id;
	put_CAN_teep.number = 1;
	put_CAN_teep.dates[0] = string;
	Put_CAN1(&put_CAN_teep);
	
	return SUCCESS;
}

ErrorStatus U2_Printf_CAN(unsigned int id, char* string)
{
	unsigned char teep = 0;
	char* string_teep = string;
	struct CAN_Mail put_CAN_teep;
	
	put_CAN_teep.ID = id;
	
	while((NULL != *string_teep)&&(0xff > teep))
	{
		if(Checkput_CAN1()) 
		{
			for(put_CAN_teep.number = 0; (8> put_CAN_teep.number)&&(0xff > teep); teep++)
			{
				if(NULL != *string_teep) 
				{
					put_CAN_teep.dates[put_CAN_teep.number] = *string_teep;
					string_teep++;
					put_CAN_teep.number++;
				}
				else break;
			}

			Put_CAN1(&put_CAN_teep);
		}
	}
	
	return SUCCESS;
}
