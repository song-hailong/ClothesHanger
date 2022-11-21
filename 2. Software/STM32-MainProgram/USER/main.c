#include "stm32f10x.h"
#include "function.h"

bool rain = 0;//�Ƿ�����
float illumination;//����ǿ��
u8 illu = 0;//��¼���������2Ϊ��ڣ�1Ϊ������0Ϊ��̫��
u8 humidity = 0; //ʪ��
u8 temperature = 0;  //�¶�
bool airer_state=0;//�¼�״̬��־  0Ϊ����״̬��1Ϊ���״̬
bool state_flag = 0;//����/״̬�Ƿ�ı��־
bool automatic_mode = 1;//�Զ�ģʽ��Ĭ��Ϊ��
int  automatic_time = 0;//��¼�Զ�ģʽ�ر�ʱ��

char Display[100];

int main()
{
	delay_init();
	Init_BH1750();
	Start_BH1750();			//�ϵ磬����������ݼĴ���
	DHT11_Init();	//DHT11��ʼ��	
	
	LCD_Init();
	LCD_Clear(BLUE);	
	POINT_COLOR=WHITE;//������ɫ    
	BACK_COLOR=BLUE; //������ɫ
	
	GPIO_INIT();
	tim2_init(1000);
	tim5_init(1000);
	tim3_init(); 
	uart_init(115200);
	
	//LCD_ShowString(0,10,240,240,16,(u8*)"Smart low-carbon clothes dryer");		//��ʾ����
	LCD_ShowString(0,0,240,240,16,(u8*)"Intelligent drying rack based on AIOT");		//��ʾ����
	LCD_ShowString(95,250,240,240,16,(u8*)"from : Group of 20");		//��ʾ���
	
	Airer_Reset(); //���¼�״̬��λ

	while(1)
	{
		environment();//��ȡ����״̬���ݲ���ʾ
		
		if(automatic_mode == 1 &&(rain == 1 || illu == 2)&& airer_state == 1)//����Զ�ģʽ���������ҹ�䡢�¼�Ϊ���״̬
		{
			Shrink();  //����
			airer_state=0;//�¼�Ϊ����״̬
		}
		
		if(automatic_mode == 1&&illu != 2&&rain == 0&&airer_state == 0)//����Զ�ģʽ������ҹ�䡢δ���ꡢ�¼�Ϊ����״̬
		{
			ReachOut();//���
			airer_state=1;//�¼�Ϊ���״̬
		}
		
		if(KEY_Scan(1) == 1&&airer_state == 0)//����򿪰��������¡��¼�Ϊ����״̬
		{
			ReachOut();//���
			airer_state=1;//�¼�Ϊ���״̬
			automatic_mode = 1; //���Զ�ģʽ
			state_flag = 1;//״̬�����ı�
		}
		
		if(KEY_Scan(1) == 2&&airer_state == 1)//����رհ��������¡��¼�Ϊ���״̬
		{
			Shrink();  //����
			airer_state=0;//�¼�Ϊ����״̬
			automatic_mode = 0; //�ر��Զ�ģʽ
			state_flag = 1;//״̬�����ı�
		}
		
		
		if(USART_RX_STA&0x8000)//���ڽ��յ�����
		{	
			if((USART_RX_STA&0x3fff) == 14 && USART_RX_BUF[0] == 'B' && USART_RX_BUF[13] == 'E')//�õ��˴ν��յ������ݳ���
			{
				if(USART_RX_BUF[2] - '0' != airer_state)
				{
					automatic_mode = 0; //�ر��Զ�ģʽ
					if(USART_RX_BUF[2] - '0' == 1)
					{
						ReachOut();//���
						airer_state=1;//�¼�Ϊ���״̬
					}
					if(USART_RX_BUF[2] - '0' == 0)
					{
						Shrink();  //����
						airer_state=0;//�¼�Ϊ����״̬
					}
				}
			}
			USART_RX_STA=0;	//�������״̬���	
		}
	}
}


void TIM3_IRQHandler() //��ʱ���жϺ���
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{

		if(state_flag == 1)//�������/״̬�����ı�
		{
			state_flag = 0;//�����־λ
			printf("BA%dR%dI%dH%02dT%02dE\r\n", airer_state, rain, illu, humidity, temperature);
			//delay_ms(200);
		}
		if(automatic_mode == 0)//����Զ�ģʽ�ر�,��ʼ��ʱ
		{
			automatic_time++;
			if(automatic_time == 600)//�Զ�ģʽ�ر�10���Ӻ��
			{
				automatic_mode = 1;//���Զ�ģʽ
				automatic_time = 0;//��ռ�ʱ
			}
		}

		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}
