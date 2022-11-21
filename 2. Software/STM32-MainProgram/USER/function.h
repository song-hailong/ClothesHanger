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

//光照阀值
#define illu_night   10  //晚上
#define illu_daytime 500  //白天
#define illu_sun     1000 //晴天

void Airer_Reset(void); //晾衣架状态复位
void ReachOut(void);//伸出
void Shrink(void);      //收缩
u8 KEY_Scan(u8);  	//按键扫描函数	
void environment(void);//获取环境状态并显示

extern bool rain;//是否下雨
extern float illumination;//光照强度
extern u8 illu;//记录光照情况，2为天黑，1为天亮，0为出太阳
extern u8 humidity; //湿度
extern u8 temperature;  //温度
extern bool airer_state;//衣架状态标志  0为收缩状态，1为伸出状态
extern bool state_flag;//数据/状态是否改变标志
extern char Display[100];

#endif 


