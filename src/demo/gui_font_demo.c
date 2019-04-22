/* http://zq281598585.taobao.com/  
�������	  ELH    enlighten  sunny

1.TFT����IC:	ILI9341
2.������   :	RS-P5^5;    WR-P4^2;   RD-P4^4;   CS-P5^4;   REST-Ӳ������
3.������   :  P0�ڵͰ�λ���ݶ˿�  
						  P2�ڸ߰�λ���ݶ˿ڣ��������ݿڶ���ѡ�� ������������

4.TFT�ַ���ʾ
	�ַ�Ϊ8x16�������ַ���zifu8x16.h ��GUI�ļ����� 
	�ַ�����ASCII����ʽ����� ǰ32��û����� �����ڵ��õ�ʱ��Ҫ��ȥ32
	Ҳ������ASCII��32��ʼ��һ���ǿո� ���в������뿴		
         				
*/

#include  "config.h"		
#include  "delay.h"				  	//��ʱ����
#include  "tft_drv.h"			      	//TFT IC�ײ�����
#include  "gui.h"
#include  "gui_font_demo.h"


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


void GUI_Font_Demo(void)
{  
  IO_init();				   //��� IAP15W4K61S4  IO�ڳ�ʼ��
  Lcd_Init();                //tft��ʼ��
	

  GUI_Clear(White);			 //��ɫ����   
	LCD_scan(6);
  GUI_sprintf_ZMHZs(10,40,"ELHҶľϦ��",Blue,White);
  GUI_sprintf_hzstr16x(10,80,"STC15W4K61S4",White,Red);
	GUI_sprintf_hzstr16x(10,100,"��ϼ�������ɣ�",Red3,White);
  GUI_sprintf_hzstr16x(10,120,"��ˮ������һɫ��",Red3,White);
  while(1)
	{
		    

		P35 = !P35;
		delay_ms(500);
	}		
}




