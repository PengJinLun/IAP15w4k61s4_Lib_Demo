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

/*************	����˵��	**************

��������ʾ��·ADC��ѯ������ͨ��ģ�⴮�ڷ��͸���λ����������9600,8,n,1��
�û������޸�Ϊ1~8·��ADCת����

******************************************/

/*************	���س�������	**************/


/*************	���ر�������	**************/


/*************	���غ�������	**************/



/*************  �ⲿ�����ͱ������� *****************/


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
	ADC_InitTypeDef		ADC_InitStructure;				//�ṹ����
	ADC_InitStructure.ADC_Px        = ADC_P10 | ADC_P11 | ADC_P12;	//����Ҫ��ADC��IO,	ADC_P10 ~ ADC_P17(�����),ADC_P1_All
	ADC_InitStructure.ADC_Speed     = ADC_360T;			//ADC�ٶ�			ADC_90T,ADC_180T,ADC_360T,ADC_540T
	ADC_InitStructure.ADC_Power     = ENABLE;			//ADC��������/�ر�	ENABLE,DISABLE
	ADC_InitStructure.ADC_AdjResult = ADC_RES_H8L2;		//ADC�������,	ADC_RES_H2L8,ADC_RES_H8L2
	ADC_InitStructure.ADC_Polity    = PolityLow;		//���ȼ�����	PolityHigh,PolityLow
	ADC_InitStructure.ADC_Interrupt = DISABLE;			//�ж�����		ENABLE,DISABLE
	ADC_Inilize(&ADC_InitStructure);					//��ʼ��
	ADC_PowerControl(ENABLE);							//������ADC��Դ��������, ENABLE��DISABLE
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


		//	Get_ADC10bitResult(i);		//����0~7,��ѯ��ʽ��һ��ADC, ����һ��
			j = Get_ADC10bitResult(i);	//����0~7,��ѯ��ʽ��һ��ADC, ����ֵ���ǽ��, == 1024 Ϊ����
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
	
	ADC_InitTypeDef		ADC_InitStructure;				//�ṹ����
	ADC_InitStructure.ADC_Px        = ADC_P10 ;	//����Ҫ��ADC��IO,	ADC_P10 ~ ADC_P17(�����),ADC_P1_All
	ADC_InitStructure.ADC_Speed     = ADC_360T;			//ADC�ٶ�			ADC_90T,ADC_180T,ADC_360T,ADC_540T
	ADC_InitStructure.ADC_Power     = ENABLE;			//ADC��������/�ر�	ENABLE,DISABLE
	ADC_InitStructure.ADC_AdjResult = ADC_RES_H8L2;		//ADC�������,	ADC_RES_H2L8,ADC_RES_H8L2
	ADC_InitStructure.ADC_Polity    = PolityLow;		//���ȼ�����	PolityHigh,PolityLow
	ADC_InitStructure.ADC_Interrupt = DISABLE;			//�ж�����		ENABLE,DISABLE
	ADC_Inilize(&ADC_InitStructure);					//��ʼ��
	ADC_PowerControl(ENABLE);	


	IO_init();
	Lcd_Init();	
	GUI_Clear(White);			 //��ɫ���� 


	
	
	GUI_sprintf_hzstr16x(0,0,"P1.0 AD test",Blue,White);		  
	GUI_sprintf_hzstr16x(20,56,"��ǰ��ѹֵ",Black,White);
	GUI_sprintf_hzstr16x(101,56,": .  V",Black,White);
											  
	while(1)
	{
	  delay_ms(500);
	  ad=Get_ADC10bitResult(0);			 //��ȡ��ѹ����ֵ  P1.0�ڲɼ�AD��ѹֵ	
			
	  ad=(float)ad*5*100/1024;       //5/1024 5v��ѹ��1024�� ֵΪÿһ�ݵ�ֵ adΪ�ɼ��ķ���  �������Ϊ�ɼ��ĵ�ѹֵ *100С�������λ 
	                                 //10λAD�ɼ� ��2��10�η� ��ֵΪ1024 ������1024��ʾ5���ĵ�ѹ 
						             //��ô�òɼ���������ֵ ����1024 �ڳ���5 �õ���ֵ���ǲɼ��ĵ�ѹ��ֵ
	
	  GUI_sprintf_char(109,56,(ad/100%10)+'0',Red,White);	            //��ʾ��ѹֵ     
	  GUI_sprintf_char(125,56,(ad/10%10)+'0',Red,White);
	  GUI_sprintf_char(133,56,(ad%10)+'0',Red,White); 
	}
 
 }

