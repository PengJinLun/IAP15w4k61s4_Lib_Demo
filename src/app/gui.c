#include "config.h"
#include  "gui.h"
#include  "tft_drv.h"
#include  "delay.h" 			
#include  "zifu8x16.h"
#include  "hz16x16.h"
#include  "zm16x32.h"
#include  "hz32x32.h"

//����
//color�Ǳ�����ɫ��
//˵����ʹ�ñ�����ɫ���TFTģ����Ļ��ȫ����ʾ���ݡ�
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



//����
//(x��y)�ǵ������
//color �ǵ����ɫ��
//˵������ָ������ɫ��ָ��������λ���ϻ���һ���㡣
void GUI_Point(u8 x, u16 y, u16 color)
{  
	Address_set(x,y,x,y);
	Lcd_Write_Data(color);
}

 



//��8��(Bresenham�㷨)		  
//(rx,ry,a,b):����
//color:��ɫ
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

//��ָ��λ�û�һ��ָ����С��Բ
//(rx,ry):Բ��
//r    :�뾶
//color:��ɫ
//mode :0,�����;1,���
void GUI_arc(u16 rx,u16 ry,u16 r,u16 color,u8 mode)
{
	int a,b,c;
	int di;	  
	a=0;b=r;	  
	di=3-(r<<1);	//�ж��¸���λ�õı�־
	while(a<=b)
	{
		if(mode)
		for(c=a;c<=b;c++)
		gui_circle8(rx,ry,a,c,color);//��ʵ��Բ
 		else gui_circle8(rx,ry,a,b,color);			//������Բ
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 							  
	}
}

//��ʵ�ľ���
//(sx,sy)���ϽǶ�������, 
//(ex,ey)���½Ƕ�������, color��ɫ
//����: ��
//˵������ָ��λ���ϻ���ʵ�ľ��Ρ�
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


////����ɫ�ݱ����  �����Ǻ��� ������
////(sx,sy)���ϽǶ�������, 
////(ex,ey)���½Ƕ�������, color��ɫ
//// mode ģʽ  mode=1  ������ɫ�仯 mode=2 ������ɫ�仯
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



//������
//x0,y0:���ε����Ͻ�����
//width,height:���εĳߴ�
//color:��ɫ
void GUI_fill_box(u16 x0,u16 y0,u16 width,u16 height,u16 color)
{	  							   
	if(width==0||height==0)return;//�Ƿ�.	 
	GUI_box(x0,y0,x0+width-1,y0+height-1,color);	   	   
}


//��Բ�Ǿ���/���Բ�Ǿ���
//x,y,width,height:Բ�Ǿ��ε�λ�úͳߴ�
//r:Բ�ǵİ뾶.
//upcolor:�ϰ벿����ɫ
//downcolor:�°벿����ɫ
void GUI_arcrectangle(u16 x,u16 y,u16 width,u16 height,u8 r,u16 upcolor,u16 downcolor)
{
	u16 btnxh=0;
	if(height%2)btnxh=height+1;//��ż������
	else btnxh=height;

	GUI_fill_box(x+r,y,width-2*r,btnxh/2,upcolor);			//����
	//delay1ms(1000);
	GUI_fill_box(x+r,y+btnxh/2,width-2*r,btnxh/2,downcolor);	//����
	//delay1ms(1000);
	GUI_fill_box(x,y+r,r,btnxh/2-r,upcolor);					//����
	//delay1ms(1000);
	GUI_fill_box(x,y+btnxh/2,r,btnxh/2-r,downcolor);			//����
	//delay1ms(1000);
	GUI_fill_box(x+width-r,y+r,r,btnxh/2-r,upcolor);			//����
	//delay1ms(1000);
	GUI_fill_box(x+width-r,y+btnxh/2,r,btnxh/2-r,downcolor);	//����
	//delay1ms(1000);

	GUI_arc(x+r,y+r,r,upcolor,1);//����	
	//delay1ms(1000);
	GUI_arc(x+r,y+btnxh-r-1,r,downcolor,1);//����	
	//delay1ms(1000);
	GUI_arc(x+width-r-1,y+r,r,upcolor,1);	 //����
	//delay1ms(1000);
	GUI_arc(x+width-r-1,y+btnxh-r-1,r,downcolor,1);//����
	//delay1ms(1000);
}




//��ʾӢ�Ļ������ַ�
//x  y  Ҫ��ʾ�ַ��ĳ�ʼ����
//value ��ʾ�ַ�����
//dcolor ��ʾ���ַ���ɫ  bgcolor��ʾ�ַ��ı�����ɫ
void GUI_sprintf_char(u8 x,u16 y,u8 value,u16 dcolor,u16 bgcolor)	
{  
	u8 i,j;
	u8 *temp=zifu816;    //temp��*temp�ĵ�ַ  ����temp����zifu���׵�ַ 
 	                    
	temp+=(value-32)*16;   //ȷ��Ҫ��ʾ��ֵ
				           //��ascii��  ǰ32��asciiû�д���zifu���� ����Ҫ��32
	                       //ÿ���ַ���16���ֽ���ʾ �����ڳ���16  ���Ƕ�Ӧ����ʾλ���׵�ַ

										 
	 Address_set(x,y,x+7,y+15);    //��������		    
		for(j=0;j<16;j++)
		{
			for(i=0;i<8;i++)		    //�Ⱥ�ɨ
			{ 		     
			 	if((*temp&(1<<(7-i)))!=0)		   //��1 ���� Ȼ���Ӧλ�������� 				
				  Lcd_Write_Data(dcolor);		   //��ʾ�ַ���ɫ
				 
				else				
				  Lcd_Write_Data(bgcolor);		   //��ʾ������ɫ		
			}
			temp++;								   //��һ�ֽ�
		 }


}




//��������ʾ
//˵������������ȡģ���������õ� ָ���ĺ��� ���ִ�С��16x16	 ��32���ֽڴ洢һ������
// 		�ⲿ�ֺ����Ǵ��ڳ���ռ� ���Ժ��ֵĶ���ֱ��Ӱ�����ռ��ʣ����
//      ��Ҫ�����ھ���ʾ����ָ���ĺ�����˵�Ͳ��õ����ֿ��� 
//x  y  Ҫ��ʾ���ֵ�����
//c[2]  ���ֵ����� һ�����������ֽڱ�ʾ
//dcolor ���ֵ���ɫ   bgcolor ������ɫ
void GUI_sprintf_hz1616(u8 x, u16  y, u8 c[2], u16 dcolor,u16 bgcolor)
{
	u8 i,j,k,m;									//������ʱ����
												
	Address_set(x,y  ,x+16-1, y+16-1);      //ѡ������λ��

	for (k=0;k<64;k++) 		  //64��ʾ�Խ����ֿ��еĸ�����ѭ����ѯ����	���������ȷ�� ȡ���ڴ���ĺ��ָ���	  
	{                        
	  if ((codeHZ_16[k].Index[0]==c[0])&&(codeHZ_16[k].Index[1]==c[1]))	    //Ѱ�Ҷ�Ӧ����	 һ��������Ҫ�����ֽ�
	  { 
    	for(i=0;i<32;i++) {								    //32���ֽ� ÿ���ֽڶ�Ҫһ����һ���㴦�� �����Ǵ�����32X8��
		   m=codeHZ_16[k].Msk[i];							//��ȡ��Ӧ�ֽ�����
		  for(j=0;j<8;j++) 									//��ʾһ���ֽ�  һ���ֽ�8λ Ҳ����8����
		   {
			 if((m&0x80)==0x80) 							//�ж��Ƿ���Ҫд��� 	 ����� ��������ɫ
			   Lcd_Write_Data(dcolor);				
			 else 											//������� Ϊ����ɫ  ����ɫ
			   Lcd_Write_Data(bgcolor);
				
			 m<<=1;										    //����һλ  �ж���һ��
		   } 
		  }
		}  
	  }	
	}




//��ʾ�ַ��� 
//x1 y1 ��ʾ�ĳ�ʼλ��
//*str Ҫ��ʾ������
//dcolor ��ʾ�ַ�����ɫ
//bgcolor ��ʾ�ַ��ı�����ɫ  
void GUI_sprintf_hzstr16x(u8 x1,u16 y1,u8 *str,u16 dcolor,u16 bgcolor)	  
{  
	 u8 l=0;
	while(*str)
	{	
	  if(*str<0x80)		  //С��128   ascii ����������
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





//��ʾ32x32��������
//(x��y) ����ʾ���ݵ����Ͻ����꣬c[2]��ʾ���ֵ������ֽ�
//dcolor ���ֵ���ɫ   bgcolor ������ɫ
//˵������ָ��λ������ʾһ��32x32�ĺ���
void GUI_sprintf_hz3232(u8 x, u16  y, u8 c[2], u16 dcolor,u16 bgcolor)
{
	u8 i,j,k;

	Address_set(x,y,x+32-1, y+32-1);     //������ʾ��Χ

	for (k=0;k<40;k++) 	  //�Խ����ֿ⣨k���ֵ�ͺ��ֿ��к��ָ����йأ���ѭ����ѯ����
	{ 
	  if ((codeHZ_32[k].Index[0]==c[0])&&(codeHZ_32[k].Index[1]==c[1]))		//Ѱ��Ҫ��ʾ�ĺ���
	  { 
    	for(i=0;i<128;i++)													//1������Ҫ128���ֽڵ�����ʾ
		{
		  u8 m=codeHZ_32[k].Msk[i];											//һ���ֽ�һ���ֽڵ���ʾ
		  for(j=0;j<8;j++) 
		  {
			if((m&0x80)==0x80) 
			  Lcd_Write_Data(dcolor);	//��ʾ������ɫ
			else 
			  Lcd_Write_Data(bgcolor);	//��ʾ������ɫ
			m<<=1;
		  }    
		}
	  }  
	}	
}



//��ʾ16x32��ĸ
//x��y����ʾ���ݵ����Ͻ����꣬
//c ��ĸֵ
//dcolor ���ֵ���ɫ   bgcolor ������ɫ
//˵������ָ��λ������ʾӢ����ĸ
void GUI_sprintf_ZM3216(u8 x, u16  y, u8 c, u16 dcolor,u16 bgcolor)
{
	u8 i,j,k;

	Address_set(x,y,x+16-1, y+32-1);     //������ʾ��Χ

	for (k=0;k<19;k++) 		  //�Խ�16x32��ĸ�⣨k���ֵ����ĸ���к��ָ����йأ���ѭ����ѯ����
	{ 
	  if (codeZM_32[k].Index[0]==c)			       //Ѱ��Ҫ��ʾ����ĸ
	  { 
    	for(i=0;i<64;i++) 						   //1����ĸҪ64���ֽڵ�����ʾ
		{
		  u8 m=codeZM_32[k].Msk[i];				   //һ���ֽ�һ���ֽڵ���ʾ
		  for(j=0;j<8;j++) 
		  {
			if((m&0x80)==0x80) 			
			  Lcd_Write_Data(dcolor);	   //��ʾ��ĸ��ɫ			
			else 
			  Lcd_Write_Data(bgcolor);  //��ʾ������ɫ
			m<<=1;
		  }    
		}
	  }  
	}	
}





//��ʾ32x32���� ��32x16 ��ĸ��
//x1��y1����ʾ���ݵ����Ͻ����꣬
//dcolor ���ֵ���ɫ   bgcolor ������ɫ
//*str Ҫ��ʾ������
void GUI_sprintf_ZMHZs(u8 x1,u16 y1,u8 *str,u16 dcolor,u16 bgcolor)	 
{  
   u8 l=0;	  
   while(*str)
   {
	 if(*str<0x80)		  //ascii������	        
	 {
	   GUI_sprintf_ZM3216(x1+l*8,y1,*str,dcolor, bgcolor);
	   str+=1;l+=2;
	 }
	 else				  //��ʾ����
	 {
	   GUI_sprintf_hz3232(x1+l*8,y1,(u8*)str,dcolor,bgcolor);
	   l+=4;
	   str+=2;
	 }
   }
}


