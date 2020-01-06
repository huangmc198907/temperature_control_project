/**
  ******************************************************************************
  * @file    ADC/3ADCs_DMA/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "USART.h"
#include "system_Init.h"
#include "stm32f10x.h"
#include "CAN.H"
#include "U2_CAN_agreement.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup ADC_3ADCs_DMA
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint16_t ADC2ConvertedValue;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/
 
/**
  * @brief  This function handles ADC1 and ADC2 global interrupts requests.
  * @param  None
  * @retval None
  */
/*void ADC1_2_IRQHandler(void)
{
  // Get injected channel13 converted value 
  //ADC2ConvertedValue = ADC_GetConversionValue(ADC2);
} */

/**/

#if defined USED_USART1
void USART1_IRQHandler(void)
{
	extern uint16_t puttop_USART1;
	extern uint16_t putend_USART1;
	extern uint16_t putall_USART1;
	extern uint8_t putbuff_USART1[PUTBUFFS_USART1];
	extern uint16_t gettop_USART1;
	extern uint16_t getend_USART1;
	extern uint16_t getall_USART1;
	extern uint8_t getbuff_USART1[GETBUFFS_USART1];
	
	if(USART_GetFlagStatus( USART1, USART_FLAG_RXNE) == SET)
	{
		if(getall_USART1 < GETBUFFS_USART1)
		{
			getbuff_USART1[ gettop_USART1] = USART_ReceiveData( USART1);
			getall_USART1++;
			if( gettop_USART1 < ( GETBUFFS_USART1 - 1)) gettop_USART1++;
			else gettop_USART1 = 0;
		}
		else USART_ReceiveData( USART1);

		USART_ClearFlag( USART1, USART_FLAG_RXNE);
	}
	if(USART_GetFlagStatus( USART1, USART_FLAG_TXE) == SET)
	{
		if(putall_USART1)
		{
			USART_SendData( USART1, putbuff_USART1[ putend_USART1]);
			putall_USART1--;
			if( putend_USART1 < ( PUTBUFFS_USART1 - 1)) putend_USART1++;
			else putend_USART1 = 0;

			USART_ClearFlag( USART1, USART_FLAG_TXE);
		}
		else USART_ITConfig( USART1, USART_IT_TXE, DISABLE);
	}
}
#endif

#if defined USED_USART2
void USART2_IRQHandler(void)
{
	extern uint16_t puttop_USART2;
	extern uint16_t putend_USART2;
	extern uint16_t putall_USART2;
	extern uint8_t putbuff_USART2[PUTBUFFS_USART2];
	extern uint16_t gettop_USART2;
	extern uint16_t getend_USART2;
	extern uint16_t getall_USART2;
	extern uint8_t getbuff_USART2[GETBUFFS_USART2];
	
	if(USART_GetFlagStatus( USART2, USART_FLAG_RXNE) == SET)
	{
		if(getall_USART2 < GETBUFFS_USART2)
		{
			getbuff_USART2[ gettop_USART2] = USART_ReceiveData( USART2);
			getall_USART2++;
			if( gettop_USART2 < ( GETBUFFS_USART2 - 1)) gettop_USART2++;
			else gettop_USART2 = 0;
		}
		else USART_ReceiveData( USART2);

		USART_ClearFlag( USART2, USART_FLAG_RXNE);
	}
	if(USART_GetFlagStatus( USART2, USART_FLAG_TXE) == SET)
	{
		if(putall_USART2)
		{
			USART_SendData( USART2, putbuff_USART2[ putend_USART2]);
			putall_USART2--;
			if( putend_USART2 < ( PUTBUFFS_USART2 - 1)) putend_USART2++;
			else putend_USART2 = 0;

			USART_ClearFlag( USART2, USART_FLAG_TXE);
		}
		else USART_ITConfig( USART2, USART_IT_TXE, DISABLE);
	}
} 
#endif

#if defined USED_USART3
void USART3_IRQHandler(void)
{
	extern uint16_t puttop_USART3;
	extern uint16_t putend_USART3;
	extern uint16_t putall_USART3;
	extern uint8_t putbuff_USART3[PUTBUFFS_USART3];
	extern uint16_t gettop_USART3;
	extern uint16_t getend_USART3;
	extern uint16_t getall_USART3;
	extern uint8_t getbuff_USART3[GETBUFFS_USART3];
	
	if(USART_GetFlagStatus( USART3, USART_FLAG_RXNE) == SET)
	{
		if(getall_USART3 < GETBUFFS_USART3)
		{
			getbuff_USART3[ gettop_USART3] = USART_ReceiveData( USART3);
			getall_USART3++;
			if( gettop_USART3 < ( GETBUFFS_USART3 - 1)) gettop_USART3++;
			else gettop_USART3 = 0;
		}
		else USART_ReceiveData( USART3);

		USART_ClearFlag( USART3, USART_FLAG_RXNE);
	}
	if(USART_GetFlagStatus( USART3, USART_FLAG_TXE) == SET)
	{
		if(putall_USART3)
		{
			USART_SendData( USART3, putbuff_USART3[ putend_USART3]);
			putall_USART3--;
			if( putend_USART3 < ( PUTBUFFS_USART3 - 1)) putend_USART3++;
			else putend_USART3 = 0;

			USART_ClearFlag( USART3, USART_FLAG_TXE);
		}
		else USART_ITConfig( USART3, USART_IT_TXE, DISABLE);
	}
} 
#endif

#if defined USED_CAN1
void USB_HP_CAN1_TX_IRQHandler(void)
{
	extern uint16_t puttop_CAN1;
	extern uint16_t putend_CAN1;
	extern uint16_t putall_CAN1;
	extern struct CAN_Mail putbuff_CAN1[PUTBUFF_CAN1];
	
	CanTxMsg TxMessage;
	unsigned char teep;
	
	if(CAN_GetFlagStatus( CAN1, CAN_FLAG_RQCP0) == SET)
	{
		if(putall_CAN1)
		{
			TxMessage.StdId = putbuff_CAN1[ putend_CAN1].ID;
			TxMessage.IDE = CAN_ID_STD;
			TxMessage.RTR = CAN_RTR_DATA;
			TxMessage.DLC = putbuff_CAN1[ putend_CAN1].number;
			for(teep = 0; teep < putbuff_CAN1[ putend_CAN1].number; teep++)
			{
				TxMessage.Data[teep] = putbuff_CAN1[ putend_CAN1].dates[teep];
			}
			putall_CAN1--;
			if( putend_CAN1 < ( PUTBUFF_CAN1 - 1)) putend_CAN1++;
			else putend_CAN1 = 0;
			
			CAN_ClearFlag( CAN1, CAN_FLAG_RQCP0);
			CAN_Transmit( CAN1, &TxMessage);
		}
		else CAN_ITConfig( CAN1, CAN_IT_TME, DISABLE);
	}
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{

	extern uint16_t gettop_CAN1;
	extern uint16_t getend_CAN1;
	extern uint16_t getall_CAN1;
	extern struct CAN_Mail getbuff_CAN1[GETBUFF_CAN1];
	
	CanRxMsg RxMessage;
	unsigned char teep;
	
	if(CAN_GetFlagStatus( CAN1, CAN_FLAG_FMP0) == SET)
	{
		CAN_Receive( CAN1, CAN_FIFO0, &RxMessage);
		if(getall_CAN1 < (GETBUFF_CAN1-8))
		{
			getbuff_CAN1[ gettop_CAN1].ID = RxMessage.StdId;
			getbuff_CAN1[ gettop_CAN1].number = RxMessage.DLC;
			for(teep = 0; teep < RxMessage.DLC; teep++)
			{
				getbuff_CAN1[ gettop_CAN1].dates[teep] = RxMessage.Data[teep];
			}
			getall_CAN1++;
			if( gettop_CAN1 < ( GETBUFF_CAN1 - 1)) gettop_CAN1++;
			else gettop_CAN1 = 0;
		}
			
		CAN_ClearFlag( CAN1, CAN_FLAG_FMP0);
	}
}
#endif

void TIM2_IRQHandler(void)
{
	extern unsigned int out_time_YMoDem;
	extern unsigned int time_1ms_main;
  extern unsigned int out_time_IIC;
	static unsigned int time_ms = 0;
	
	if( TIM_GetFlagStatus(TIM2 , TIM_FLAG_Update) == SET)
	{
		time_ms++;
		if(1000 <= time_ms)
		{
			time_ms = 0;
			
		}
		
		if(time_1ms_main) time_1ms_main--;
		if(out_time_YMoDem) out_time_YMoDem--;
		if(out_time_IIC) out_time_IIC--;
		
		TIM_ClearFlag( TIM2, TIM_FLAG_Update);
	}
}


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
