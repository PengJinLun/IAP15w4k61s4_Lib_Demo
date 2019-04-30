#include "ntc_temperature_sensor.h"
#include "tft_drv.h"
#include "adc.h"
#include "delay.h"
#include "gui.h"
#include "gpio.h"
#include "timer.h"
#include "USART.h"
#include "pid.h"
#include <math.h>
#include <stdio.h>


/*************	���س�������	**************/
static const float Ka  = 273.15;	//������
static  float t_upper= 30;		//�����¶�
	
/*************	���ر�������	**************/
static u16 Nref = 0;	//��׼��ѹ��ӦADֵ
static char buf[12] = {0};
int	vcc = 4500 ;			//������ѹ
PID_LocTypeDef pid = {0};
int pwm_duty = 100;
int pwm_high = 20;
int i = 0;
int pwm = 0;
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

static void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//�ṹ����
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//ģʽ,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//ʹ�ò�����,   BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//������, һ�� 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//�����ʼӱ�, ENABLE��DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//�ж�����,   ENABLE��DISABLE
	COMx_InitStructure.UART_Polity    = PolityLow;			//�ж����ȼ�, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART1_SW_P30_P31;	//�л��˿�,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(����ʹ���ڲ�ʱ��)
	COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;		//�ڲ���·RXD��TXD, ���м�, ENABLE,DISABLE
	USART_Configuration(USART1, &COMx_InitStructure);		//��ʼ������1 USART1,USART2

	PrintString1("STC15F2K60S2 UART1 Test Prgramme!\r\n");	//SUART1����һ���ַ���
}

/*--------------------------------
-	function:	��ʱ��0,1ms
---------------------------------*/
static void	Timer_config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;					//�ṹ����
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityLow;			//ָ���ж����ȼ�, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//�ж��Ƿ�����,   ENABLE��DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;			//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 1000UL);		//��ֵ,
	TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);				//��ʼ��Timer0	  Timer0,Timer1,Timer2
					
}


/*------------------------------------
-	function:��ʼ��LED��ADC0��LCD
-			 ��ȡ��ѹ��׼ֵ	 
--------------------------------------*/
void Ntc_Tprt_Sensor_Init(void)
{
	GPIO_Init();
	UART_config();
	ADC0_Init();
	Lcd_Init();
	Timer_config();

	pid.Kp = 5;
	pid.Ki = 3;
	pid.Kd = 3;

	EA = 1;	



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
	//vcc = (u32)1250 * 1024 / Nref ;
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
		RT1 = (float)tptr_value / ((float)(vcc - tptr_value)/100000);
		sprintf(buf, "%.3fR", RT1);		
		GUI_sprintf_hzstr16x(90,90,buf,Red,White);
		
		//����RT1�����Ӧ�¶�T1
		T1=((float)1.0/(log((float)RT1/100000)/3950 + (float)1.0/298.15) - Ka + 0.5);

		sprintf(buf, "%.3f %s",T1, "��");
		GUI_sprintf_hzstr16x(90,110,buf,Red,White); 		

	}
	return T1;
}

void Ntc_Tprt_Sensor_Demo(void)
{
	float t = 0;
//	float u; 

	while(1)
	{
		//delay_ms(100);
		//P35 = !P35;	
		t = Ntc_Tprt_Sensor_GetTptr();
		//u = PID_Loc(t_upper, t, &pid);
		//sprintf(buf, "%.3f ",u);
	//	GUI_sprintf_hzstr16x(90,130,buf,Red,White);

//		if(u > 100)
//		{
//			pwm_high = 100;
//			pid.LocSum = 100;
//		}
//		else if(u < 0){
//			pwm_high = 0;
//			pid.LocSum = 0;
//		}
//		else
//		{
//			pwm_high = u;
//		}

		if(t > t_upper)
			P14 = 0;
		else
			P14 = 1;		
	}

}


static void set_temp(void)
{
	
	if(COM1.RX_TimeOut > 0)		//��ʱ����
	{
		if(--COM1.RX_TimeOut == 0)
		{
			if(RX1_Buffer[1] == ' ')
			{
				if(RX1_Buffer[0] == 'S')
				{
					t_upper = (RX1_Buffer[2]-'0')*10 + (RX1_Buffer[3]-'0');
					PrintString1("�����¶���Ϊ:");
					TX1_write2buff(RX1_Buffer[2]);
					TX1_write2buff(RX1_Buffer[3]);
					PrintString1("\r\n");
				}else{
					PrintString1("�������\r\n");
				}
	
			}
			else{
				PrintString1("�������\r\n");
			}

			COM1.RX_Cnt = 0;
		}
	}	
}

/*------------------------------------
-	function:	��ʱ��0�жϷ�����
-------------------------------------*/
void Ntc_time0_isr(void)
{
	//led��
	i++;
	if(i == 500){
		i = 0;
		P35 = !P35;
	}
//	pwm++;
//	if(pwm <= pwm_duty){
//		if(pwm <= pwm_high)
//			P14 = 1;	
//		else
//			P14 = 0;
//	}
//	else
//	{
//		pwm = 0;
//	}

	//����
	set_temp();
	
}
