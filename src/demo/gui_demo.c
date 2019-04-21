#include "gui_demo.h"
#include "gui.h"
#include "tft_drv.h"
#include "delay.h"


//��� IAP15W4K61S4 ϵ�� IO�ڳ�ʼ��
//io�ڳ�ʼ�� P0 P1 P2 P3 P4 Ϊ׼˫��IO��   
//ע��: STC15W4K32S4ϵ�е�оƬ,�ϵ��������PWM��ص�IO�ھ�Ϊ
//      ����̬,�轫��Щ������Ϊ׼˫��ڻ�ǿ����ģʽ��������ʹ��
//���IO: P0.6/P0.7/P1.6/P1.7/P2.1/P2.2
//        P2.3/P2.7/P3.7/P4.2/P4.4/P4.5
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

void GUI_Demo(void)
{
  IO_init();				   //��� IAP15W4K61S4  IO�ڳ�ʼ��
  Lcd_Init();                //tft��ʼ��
  GUI_Clear(White);			 //��ɫ����   

  GUI_Point(10,10,Red);		 //20,20λ�û�һ����ɫ��

  GUI_arc(30,30,10,Blue,0);	 //30��30λ�û�һ���뾶Ϊ10�Ŀ���Բ

  GUI_arc(50,30,10,Blue,1);	 //50��30λ�û�һ���뾶Ϊ10��ʵ��Բ

  GUI_box(20,70,40,90,Green);//������

  GUI_arcrectangle(64,160,111,68,6,Purple,Red2); //��Բ����


  while(1)
	{
		P35 = !P35;
		delay_ms(500);
	}  	

}

