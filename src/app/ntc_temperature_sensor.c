#include "ntc_temperature_sensor.h"
#include "tft_drv.h"
#include "adc.h"
#include "delay.h"
#include "gui.h"
#include "gpio.h"
#include <math.h>
#include <stdio.h>


/*************	本地常量声明	**************/
static const float	T2  = 273.15 + 25.0;
static const u16	RT	= 10000;	//NTC电阻25℃时阻值
static const u16	Bx  = 3950;		//B值
static const float Ka  = 273.15;	//开尔文
static const float Rup = 20000;		//上拉电阻
static const float t_upper= 40;		//上限温度
	
/*************	本地变量声明	**************/
static u16 Nref = 0;	//基准电压对应AD值
static char buf[12] = {0};
static  int	vcc = 5000 ;			//上拉电压

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


/*------------------------------------
-	function:初始化LED、ADC0、LCD
-			 获取电压基准值	 
--------------------------------------*/
void Ntc_Tprt_Sensor_Init(void)
{
	GPIO_Init();
	ADC0_Init();
	Lcd_Init();
		

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
//	vcc = (u32)1250 * (u32)vcc / Nref ;
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
		RT1 = (float)tptr_value / ((float)(vcc - tptr_value)/Rup);
		sprintf(buf, "%.3fR", RT1);		
		GUI_sprintf_hzstr16x(90,90,buf,Red,White);
		
		//根据RT1计算对应温度T1
		T1=((float)1.0/(log(RT1/10000)/Bx + (float)1.0/T2) - Ka + 0.5);

		sprintf(buf, "%.3f %s %d",T1, "℃" ,RT);
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

