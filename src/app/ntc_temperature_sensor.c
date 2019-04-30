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


/*************	本地常量声明	**************/
static const float Ka  = 273.15;	//开尔文
static  float t_upper= 30;		//上限温度
	
/*************	本地变量声明	**************/
static u16 Nref = 0;	//基准电压对应AD值
static char buf[12] = {0};
int	vcc = 4500 ;			//上拉电压
PID_LocTypeDef pid = {0};
int pwm_duty = 100;
int pwm_high = 20;
int i = 0;
int pwm = 0;
/*************	本地函数声明	**************/

/*************  外部函数和变量声明 *****************/


static void	ADC0_Init(void)
{
	ADC_InitTypeDef		ADC_InitStructure;				//结构定义
	ADC_InitStructure.ADC_Px        = ADC_P10;	//设置要做ADC的IO,	ADC_P10 ~ ADC_P17(或操作),ADC_P1_All
	ADC_InitStructure.ADC_Speed     = ADC_360T;			//ADC速度			ADC_90T,ADC_180T,ADC_360T,ADC_540T
	ADC_InitStructure.ADC_Power     = ENABLE;			//ADC功率允许/关闭	ENABLE,DISABLE
	ADC_InitStructure.ADC_AdjResult = ADC_RES_H8L2;		//ADC结果调整,	ADC_RES_H2L8,ADC_RES_H8L2
	ADC_InitStructure.ADC_Polity    = PolityLow;		//优先级设置	PolityHigh,PolityLow
	ADC_InitStructure.ADC_Interrupt = DISABLE;			//中断允许		ENABLE,DISABLE
	ADC_Inilize(&ADC_InitStructure);					//初始化
	ADC_PowerControl(ENABLE);							//单独的ADC电源操作函数, ENABLE或DISABLE
}

static void	GPIO_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
	GPIO_InitStructure.Pin  = GPIO_Pin_5 ;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	//初始化 LED

	GPIO_InitStructure.Pin  = GPIO_Pin_4 ;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	//初始化 P1^4
	
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


/*------------------------------------
-	function:初始化LED、ADC0、LCD
-			 获取电压基准值	 
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

	GUI_sprintf_hzstr16x(10,10, "热敏传感器测试", Black, White);
	GUI_sprintf_hzstr16x(10,30, "基准电压:", Black, White );
	GUI_sprintf_hzstr16x(10,70, "AD电压:", Black, White );
	GUI_sprintf_hzstr16x(10,90, "NTC阻值:  ", Black, White );
	GUI_sprintf_hzstr16x(10,110, "温度值:  ", Black, White );

	//读内部基准电压, P1ASF=0, 读0通道,实际读9通道
	//Vref = Nref * Vcc / 1024   Vref = 1250mv
	P1ASF = 0;
	Nref = Get_ADC10bitResult(0);
	P1ASF = ADC_P10;
	sprintf(buf, "%dmv AD值%d", 1250, Nref);		
	GUI_sprintf_hzstr16x(90,30,buf,Red,White);
	
	//通过基准电压计算出输入电压
	//vcc = (u32)1250 * 1024 / Nref ;
	sprintf(buf, "vcc： %d", vcc);	 	
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
		//读ADC0电压
		//Ux = Vref * Nx / Nref. Vref = 1250mV
		tptr_value =  Get_ADC10bitResult(0);
		tptr_value = (u32)1250 * (u32)tptr_value / Nref ;
		sprintf(buf, "%.3f %c",(float)tptr_value/1000, 'V' );
		GUI_sprintf_hzstr16x(90,70,buf,Red,White);

		//计算出NTC阻值
		RT1 = (float)tptr_value / ((float)(vcc - tptr_value)/100000);
		sprintf(buf, "%.3fR", RT1);		
		GUI_sprintf_hzstr16x(90,90,buf,Red,White);
		
		//根据RT1计算对应温度T1
		T1=((float)1.0/(log((float)RT1/100000)/3950 + (float)1.0/298.15) - Ka + 0.5);

		sprintf(buf, "%.3f %s",T1, "℃");
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

/*------------------------------------
-	function:	定时器0中断服务函数
-------------------------------------*/
void Ntc_time0_isr(void)
{
	//led灯
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

	//串口
	set_temp();
	
}
