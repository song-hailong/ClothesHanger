#ifndef __GPIO_H
#define __GPIO_H

void GPIO_INIT(void);

#define lStart TIM5->CCR4=500  //开始  PA3
#define lStop  TIM5->CCR4=0    //停止
#define lClose GPIO_ResetBits(GPIOA, GPIO_Pin_5)   //DIR
#define lOpen  GPIO_SetBits(GPIOA, GPIO_Pin_5)
#define lLimit GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5) //限位开关

#define rStart TIM2->CCR3=500  //开始  PA2
#define rStop  TIM2->CCR3=0    //停止
#define rClose GPIO_ResetBits(GPIOA, GPIO_Pin_4)   //DIR
#define rOpen  GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define rLimit GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) //限位开关

#define YL_38 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)


#define Open_KEY  GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9)//读取打开按键
#define Close_KEY  GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_10)//读取关闭按键

#define Open_KEY_PRES 	1	//打开按键按下
#define Close_KEY_PRES	2	//关闭按键按下


#endif

