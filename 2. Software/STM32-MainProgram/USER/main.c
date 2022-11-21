#include "stm32f10x.h"
#include "function.h"

bool rain = 0;//是否下雨
float illumination;//光照强度
u8 illu = 0;//记录光照情况，2为天黑，1为天亮，0为出太阳
u8 humidity = 0; //湿度
u8 temperature = 0;  //温度
bool airer_state=0;//衣架状态标志  0为收缩状态，1为伸出状态
bool state_flag = 0;//数据/状态是否改变标志
bool automatic_mode = 1;//自动模式，默认为开
int  automatic_time = 0;//记录自动模式关闭时间

char Display[100];

int main()
{
	delay_init();
	Init_BH1750();
	Start_BH1750();			//上电，设置清除数据寄存器
	DHT11_Init();	//DHT11初始化	
	
	LCD_Init();
	LCD_Clear(BLUE);	
	POINT_COLOR=WHITE;//画笔颜色    
	BACK_COLOR=BLUE; //背景颜色
	
	GPIO_INIT();
	tim2_init(1000);
	tim5_init(1000);
	tim3_init(); 
	uart_init(115200);
	
	//LCD_ShowString(0,10,240,240,16,(u8*)"Smart low-carbon clothes dryer");		//显示标题
	LCD_ShowString(0,0,240,240,16,(u8*)"Intelligent drying rack based on AIOT");		//显示标题
	LCD_ShowString(95,250,240,240,16,(u8*)"from : Group of 20");		//显示组号
	
	Airer_Reset(); //晾衣架状态复位

	while(1)
	{
		environment();//获取环境状态数据并显示
		
		if(automatic_mode == 1 &&(rain == 1 || illu == 2)&& airer_state == 1)//如果自动模式开、下雨或夜间、衣架为伸出状态
		{
			Shrink();  //收缩
			airer_state=0;//衣架为收缩状态
		}
		
		if(automatic_mode == 1&&illu != 2&&rain == 0&&airer_state == 0)//如果自动模式开、非夜间、未下雨、衣架为收缩状态
		{
			ReachOut();//伸出
			airer_state=1;//衣架为伸出状态
		}
		
		if(KEY_Scan(1) == 1&&airer_state == 0)//如果打开按键被按下、衣架为收缩状态
		{
			ReachOut();//伸出
			airer_state=1;//衣架为伸出状态
			automatic_mode = 1; //打开自动模式
			state_flag = 1;//状态发生改变
		}
		
		if(KEY_Scan(1) == 2&&airer_state == 1)//如果关闭按键被按下、衣架为伸出状态
		{
			Shrink();  //收缩
			airer_state=0;//衣架为收缩状态
			automatic_mode = 0; //关闭自动模式
			state_flag = 1;//状态发生改变
		}
		
		
		if(USART_RX_STA&0x8000)//串口接收到数据
		{	
			if((USART_RX_STA&0x3fff) == 14 && USART_RX_BUF[0] == 'B' && USART_RX_BUF[13] == 'E')//得到此次接收到的数据长度
			{
				if(USART_RX_BUF[2] - '0' != airer_state)
				{
					automatic_mode = 0; //关闭自动模式
					if(USART_RX_BUF[2] - '0' == 1)
					{
						ReachOut();//伸出
						airer_state=1;//衣架为伸出状态
					}
					if(USART_RX_BUF[2] - '0' == 0)
					{
						Shrink();  //收缩
						airer_state=0;//衣架为收缩状态
					}
				}
			}
			USART_RX_STA=0;	//清楚接收状态标记	
		}
	}
}


void TIM3_IRQHandler() //定时器中断函数
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{

		if(state_flag == 1)//如果数据/状态发生改变
		{
			state_flag = 0;//清除标志位
			printf("BA%dR%dI%dH%02dT%02dE\r\n", airer_state, rain, illu, humidity, temperature);
			//delay_ms(200);
		}
		if(automatic_mode == 0)//如果自动模式关闭,开始计时
		{
			automatic_time++;
			if(automatic_time == 600)//自动模式关闭10分钟后打开
			{
				automatic_mode = 1;//打开自动模式
				automatic_time = 0;//清空计时
			}
		}

		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}
