#include "ds18b20_drv.h"
#include "USART.h"
#include "gpio.h"
#include "delay.h"


void	DS18b20_LED_GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ����
	GPIO_InitStructure.Pin  = GPIO_Pin_5;		//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	//��ʼ��
}

void	DS18b20_UART_config(void)
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

	PrintString1("UART1 init ok...\r\n");	//SUART1����һ���ַ���
}

void 	DS18b20_Demo(void)
{
	short temperature;
	char buf[8] = {0};
	
	DS18b20_LED_GPIO_config();
	DS18b20_UART_config();
	EA = 1;	
		
	if(ds18b20_init()){
		PrintString1("ds18b20 sensor is not exit...\r\n");
		return;	
	}

	while(1)
	{
		temperature = Read_Temp();
		if(temperature < 0)
			buf[0] = '-';
		else
			buf[0] = ' ';
		buf[1] = temperature/100%10+'0' ;
		buf[2] = temperature/10%10+'0' ;
		buf[3] = '.';
		buf[4] = temperature%10+'0' ;
		buf[5] = '\r';
		buf[6] = '\n';
		PrintString1(buf);
		P35 = !P35;
		delay_ms(1000);
		
	}
}