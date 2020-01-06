#include "system_Init.h"
#include "stm32f10x.h"
#include "USART.h"
#include "U2_CAN_agreement.h"
#include "MCP3204_8.H"
#include "AT24C0X.H"


void Init_system(void)
{
	NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x06000);
	Init_RCC();				IWDG_ReloadCounter();
	Init_NVIC();			IWDG_ReloadCounter();
	Init_GPIO();			IWDG_ReloadCounter();
	
	Init_USART2();
	Init_U2_CAN();
	
	Init_MCP3204_8();
	Init_AT24C0X();
}

ErrorStatus Init_RCC(void)
{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
//	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_DMA1, ENABLE);
	
	RCC_PCLK1Config(RCC_HCLK_Div4);
//	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE);	//启用TIM2时钟
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_CAN1, ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE );
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE );
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C1, ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE);
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 
//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE); 
//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE); 
//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE, ENABLE); 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO,  ENABLE);
//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1, ENABLE);
//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE);
	
	
	//定时器2初始化
//	TIM_DeInit( TIM2);//将外设TIMx寄存器重设为缺省值
//	TIM_TimeBaseInitStruct.TIM_Period = 72;   //自动重装载寄存器周期的值   1ms
//	TIM_TimeBaseInitStruct.TIM_Prescaler = 1000 - 1; //TIMx时钟频率除数的预分频值
//	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; //设置了时钟分割
//	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//TIM向上计数模式
//	TIM_TimeBaseInit( TIM2, &TIM_TimeBaseInitStruct);
//	
//	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//清中断标志位
//	TIM_ITConfig( TIM2, TIM_IT_Update, ENABLE);//使能指定的TIM中断
//	TIM_Cmd( TIM2, ENABLE);//能或者失能TIMx外设
	

	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	IWDG_SetReload(1250);
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable);
	IWDG_ReloadCounter();
	IWDG_Enable();


	SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK );
	
	return SUCCESS;
}

ErrorStatus Init_NVIC(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;

	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2);
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2);
	NVIC_InitStruct.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2);
	NVIC_InitStruct.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
//	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2);
//	NVIC_InitStruct.NVIC_IRQChannel = SPI2_IRQn;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStruct);
	
	return SUCCESS;
}

ErrorStatus Close_it(void)
{
//	NVIC_InitTypeDef NVIC_InitStruct;
	
//	TIM_Cmd( TIM2, DISABLE);//能或者失能TIMx外设
//	TIM_ITConfig( TIM2, TIM_IT_Update, DISABLE);//使能指定的TIM中断
//	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//清中断标志位
//	
//	USART_Cmd( USART3, DISABLE);
//	USART_ITConfig( USART3, USART_IT_TXE, DISABLE);
//	USART_ITConfig( USART3, USART_IT_RXNE, DISABLE);
//	
//	CAN_Sleep(CAN1);
//	CAN_ITConfig( CAN1, CAN_IT_TME, DISABLE);
//	CAN_ITConfig( CAN1, CAN_IT_FMP0, DISABLE);
//	
//	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2);
//	NVIC_InitStruct.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStruct.NVIC_IRQChannelCmd = DISABLE;
//	NVIC_Init(&NVIC_InitStruct);
//	
//	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2);
//	NVIC_InitStruct.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStruct.NVIC_IRQChannelCmd = DISABLE;
//	NVIC_Init(&NVIC_InitStruct);
//	
//	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2);
//	NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStruct.NVIC_IRQChannelCmd = DISABLE;
//	NVIC_Init(&NVIC_InitStruct);
//	
//	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_0);
//	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStruct.NVIC_IRQChannelCmd = DISABLE;
//	NVIC_Init(&NVIC_InitStruct);
	
	return SUCCESS;
}

ErrorStatus Init_GPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
//  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	
	/* Configure USART2 */
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
  /* Configure CAN1 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Configure I2C1 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Configure SPI2 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	return SUCCESS;
}
