#include "stm32f10x.h"
#include "gpio.h"

void GPIO_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOG, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
	
	/*电机驱动口初始化*/
	//l 左 ENA(电机脱机控制正)：PA7  DIR(电机正、反转控制正)：PA5  PUL(脉冲信号输入)：PA3(TIM5->CCR4)
	//r 右 ENA(电机脱机控制正)：PA6  DIR(电机正、反转控制正)：PA4  PUL(脉冲信号输入)：PA2(TIM2->CCR3)
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_ResetBits(GPIOA, GPIO_Pin_6|GPIO_Pin_7);//拉低，关闭脱机功能，无脉冲时电机锁紧
	
	/*雨滴模块数据口初始化*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD; //设置成下拉输入
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	/*按键口初始化*/
	//开 PG9
	//关 PG10
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;//设置成上拉输入
	GPIO_Init(GPIOG, &GPIO_InitStruct);
	
	/*限位开关口初始化*/
	//l 左 PB5
	//r 右 PB7
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;//设置成上拉输入
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_ResetBits(GPIOB, GPIO_Pin_7|GPIO_Pin_5); //拉低限位开关口
}
