#include "k_thermocouple.h"
#include "k_thermocouple_drv.h"
#include "tft_drv.h"
#include "adc.h"
#include "delay.h"
#include "gui.h"
#include "gpio.h"
#include <math.h>
#include <stdio.h>
/*************	本地常量声明	**************/

	
/*************	本地变量声明	**************/


/*************	本地函数声明	**************/


/*************  外部函数和变量声明 *****************/

static void	GPIO_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
	GPIO_InitStructure.Pin  = GPIO_Pin_0|GPIO_Pin_1 ;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_PullUp;		//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	//初始化 LED

	GPIO_InitStructure.Pin  = GPIO_Pin_2 ;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_PullUp;		//指定IO的输入或输出方式,GPIO_PullUp,,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	//初始化 LED

	GPIO_InitStructure.Pin  = GPIO_Pin_5 ;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	//初始化 P1^4
	
}



/*------------------------------------
-	function:初始化LED、ADC0、LCD
-			 获取电压基准值	 
--------------------------------------*/
void K_thermocouple_Init(void)
{
	GPIO_Init();
	Lcd_Init();

	GUI_Clear(White);

	GUI_sprintf_hzstr16x(0,10, "k型热电偶测试", Black, White);
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
		sprintf(buf, "温度值: %.3f", t/10);
		GUI_sprintf_hzstr16x(0,30,buf,Red,White);		
	}

}



