#include "gui_demo.h"
#include "gui.h"
#include "tft_drv.h"
#include "delay.h"


//真对 IAP15W4K61S4 系列 IO口初始化
//io口初始化 P0 P1 P2 P3 P4 为准双向IO口   
//注意: STC15W4K32S4系列的芯片,上电后所有与PWM相关的IO口均为
//      高阻态,需将这些口设置为准双向口或强推挽模式方可正常使用
//相关IO: P0.6/P0.7/P1.6/P1.7/P2.1/P2.2
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
  IO_init();				   //真对 IAP15W4K61S4  IO口初始化
  Lcd_Init();                //tft初始化
  GUI_Clear(White);			 //白色清屏   

  GUI_Point(10,10,Red);		 //20,20位置画一个红色点

  GUI_arc(30,30,10,Blue,0);	 //30，30位置画一个半径为10的空心圆

  GUI_arc(50,30,10,Blue,1);	 //50，30位置画一个半径为10的实心圆

  GUI_box(20,70,40,90,Green);//画巨型

  GUI_arcrectangle(64,160,111,68,6,Purple,Red2); //画圆巨型


  while(1)
	{
		P35 = !P35;
		delay_ms(500);
	}  	

}

