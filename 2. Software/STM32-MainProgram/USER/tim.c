#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "misc.h"

#include "tim.h"

void tim2_init(int PWM)    //x轴步进电机
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_Initstr;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	
	GPIO_Initstr.GPIO_Pin=GPIO_Pin_2;
	GPIO_Initstr.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Initstr.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_Initstr);

  TIM_TimeBaseStructure.TIM_Period = PWM-1;     //计数         (72M/TIM_Period*TIM_Prescaler)=频率
  TIM_TimeBaseStructure.TIM_Prescaler = 72-1;    //分频
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_Pulse=0;                //TIM_Pulse/TIM_Period=占空比*100%
	
	TIM_OC3Init(TIM2,&TIM_OCInitStructure);
	TIM_OC3PolarityConfig(TIM2,TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM2, ENABLE);
}

void tim5_init(int PWM)     //y轴步进电机
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_Initstr;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	
	GPIO_Initstr.GPIO_Pin=GPIO_Pin_3;
	GPIO_Initstr.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Initstr.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_Initstr);

  TIM_TimeBaseStructure.TIM_Period = PWM-1;     //计数         (72M/TIM_Period*TIM_Prescaler)=频率
  TIM_TimeBaseStructure.TIM_Prescaler = 72-1;    //分频
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_Pulse=0;                //TIM_Pulse/TIM_Period=占空比*100%
	
	TIM_OC4Init(TIM5,&TIM_OCInitStructure);
	TIM_OC4PolarityConfig(TIM5,TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM5, ENABLE);
}

void tim3_init()   
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  TIM_TimeBaseStructure.TIM_Period = 10000-1;     //计数
  TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;    //分频         分频/72 * 计数=中断时间
  TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1; //TIM_CKD_DIV1=0
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;  //设置溢出几次进入中断(高级定时器必须定义)
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);   //TIM2-TIM4为通用定时器，TIM5-TIM7为普通定时器TIM1和TIM8为高级定时器
	TIM_ITConfig(TIM3, TIM_IT_Update , ENABLE);
	TIM_Cmd(TIM3, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;    //中断优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //允许中断使能
  NVIC_Init(&NVIC_InitStructure);
}
