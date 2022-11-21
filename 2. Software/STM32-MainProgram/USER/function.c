#include "stm32f10x.h"
#include "function.h"

u32 i;
bool rain_storage = 0;//�洢�ϴ� �Ƿ�����
bool illu_storage = 0;//�洢�ϴ� �Ƿ��й���
u8 humidity_storage = 0; //�洢�ϴ� ʪ��
u8 temperature_storage = 0;  //�洢�ϴ� �¶�

void Airer_Reset() //���¼�״̬��λ
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
	airer_state=0;//�¼�Ϊ����״̬
}

void ReachOut()//���
{
	lOpen;rOpen;
	lStart;rStart;
	for(i=1;i<1600*12;i++)         //8mm*12.5=100mm
		delay_ms(1);                   //0.225��*1600=1Ȧ=8mm
	lStop;rStop;
	airer_state=1;//�¼�Ϊ���״̬
}

void Shrink()//����
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
	airer_state=0;//�¼�Ϊ����״̬
}


//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��Open_KEY�򿪰�������
//2��Close_KEY�رհ�������
//ע��˺�������Ӧ���ȼ�,Open_KEY>Close_KEY!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(Open_KEY == 1||Close_KEY == 1))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(Open_KEY==0)return Open_KEY_PRES;
		else if(Close_KEY==0)return Close_KEY_PRES;
		
	}else if(Open_KEY==0&&Open_KEY==0)key_up=1; 	    
 	return 0;// �ް�������
}


void environment()//��ȡ����״̬����ʾ
{
	rain_storage = rain;//�����ϴμ���Ƿ�����
	rain = !YL_38; //(����ʱ��YL_38����͵�ƽ)
	if(rain == 1)
		LCD_ShowString(10,100,240,240,16,(u8*)"     Weather :    Rain");	//��ʾ����
	else
		LCD_ShowString(10,100,240,240,16,(u8*)"     Weather :   No Rain");	//��ʾδ����
	if(rain != rain_storage) //����״̬�Ƿ�ı� 
		state_flag = 1;
	
	Read_BH1750();
	delay_ms(2);
	illumination=Convert_BH1750();//��ȡ����ǿ��
	illu_storage = illu;//�����ϴι���
	if(illumination<illu_night) //���ݹ�ǿ�ж�
		illu = 2;   //ҹ��
	else if(illumination >= illu_night && illumination < illu_sun)
		illu  = 1;  //����
	else if(illumination >= illu_sun)
		illu  = 0;    //����
	sprintf(Display,"Lllumination : %7.2f lx",illumination);
	LCD_ShowString(10,130,240,240,16,(u8*)Display);		//��ʾ����ǿ��
	if(illu != illu_storage) //����״̬�Ƿ�ı� 
		state_flag = 1;
	
	temperature_storage = temperature;//�����ϴ��¶�
	humidity_storage = humidity;//�����ϴ�ʪ��
	DHT11_Read_Data(&temperature,&humidity);	//��ȡ��ʪ��ֵ
	sprintf(Display,"    Humidity :    %02d %%",humidity);
	LCD_ShowString(10,160,240,240,16,(u8*)Display);		//��ʾʪ��	
	sprintf(Display," Temperature :    %02d C",temperature);
	LCD_ShowString(10,160+30,240,240,16,(u8*)Display);		//��ʾ�¶�
	if((temperature != temperature_storage) || (humidity != humidity_storage)) //��ʪ״̬�Ƿ�ı� 
		state_flag = 1;

	
	if(airer_state==0)
		LCD_ShowString(10,160+60,240,240,16,(u8*)"      Hanger :   Shrink");	//��ʾ�¼�״̬
	else
		LCD_ShowString(10,160+60,240,240,16,(u8*)"      Hanger :  ReachOut");	//��ʾ�¼�״̬
}

