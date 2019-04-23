/*------------------------------------------------------------------*/
/* --- STC MCU International Limited -------------------------------*/
/* --- STC 1T Series MCU RC Demo -----------------------------------*/
/* --- Mobile: (86)13922805190 -------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ---------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ---------------------*/
/* --- Web: www.GXWMCU.com -----------------------------------------*/
/* --- QQ:  800003751 ----------------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/

#include	"config.h"
#include	"adc.h"
#include	"delay.h"
#include	"soft_uart.h"
#include 	"adc_demo.h"
#include 	"gui.h"
#include 	"tft_drv.h"

/*************	功能说明	**************

本程序演示多路ADC查询采样，通过模拟串口发送给上位机，波特率9600,8,n,1。
用户可以修改为1~8路的ADC转换。

******************************************/

/*************	本地常量声明	**************/


/*************	本地变量声明	**************/


/*************	本地函数声明	**************/



/*************  外部函数和变量声明 *****************/


static void IO_init(void)
{
  P0M0 = 0X00;
  P0M1 = 0X00;

  P1M0 = 0X00;
  P1M1 = 0X00;

  P2M0 = 0X00;
  P2M1 = 0X00;

  P3M0 = 0X00;
  P3M1 = 0X00;

  P4M0 = 0X00;
  P4M1 = 0X00;  
}

static void	ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;				//结构定义
	ADC_InitStructure.ADC_Px        = ADC_P10 | ADC_P11 | ADC_P12;	//设置要做ADC的IO,	ADC_P10 ~ ADC_P17(或操作),ADC_P1_All
	ADC_InitStructure.ADC_Speed     = ADC_360T;			//ADC速度			ADC_90T,ADC_180T,ADC_360T,ADC_540T
	ADC_InitStructure.ADC_Power     = ENABLE;			//ADC功率允许/关闭	ENABLE,DISABLE
	ADC_InitStructure.ADC_AdjResult = ADC_RES_H8L2;		//ADC结果调整,	ADC_RES_H2L8,ADC_RES_H8L2
	ADC_InitStructure.ADC_Polity    = PolityLow;		//优先级设置	PolityHigh,PolityLow
	ADC_InitStructure.ADC_Interrupt = DISABLE;			//中断允许		ENABLE,DISABLE
	ADC_Inilize(&ADC_InitStructure);					//初始化
	ADC_PowerControl(ENABLE);							//单独的ADC电源操作函数, ENABLE或DISABLE
}



/**********************************************/
void ADC_Demo(void)
{
	u8	i;
	u16	j;


	ADC_config();

	while (1)
	{

		for(i=0; i<3; i++)
		{
			delay_ms(250);


		//	Get_ADC10bitResult(i);		//参数0~7,查询方式做一次ADC, 丢弃一次
			j = Get_ADC10bitResult(i);	//参数0~7,查询方式做一次ADC, 返回值就是结果, == 1024 为错误
			TxSend('A');
			TxSend('D');
			TxSend(i+'0');
			TxSend('=');
			TxSend(j/1000 + '0');
			TxSend(j%1000/100 + '0');
			TxSend(j%100/10 + '0');
			TxSend(j%10 + '0');
			TxSend(' ');
			TxSend(' ');



		}
		P35 = !P35;
		PrintString("\r\n");
	}
}

void 	ADC0_Demo(void)
{
	u16 ad;
	
	ADC_InitTypeDef		ADC_InitStructure;				//结构定义
	ADC_InitStructure.ADC_Px        = ADC_P10 ;	//设置要做ADC的IO,	ADC_P10 ~ ADC_P17(或操作),ADC_P1_All
	ADC_InitStructure.ADC_Speed     = ADC_360T;			//ADC速度			ADC_90T,ADC_180T,ADC_360T,ADC_540T
	ADC_InitStructure.ADC_Power     = ENABLE;			//ADC功率允许/关闭	ENABLE,DISABLE
	ADC_InitStructure.ADC_AdjResult = ADC_RES_H8L2;		//ADC结果调整,	ADC_RES_H2L8,ADC_RES_H8L2
	ADC_InitStructure.ADC_Polity    = PolityLow;		//优先级设置	PolityHigh,PolityLow
	ADC_InitStructure.ADC_Interrupt = DISABLE;			//中断允许		ENABLE,DISABLE
	ADC_Inilize(&ADC_InitStructure);					//初始化
	ADC_PowerControl(ENABLE);	


	IO_init();
	Lcd_Init();	
	GUI_Clear(White);			 //白色清屏 


	
	
	GUI_sprintf_hzstr16x(0,0,"P1.0 AD test",Blue,White);		  
	GUI_sprintf_hzstr16x(20,56,"当前电压值",Black,White);
	GUI_sprintf_hzstr16x(101,56,": .  V",Black,White);
											  
	while(1)
	{
	  delay_ms(500);
	  ad=Get_ADC10bitResult(0);			 //读取电压数量值  P1.0口采集AD电压值	
			
	  ad=(float)ad*5*100/1024;       //5/1024 5v电压份1024份 值为每一份的值 ad为采集的份数  与其相乘为采集的电压值 *100小数点后两位 
	                                 //10位AD采集 即2的10次方 满值为1024 这里用1024表示5伏的电压 
						             //那么用采集到的数量值 除以1024 在乘以5 得到的值就是采集的电压数值
	
	  GUI_sprintf_char(109,56,(ad/100%10)+'0',Red,White);	            //显示电压值     
	  GUI_sprintf_char(125,56,(ad/10%10)+'0',Red,White);
	  GUI_sprintf_char(133,56,(ad%10)+'0',Red,White); 
	}
 
 }

