#include "stm32f10x.h"
#include "gpio.h"

void GPIO_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOG, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
	
	/*��������ڳ�ʼ��*/
	//l �� ENA(����ѻ�������)��PA7  DIR(���������ת������)��PA5  PUL(�����ź�����)��PA3(TIM5->CCR4)
	//r �� ENA(����ѻ�������)��PA6  DIR(���������ת������)��PA4  PUL(�����ź�����)��PA2(TIM2->CCR3)
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_ResetBits(GPIOA, GPIO_Pin_6|GPIO_Pin_7);//���ͣ��ر��ѻ����ܣ�������ʱ�������
	
	/*���ģ�����ݿڳ�ʼ��*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	/*�����ڳ�ʼ��*/
	//�� PG9
	//�� PG10
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;//���ó���������
	GPIO_Init(GPIOG, &GPIO_InitStruct);
	
	/*��λ���ؿڳ�ʼ��*/
	//l �� PB5
	//r �� PB7
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;//���ó���������
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_ResetBits(GPIOB, GPIO_Pin_7|GPIO_Pin_5); //������λ���ؿ�
}
