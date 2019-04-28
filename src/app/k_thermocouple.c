#include "k_thermocouple.h"
#include "k_thermocouple_drv.h"
#include "tft_drv.h"
#include "adc.h"
#include "delay.h"
#include "gui.h"
#include "gpio.h"
#include <math.h>
#include <stdio.h>
/*************	���س�������	**************/

	
/*************	���ر�������	**************/


/*************	���غ�������	**************/


/*************  �ⲿ�����ͱ������� *****************/

static void	GPIO_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ����
	GPIO_InitStructure.Pin  = GPIO_Pin_0|GPIO_Pin_1 ;		//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_PullUp;		//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	//��ʼ�� LED

	GPIO_InitStructure.Pin  = GPIO_Pin_2 ;		//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_PullUp;		//ָ��IO������������ʽ,GPIO_PullUp,,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	//��ʼ�� LED

	GPIO_InitStructure.Pin  = GPIO_Pin_5 ;		//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	//��ʼ�� P1^4
	
}



/*------------------------------------
-	function:��ʼ��LED��ADC0��LCD
-			 ��ȡ��ѹ��׼ֵ	 
--------------------------------------*/
void K_thermocouple_Init(void)
{
	GPIO_Init();
	Lcd_Init();

	GUI_Clear(White);

	GUI_sprintf_hzstr16x(0,10, "k���ȵ�ż����", Black, White);
}




void K_thermocouple_Demo(void)
{
	float t = 0;
	char buf[12];
	while(1)
	{
		delay_ms(300);
		P35 = !P35;	

		t = K_threomcouple_ReadTemp();
		sprintf(buf, "�¶�ֵ: %.3f", t/10);
		GUI_sprintf_hzstr16x(0,30,buf,Red,White);		
	}

}



