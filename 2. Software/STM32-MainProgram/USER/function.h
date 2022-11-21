#ifndef __FUNCTION_H
#define __FUNCTION_H

#include "i2c.h"
#include "gpio.h"
#include "usart.h"
#include "bh1750.h"
#include "lcd.h"
#include "delay.h"	
#include "tim.h"
#include "dht11.h"
#include "stdbool.h"
#include "stdio.h"

//���շ�ֵ
#define illu_night   10  //����
#define illu_daytime 500  //����
#define illu_sun     1000 //����

void Airer_Reset(void); //���¼�״̬��λ
void ReachOut(void);//���
void Shrink(void);      //����
u8 KEY_Scan(u8);  	//����ɨ�躯��	
void environment(void);//��ȡ����״̬����ʾ

extern bool rain;//�Ƿ�����
extern float illumination;//����ǿ��
extern u8 illu;//��¼���������2Ϊ��ڣ�1Ϊ������0Ϊ��̫��
extern u8 humidity; //ʪ��
extern u8 temperature;  //�¶�
extern bool airer_state;//�¼�״̬��־  0Ϊ����״̬��1Ϊ���״̬
extern bool state_flag;//����/״̬�Ƿ�ı��־
extern char Display[100];

#endif 


