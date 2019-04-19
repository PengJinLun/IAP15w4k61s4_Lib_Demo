#include "temperature_controller.h"
#include "gpio.h"
#include "USART.h"
#include "delay.h"
#include "ds18b20_drv.h"
//#include "common.h"
//#include  "printf.h"

/*************	本地常量声明	**************/
sbit  H = P1^0;		  //加热片加热管脚  0:加热 1:关闭

/*************	本地变量声明	**************/
static short t_upper = 30;


/*************	本地函数声明	**************/



/*************  外部函数和变量声明 *****************/





static void	GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
	GPIO_InitStructure.Pin  = GPIO_Pin_5;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	//初始化

	GPIO_InitStructure.Pin  = GPIO_Pin_0;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	//初始化
	H = 0;

	GPIO_InitStructure.Pin  = GPIO_Pin_3;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_PullUp;		//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	//初始化
	
}

static void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//结构定义
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//模式,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//使用波特率,   BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//波特率, 一般 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//中断允许,   ENABLE或DISABLE
	COMx_InitStructure.UART_Polity    = PolityLow;			//中断优先级, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART1_SW_P30_P31;	//切换端口,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(必须使用内部时钟)
	COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;		//内部短路RXD与TXD, 做中继, ENABLE,DISABLE
	USART_Configuration(USART1, &COMx_InitStructure);		//初始化串口1 USART1,USART2

	PrintString1("STC15F2K60S2 UART1 Test Prgramme!\r\n");	//SUART1发送一个字符串
}

void 	Tprt_ctlr_Init(void)
{
	short temperature;
	char buf[8] = {0};
	
	GPIO_config();
	UART_config();
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

		if(t_upper*10 > temperature)
			H = 1;
		else
			H = 0;
		
	}
}


