#include "ntc_temperature_sensor.h"
#include "tft_drv.h"
#include "adc.h"
#include "delay.h"
#include "gui.h"
#include "gpio.h"
#include <math.h>
#include <stdio.h>
#include <string.H>

/*************	���س�������	**************/
const float	T2  = 273.15 + 25.0;
const float	RT2	= 10000;	//NTC����25��ʱ��ֵ
const u16	Bx  = 3950;		//Bֵ
const float Ka  = 273.15;	//������
const float Rup = 20000;	//��������	
/*************	���ر�������	**************/
static u16 Nref = 0;	//��׼��ѹ��ӦADֵ
static char buf[12] = {0};
const  int	vcc = 5000 ;			//������ѹ

/*************	���غ�������	**************/
static void	ADC0_Init(void);
static void	LED_Init(void);
void Ntc_Tprt_Sensor_Init(void);
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

static void	LED_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ����
	GPIO_InitStructure.Pin  = GPIO_Pin_5;		//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	//��ʼ��
	
}



void Ntc_Tprt_Sensor_Init(void)
{
	LED_Init();
	ADC0_Init();
	Lcd_Init();
	
	GUI_Clear(White);

	GUI_sprintf_hzstr16x(10,10, "��������������", Black, White);
	GUI_sprintf_hzstr16x(10,30, "��׼��ѹ:", Black, White );
	GUI_sprintf_hzstr16x(10,50, "AD��ѹ:", Black, White );
	GUI_sprintf_hzstr16x(10,70, "NTC��ֵ:  ", Black, White );
	GUI_sprintf_hzstr16x(10,90, "�¶�ֵ:  ", Black, White );

	//���ڲ���׼��ѹ, P1ASF=0, ��0ͨ��,ʵ�ʶ�9ͨ��
	//Vref = Nref * Vcc / 1024   Vref = 1250mv
	P1ASF = 0;
	Nref = Get_ADC10bitResult(0);
	P1ASF = ADC_P10;
	sprintf(buf, "%dmv ADֵ%d", 1250, Nref);		
	GUI_sprintf_hzstr16x(90,30,buf,Red,White);
	
	//ͨ����׼��ѹ����������ѹ
	//vcc = ((float)1250 / Nref * 1024); 	
}


/*------------------------------------
B=ln(RT1/ RT2)/(1/T1-1/T2)
1/T1 = ln(RT1/ RT2)	/ B + 1/T2
--------------------------------------*/
void  Ntc_Tprt_Sensor_GetTptr(void)
{
	u16 tptr_value = 0;
	float RT1 = 0;
	float T1 = 0;

	while(1){
		delay_ms(500);
		P35 = !P35;
		
		//��ADC0��ѹ
		//Ux = Vref * Nx / Nref. Vref = 1250mV
		tptr_value =  Get_ADC10bitResult(0);
		tptr_value = (u32)1250 * (u32)tptr_value / Nref ;
		sprintf(buf, "%.3f %c",(float)tptr_value/1000, 'V' );
		GUI_sprintf_hzstr16x(90,50,buf,Red,White);

		//�����NTC��ֵ
		RT1 = (float)tptr_value / ((float)(vcc - tptr_value)/Rup);
		sprintf(buf, "%.3fR", RT1);		
		GUI_sprintf_hzstr16x(90,70,buf,Red,White);

		//����RT1�����Ӧ�¶�T1
		T1=((float)1.0/(log(RT1/ RT2)/Bx + (float)1.0/T2) - Ka + 0.5);
		memset(buf,0,12);
		sprintf(buf, "%.3f %s",T1, "��" );
		GUI_sprintf_hzstr16x(90,90,buf,Red,White); 		


	}
}
