#include "stm32f10x.h"
#include "function.h"

u32 i;
bool rain_storage = 0;//存储上次 是否下雨
bool illu_storage = 0;//存储上次 是否有光照
u8 humidity_storage = 0; //存储上次 湿度
u8 temperature_storage = 0;  //存储上次 温度

void Airer_Reset() //晾衣架状态复位
{
	lClose;rClose;
	lStart;rStart;
	while(lLimit||rLimit)
	{
		if(lLimit == 0)
			lStop;
		if(rLimit == 0)
			rStop;
	}
	lStop;rStop;
	airer_state=0;//衣架为收缩状态
}

void ReachOut()//伸出
{
	lOpen;rOpen;
	lStart;rStart;
	for(i=1;i<1600*12;i++)         //8mm*12.5=100mm
		delay_ms(1);                   //0.225°*1600=1圈=8mm
	lStop;rStop;
	airer_state=1;//衣架为伸出状态
}

void Shrink()//收缩
{
	lClose;rClose;
	lStart;rStart;
//	for(i=1;i<1600*8.75;i++)
//		delay_ms(1);
//	lStop;rStop;
	
	while(lLimit||rLimit)
	{
		if(lLimit == 0)
			lStop;
		if(rLimit == 0)
			rStop;
	}
	lStop;rStop;
	airer_state=0;//衣架为收缩状态
}


//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，Open_KEY打开按键按下
//2，Close_KEY关闭按键按下
//注意此函数有响应优先级,Open_KEY>Close_KEY!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(Open_KEY == 1||Close_KEY == 1))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(Open_KEY==0)return Open_KEY_PRES;
		else if(Close_KEY==0)return Close_KEY_PRES;
		
	}else if(Open_KEY==0&&Open_KEY==0)key_up=1; 	    
 	return 0;// 无按键按下
}


void environment()//获取环境状态并显示
{
	rain_storage = rain;//储存上次检测是否下雨
	rain = !YL_38; //(下雨时，YL_38输出低电平)
	if(rain == 1)
		LCD_ShowString(10,100,240,240,16,(u8*)"     Weather :    Rain");	//显示下雨
	else
		LCD_ShowString(10,100,240,240,16,(u8*)"     Weather :   No Rain");	//显示未下雨
	if(rain != rain_storage) //下雨状态是否改变 
		state_flag = 1;
	
	Read_BH1750();
	delay_ms(2);
	illumination=Convert_BH1750();//获取光照强度
	illu_storage = illu;//储存上次光照
	if(illumination<illu_night) //根据光强判断
		illu = 2;   //夜间
	else if(illumination >= illu_night && illumination < illu_sun)
		illu  = 1;  //白天
	else if(illumination >= illu_sun)
		illu  = 0;    //晴天
	sprintf(Display,"Lllumination : %7.2f lx",illumination);
	LCD_ShowString(10,130,240,240,16,(u8*)Display);		//显示光照强度
	if(illu != illu_storage) //光照状态是否改变 
		state_flag = 1;
	
	temperature_storage = temperature;//储存上次温度
	humidity_storage = humidity;//储存上次湿度
	DHT11_Read_Data(&temperature,&humidity);	//读取温湿度值
	sprintf(Display,"    Humidity :    %02d %%",humidity);
	LCD_ShowString(10,160,240,240,16,(u8*)Display);		//显示湿度	
	sprintf(Display," Temperature :    %02d C",temperature);
	LCD_ShowString(10,160+30,240,240,16,(u8*)Display);		//显示温度
	if((temperature != temperature_storage) || (humidity != humidity_storage)) //温湿状态是否改变 
		state_flag = 1;

	
	if(airer_state==0)
		LCD_ShowString(10,160+60,240,240,16,(u8*)"      Hanger :   Shrink");	//显示衣架状态
	else
		LCD_ShowString(10,160+60,240,240,16,(u8*)"      Hanger :  ReachOut");	//显示衣架状态
}

