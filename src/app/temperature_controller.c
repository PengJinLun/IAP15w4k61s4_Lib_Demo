#include "temperature_controller.h"
#include "gpio.h"
#include "USART.h"
#include "delay.h"
#include "ds18b20_drv.h"
#include  "tft_drv.h"			      	//TFT IC�ײ�����
#include  "gui.h"
#include  "timer.h"
#include <stdio.h>


/*************	���س�������	**************/
sbit  H = P1^0;		  //����Ƭ���ȹܽ�  0:���� 1:�ر�

/*************	���ر�������	**************/
static float t_upper = 32;
static char	buf[12] = {0};

/*************	���غ�������	**************/



/*************  �ⲿ�����ͱ������� *****************/





static void	GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ����
	GPIO_InitStructure.Pin  = GPIO_Pin_5;		//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	//��ʼ��

	GPIO_InitStructure.Pin  = GPIO_Pin_0;		//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	//��ʼ��
	H = 0;
	
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

void 	Tprt_ctlr_Init(void)
{
	
	GPIO_config();
	UART_config();
	Timer_config();
  	Lcd_Init();                //tft��ʼ��
	EA = 1;	
	GUI_Clear(White);			//��ɫ���� 	

	GUI_sprintf_hzstr16x(0,0,"DS18B20 ����",Black,White);
											
	if(ds18b20_init()){
		PrintString1("ds18b20 sensor is not exit...\r\n");
		GUI_sprintf_hzstr16x(0,30,"ds18b20 sensor is not exit.",Red,White);
		return;	
	}	
}


void   Tprt_ctlr_Demo(void)
{
	float t = 0;

	while(1){ 	

		//��ȡ�¶�ֵ��DS18B20������ֵ������10��
		t = Read_Temp();
		sprintf(buf, "�¶�ֵ: %.3f", t/10);
		GUI_sprintf_hzstr16x(0,30,buf,Red,White);
	
		if(t_upper*10 > t)
			H = 1;
		else
			H = 0;
	}
   	
}

/*------------------------------------
-	function:	��ʱ��0�жϷ�����
-------------------------------------*/
void DS18B20_time0_isr(void)
{
	static int i = 0;
	i++;
	if(i == 500){
		i = 0;
		P35 = !P35;
	}
	set_temp();
	
}
