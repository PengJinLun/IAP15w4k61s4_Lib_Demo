#include "config.h"
#include  "gui.h"
#include  "tft_drv.h"
#include  "delay.h" 			
#include  "zifu8x16.h"
#include  "hz16x16.h"
#include  "zm16x32.h"
#include  "hz32x32.h"

//清屏
//color是背景颜色。
//说明：使用背景颜色清除TFT模块屏幕的全部显示内容。
void GUI_Clear(u16 color)
{
	u16 i;
	u8 j;
	Address_set(0,0,239,319);
	for(i=0;i<320;i++)
	{
		for (j=0;j<240;j++)
		{
				 Lcd_Write_Data(color);
		}

	}
}



//画点
//(x，y)是点的坐标
//color 是点的颜色。
//说明：用指定的颜色在指定的坐标位置上画出一个点。
void GUI_Point(u8 x, u16 y, u16 color)
{  
	Address_set(x,y,x,y);
	Lcd_Write_Data(color);
}

 



//画8点(Bresenham算法)		  
//(rx,ry,a,b):参数
//color:颜色
void gui_circle8(u16 rx,u16 ry,int a,int b,u16 color)
{
	GUI_Point(rx+a,ry-b,color);              
	GUI_Point(rx+b,ry-a,color);                       
	GUI_Point(rx+b,ry+a,color);                           
	GUI_Point(rx+a,ry+b,color);             
	GUI_Point(rx-a,ry+b,color);                  
	GUI_Point(rx-b,ry+a,color);               
	GUI_Point(rx-b,ry-a,color);                      
	GUI_Point(rx-a,ry-b,color);                  
}

//在指定位置画一个指定大小的圆
//(rx,ry):圆心
//r    :半径
//color:颜色
//mode :0,不填充;1,填充
void GUI_arc(u16 rx,u16 ry,u16 r,u16 color,u8 mode)
{
	int a,b,c;
	int di;	  
	a=0;b=r;	  
	di=3-(r<<1);	//判断下个点位置的标志
	while(a<=b)
	{
		if(mode)
		for(c=a;c<=b;c++)
		gui_circle8(rx,ry,a,c,color);//画实心圆
 		else gui_circle8(rx,ry,a,b,color);			//画空心圆
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 							  
	}
}

//画实心矩形
//(sx,sy)左上角顶点坐标, 
//(ex,ey)右下角顶点坐标, color颜色
//返回: 无
//说明：在指定位置上画出实心矩形。
void GUI_box(u8 sx,u16 sy,u8 ex,u16 ey,u16 color)
{ 
	u16 temp,temp1,m,n;   
	Address_set(sx,sy,ex,ey); 
	n=ex-sx+1;
	m=ey-sy+1;

	for(temp=0;temp<m;temp++)
	{	 	
		for(temp1=0;temp1<n;temp1++)
	 	{	
			Lcd_Write_Data(color);
		}
	}
}


////画颜色递变巨型  可以是横条 竖条等
////(sx,sy)左上角顶点坐标, 
////(ex,ey)右下角顶点坐标, color颜色
//// mode 模式  mode=1  纵向颜色变化 mode=2 横向颜色变化
//void GUI_boxchange(u8 sx,u16 sy,u8 ex,u16 ey,u16 color,u8 mode)
//{ 
//	u16 temp,temp1,m,n,color2;   
//	Address_set(sx,sy,ex,ey); 
//    n=ex-sx+1;
//	m=ey-sy+1;
//	if(mode==2)color2=color;
//	for(temp=0;temp<m;temp++)
//	{	 	
//		for(temp1=0;temp1<n;temp1++)
//	 	{	
//			Lcd_Write_Data(color);
//			if(mode==2)color+=15;
//		}
//		if(mode==1)color+=15;
//		if(mode==2)color=color2;
//	}
//}



//填充矩形
//x0,y0:矩形的左上角坐标
//width,height:矩形的尺寸
//color:颜色
void GUI_fill_box(u16 x0,u16 y0,u16 width,u16 height,u16 color)
{	  							   
	if(width==0||height==0)return;//非法.	 
	GUI_box(x0,y0,x0+width-1,y0+height-1,color);	   	   
}


//画圆角矩形/填充圆角矩形
//x,y,width,height:圆角矩形的位置和尺寸
//r:圆角的半径.
//upcolor:上半部分颜色
//downcolor:下半部分颜色
void GUI_arcrectangle(u16 x,u16 y,u16 width,u16 height,u8 r,u16 upcolor,u16 downcolor)
{
	u16 btnxh=0;
	if(height%2)btnxh=height+1;//基偶数处理
	else btnxh=height;

	GUI_fill_box(x+r,y,width-2*r,btnxh/2,upcolor);			//中上
	//delay1ms(1000);
	GUI_fill_box(x+r,y+btnxh/2,width-2*r,btnxh/2,downcolor);	//中下
	//delay1ms(1000);
	GUI_fill_box(x,y+r,r,btnxh/2-r,upcolor);					//左上
	//delay1ms(1000);
	GUI_fill_box(x,y+btnxh/2,r,btnxh/2-r,downcolor);			//左下
	//delay1ms(1000);
	GUI_fill_box(x+width-r,y+r,r,btnxh/2-r,upcolor);			//右上
	//delay1ms(1000);
	GUI_fill_box(x+width-r,y+btnxh/2,r,btnxh/2-r,downcolor);	//右下
	//delay1ms(1000);

	GUI_arc(x+r,y+r,r,upcolor,1);//左上	
	//delay1ms(1000);
	GUI_arc(x+r,y+btnxh-r-1,r,downcolor,1);//左下	
	//delay1ms(1000);
	GUI_arc(x+width-r-1,y+r,r,upcolor,1);	 //右上
	//delay1ms(1000);
	GUI_arc(x+width-r-1,y+btnxh-r-1,r,downcolor,1);//右下
	//delay1ms(1000);
}




//显示英文或数字字符
//x  y  要显示字符的初始坐标
//value 显示字符数据
//dcolor 显示的字符颜色  bgcolor显示字符的背景颜色
void GUI_sprintf_char(u8 x,u16 y,u8 value,u16 dcolor,u16 bgcolor)	
{  
	u8 i,j;
	u8 *temp=zifu816;    //temp是*temp的地址  这里temp就是zifu的首地址 
 	                    
	temp+=(value-32)*16;   //确定要显示的值
				           //用ascii表  前32个ascii没有存入zifu库里 所以要减32
	                       //每个字符用16个字节显示 所以在乘以16  就是对应的显示位的首地址

										 
	 Address_set(x,y,x+7,y+15);    //设置区域		    
		for(j=0;j<16;j++)
		{
			for(i=0;i<8;i++)		    //先横扫
			{ 		     
			 	if((*temp&(1<<(7-i)))!=0)		   //将1 左移 然后对应位进行相与 				
				  Lcd_Write_Data(dcolor);		   //显示字符颜色
				 
				else				
				  Lcd_Write_Data(bgcolor);		   //显示背景颜色		
			}
			temp++;								   //下一字节
		 }


}




//程序汉字显示
//说明：汉字是用取模软件制作好的 指定的汉字 汉字大小是16x16	 即32个字节存储一个汉字
// 		这部分汉字是存在程序空间 所以汉字的多少直接影响程序空间的剩余量
//      主要方便于就显示几个指定的汉字来说就不用调用字库了 
//x  y  要显示汉字的坐标
//c[2]  汉字的数据 一个汉字两个字节表示
//dcolor 汉字的颜色   bgcolor 背景颜色
void GUI_sprintf_hz1616(u8 x, u16  y, u8 c[2], u16 dcolor,u16 bgcolor)
{
	u8 i,j,k,m;									//定义临时变量
												
	Address_set(x,y  ,x+16-1, y+16-1);      //选择坐标位置

	for (k=0;k<64;k++) 		  //64表示自建汉字库中的个数，循环查询内码	这个数并不确定 取决于存如的汉字个数	  
	{                        
	  if ((codeHZ_16[k].Index[0]==c[0])&&(codeHZ_16[k].Index[1]==c[1]))	    //寻找对应汉字	 一个汉字需要两个字节
	  { 
    	for(i=0;i<32;i++) {								    //32个字节 每个字节都要一个点一个点处理 所以是处理了32X8次
		   m=codeHZ_16[k].Msk[i];							//读取对应字节数据
		  for(j=0;j<8;j++) 									//显示一个字节  一个字节8位 也就是8个点
		   {
			 if((m&0x80)==0x80) 							//判断是否是要写入点 	 如果是 给字体颜色
			   Lcd_Write_Data(dcolor);				
			 else 											//如果不是 为背景色  给颜色
			   Lcd_Write_Data(bgcolor);
				
			 m<<=1;										    //左移一位  判断下一点
		   } 
		  }
		}  
	  }	
	}




//显示字符串 
//x1 y1 显示的初始位置
//*str 要显示的数据
//dcolor 显示字符的颜色
//bgcolor 显示字符的背景颜色  
void GUI_sprintf_hzstr16x(u8 x1,u16 y1,u8 *str,u16 dcolor,u16 bgcolor)	  
{  
	 u8 l=0;
	while(*str)
	{	
	  if(*str<0x80)		  //小于128   ascii 都在数组内
	  {	  
			GUI_sprintf_char(x1+l*8,y1,*str,dcolor,bgcolor);
			l+=1;
			str++;
		}
	  else
	  {	  
			GUI_sprintf_hz1616(x1+l*8,y1,(u8*)str,dcolor, bgcolor);
			str+=2;l+=2;
	  }
	}	
}





//显示32x32单个汉字
//(x，y) 是显示内容的左上角坐标，c[2]表示汉字的两个字节
//dcolor 汉字的颜色   bgcolor 背景颜色
//说明：在指定位置上显示一个32x32的汉字
void GUI_sprintf_hz3232(u8 x, u16  y, u8 c[2], u16 dcolor,u16 bgcolor)
{
	u8 i,j,k;

	Address_set(x,y,x+32-1, y+32-1);     //设置显示范围

	for (k=0;k<40;k++) 	  //自建汉字库（k最大值和汉字库中汉字个数有关），循环查询内码
	{ 
	  if ((codeHZ_32[k].Index[0]==c[0])&&(codeHZ_32[k].Index[1]==c[1]))		//寻找要显示的汉字
	  { 
    	for(i=0;i<128;i++)													//1个汉字要128个字节点来显示
		{
		  u8 m=codeHZ_32[k].Msk[i];											//一个字节一个字节的显示
		  for(j=0;j<8;j++) 
		  {
			if((m&0x80)==0x80) 
			  Lcd_Write_Data(dcolor);	//显示汉字颜色
			else 
			  Lcd_Write_Data(bgcolor);	//显示背景颜色
			m<<=1;
		  }    
		}
	  }  
	}	
}



//显示16x32字母
//x，y是显示内容的左上角坐标，
//c 字母值
//dcolor 汉字的颜色   bgcolor 背景颜色
//说明：在指定位置上显示英文字母
void GUI_sprintf_ZM3216(u8 x, u16  y, u8 c, u16 dcolor,u16 bgcolor)
{
	u8 i,j,k;

	Address_set(x,y,x+16-1, y+32-1);     //设置显示范围

	for (k=0;k<19;k++) 		  //自建16x32字母库（k最大值和字母库中汉字个数有关），循环查询内码
	{ 
	  if (codeZM_32[k].Index[0]==c)			       //寻找要显示的字母
	  { 
    	for(i=0;i<64;i++) 						   //1个字母要64个字节点来显示
		{
		  u8 m=codeZM_32[k].Msk[i];				   //一个字节一个字节的显示
		  for(j=0;j<8;j++) 
		  {
			if((m&0x80)==0x80) 			
			  Lcd_Write_Data(dcolor);	   //显示字母颜色			
			else 
			  Lcd_Write_Data(bgcolor);  //显示背景颜色
			m<<=1;
		  }    
		}
	  }  
	}	
}





//显示32x32汉字 和32x16 字母串
//x1，y1是显示内容的左上角坐标，
//dcolor 汉字的颜色   bgcolor 背景颜色
//*str 要显示的数据
void GUI_sprintf_ZMHZs(u8 x1,u16 y1,u8 *str,u16 dcolor,u16 bgcolor)	 
{  
   u8 l=0;	  
   while(*str)
   {
	 if(*str<0x80)		  //ascii数组内	        
	 {
	   GUI_sprintf_ZM3216(x1+l*8,y1,*str,dcolor, bgcolor);
	   str+=1;l+=2;
	 }
	 else				  //显示汉字
	 {
	   GUI_sprintf_hz3232(x1+l*8,y1,(u8*)str,dcolor,bgcolor);
	   l+=4;
	   str+=2;
	 }
   }
}



