#include "ntc_temperature_sensor.h"
#include "tft_drv.h"
#include "adc.h"
#include "delay.h"
#include "gui.h"
#include "gpio.h"
#include <math.h>
#include <stdio.h>


/*************	���س�������	**************/
static const float	T2  = 273.15 + 25.0;
static const u16	RT	= 10000;	//NTC����25��ʱ��ֵ
static const u16	Bx  = 3950;		//Bֵ
static const float Ka  = 273.15;	//������
static const float Rup = 20000;		//��������
static const float t_upper= 40;		//�����¶�
	
/*************	���ر�������	**************/
static u16 Nref = 0;	//��׼��ѹ��ӦADֵ
static char buf[12] = {0};
static  int	vcc = 5000 ;			//������ѹ

/*************	���غ�������	**************/


/*************  �ⲿ�����ͱ������� *****************/


static void	ADC0_Init(void)
{
	ADC_InitTypeDef		ADC_InitStructure;				//�ṹ����
	ADC_InitStructure.ADC_Px        = ADC_P10;	//����Ҫ��ADC��IO,	ADC_P10 ~ ADC_P17(�����),ADC_P1_All
	ADC_InitStructure.ADC_Speed     = ADC_360T;			//ADC�ٶ�			ADC_90T,ADC_180T,ADC_360T,ADC_540T
	ADC_InitStructure.ADC_Power     = ENABLE;			//ADC��������/�ر�	ENABLE,DISABLE
	ADC_InitStructure.ADC_AdjResult = ADC_RES_H8L2;		//ADC�������,	ADC_RES_H2L8,ADC_RES_H8L2
	ADC_InitStructure.ADC_Polity    = PolityLow;		//���ȼ�����	PolityHigh,PolityLow
	ADC_InitStructure.ADC_Interrupt = DISABLE;			//�ж�����		ENABLE,DISABLE
	ADC_Inilize(&ADC_InitStructure);					//��ʼ��
	ADC_PowerControl(ENABLE);							//������ADC��Դ��������, ENABLE��DISABLE
}

static void	GPIO_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ����
	GPIO_InitStructure.Pin  = GPIO_Pin_5 ;		//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	//��ʼ�� LED

	GPIO_InitStructure.Pin  = GPIO_Pin_4 ;		//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	//��ʼ�� P1^4
	
}


/*------------------------------------
-	function:��ʼ��LED��ADC0��LCD
-			 ��ȡ��ѹ��׼ֵ	 
--------------------------------------*/
void Ntc_Tprt_Sensor_Init(void)
{
	GPIO_Init();
	ADC0_Init();
	Lcd_Init();
		

	GUI_Clear(White);

	GUI_sprintf_hzstr16x(10,10, "��������������", Black, White);
	GUI_sprintf_hzstr16x(10,30, "��׼��ѹ:", Black, White );
	GUI_sprintf_hzstr16x(10,70, "AD��ѹ:", Black, White );
	GUI_sprintf_hzstr16x(10,90, "NTC��ֵ:  ", Black, White );
	GUI_sprintf_hzstr16x(10,110, "�¶�ֵ:  ", Black, White );

	//���ڲ���׼��ѹ, P1ASF=0, ��0ͨ��,ʵ�ʶ�9ͨ��
	//Vref = Nref * Vcc / 1024   Vref = 1250mv
	P1ASF = 0;
	Nref = Get_ADC10bitResult(0);
	P1ASF = ADC_P10;
	sprintf(buf, "%dmv ADֵ%d", 1250, Nref);		
	GUI_sprintf_hzstr16x(90,30,buf,Red,White);
	
	//ͨ����׼��ѹ����������ѹ
//	vcc = (u32)1250 * (u32)vcc / Nref ;
	sprintf(buf, "vcc�� %d", vcc);	 	
	GUI_sprintf_hzstr16x(10,50,buf,Red,White);
}


/*------------------------------------
B=ln(RT1/ RT2)/(1/T1-1/T2)
T1 = 1/(ln(RT1/ RT2)/B + 1/T2) - 273.15 + 0.5
--------------------------------------*/
static float  Ntc_Tprt_Sensor_GetTptr(void)
{
	u16 tptr_value = 0;
	float RT1 = 0;
	float T1 = 0;

	//while(1)
	{	
		//��ADC0��ѹ
		//Ux = Vref * Nx / Nref. Vref = 1250mV
		tptr_value =  Get_ADC10bitResult(0);
		tptr_value = (u32)1250 * (u32)tptr_value / Nref ;
		sprintf(buf, "%.3f %c",(float)tptr_value/1000, 'V' );
		GUI_sprintf_hzstr16x(90,70,buf,Red,White);

		//�����NTC��ֵ
		RT1 = (float)tptr_value / ((float)(vcc - tptr_value)/Rup);
		sprintf(buf, "%.3fR", RT1);		
		GUI_sprintf_hzstr16x(90,90,buf,Red,White);
		
		//����RT1�����Ӧ�¶�T1
		T1=((float)1.0/(log(RT1/10000)/Bx + (float)1.0/T2) - Ka + 0.5);

		sprintf(buf, "%.3f %s %d",T1, "��" ,RT);
		GUI_sprintf_hzstr16x(90,110,buf,Red,White); 		

	}
	return T1;
}

void Ntc_Tprt_Sensor_Demo(void)
{
	float t = 0;
	while(1)
	{
		delay_ms(500);
		P35 = !P35;	

		t = Ntc_Tprt_Sensor_GetTptr();
		if(t > t_upper)
			P14 = 0;
		else
			P14 = 1;		
	}

}

