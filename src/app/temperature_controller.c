#include "temperature_controller.h"
#include "gpio.h"
#include "USART.h"
#include "delay.h"
#include "ds18b20_drv.h"
#include  "tft_drv.h"			      	//TFT IC底层驱动
#include  "gui.h"
#include  "timer.h"
#include <stdio.h>


/*************	本地常量声明	**************/
sbit  H = P1^0;		  //加热片加热管脚  0:加热 1:关闭

/*************	本地变量声明	**************/
static float t_upper = 32;
static char	buf[12] = {0};

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

/*--------------------------------
-	function:	定时器0,1ms
---------------------------------*/
static void	Timer_config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;					//结构定义
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityLow;			//指定中断优先级, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//中断是否允许,   ENABLE或DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;			//指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//是否输出高速脉冲, ENABLE或DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 1000UL);		//初值,
	TIM_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);				//初始化Timer0	  Timer0,Timer1,Timer2
}

static void set_temp(void)
{
	if(COM1.RX_TimeOut > 0)		//超时计数
	{
		if(--COM1.RX_TimeOut == 0)
		{
			if(RX1_Buffer[1] == ' ')
			{
				if(RX1_Buffer[0] == 'S')
				{
					t_upper = (RX1_Buffer[2]-'0')*10 + (RX1_Buffer[3]-'0');
					PrintString1("上限温度设为:");
					TX1_write2buff(RX1_Buffer[2]);
					TX1_write2buff(RX1_Buffer[3]);
					PrintString1("\r\n");
				}else{
					PrintString1("命令错误\r\n");
				}
	
			}
			else{
				PrintString1("命令错误\r\n");
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
  	Lcd_Init();                //tft初始化
	EA = 1;	
	GUI_Clear(White);			//白色清屏 	

	GUI_sprintf_hzstr16x(0,0,"DS18B20 测试",Black,White);
											
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

		//获取温度值，DS18B20读出的值扩大了10倍
		t = Read_Temp();
		sprintf(buf, "温度值: %.3f", t/10);
		GUI_sprintf_hzstr16x(0,30,buf,Red,White);
	
		if(t_upper*10 > t)
			H = 1;
		else
			H = 0;
	}
   	
}

/*------------------------------------
-	function:	定时器0中断服务函数
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
