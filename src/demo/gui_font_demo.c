/* http://zq281598585.taobao.com/  
启光电子	  ELH    enlighten  sunny

1.TFT驱动IC:	ILI9341
2.控制线   :	RS-P5^5;    WR-P4^2;   RD-P4^4;   CS-P5^4;   REST-硬件配置
3.数据线   :  P0口低八位数据端口  
						  P2口高八位数据端口（两个数据口都以选用 不可做其他）

4.TFT字符显示
	字符为8x16，整个字符以zifu8x16.h 在GUI文件夹内 
	字符是以ASCII码形式存入的 前32个没有添加 所以在调用的时候要减去32
	也就是在ASCII表32开始第一个是空格 如有不明白请看		
         				
*/

#include  "config.h"		
#include  "delay.h"				  	//延时函数
#include  "tft_drv.h"			      	//TFT IC底层驱动
#include  "gui.h"
#include  "gui_font_demo.h"


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


void GUI_Font_Demo(void)
{  
  IO_init();				   //真对 IAP15W4K61S4  IO口初始化
  Lcd_Init();                //tft初始化
	

  GUI_Clear(White);			 //白色清屏   
	LCD_scan(6);
  GUI_sprintf_ZMHZs(10,40,"ELH叶木夕阳",Blue,White);
  GUI_sprintf_hzstr16x(10,80,"STC15W4K61S4",White,Red);
	GUI_sprintf_hzstr16x(10,100,"落霞与孤鹜齐飞，",Red3,White);
  GUI_sprintf_hzstr16x(10,120,"秋水共长天一色。",Red3,White);
  while(1)
	{
		    

		P35 = !P35;
		delay_ms(500);
	}		
}




