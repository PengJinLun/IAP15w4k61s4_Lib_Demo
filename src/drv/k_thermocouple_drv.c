#include "k_thermocouple_drv.h"
#include "config.h"
#include <intrins.h>

sbit MAX6675_SO=P1^2;    
sbit MAX6675_SCK=P1^0;    
sbit MAX6675_CS=P1^1; 


static unsigned int Value;

/*-------------------------------------
-	feacture:	
-------------------------------------*/
unsigned int ReadMAX6675(void)   
{
	unsigned int Value;
	unsigned char count;
	MAX6675_CS=1; //关闭MAX6675
	MAX6675_CS=0;//置低，使能MAX6675
	
	MAX6675_SCK=1;
	Value=0;

	for(count=16;count>0;count--) //获取16位MSB
	{
		Value=Value<<1;     //左移
		MAX6675_SCK=1;
		if(MAX6675_SO) //取当前值
			Value|=0x0001;
		MAX6675_SCK=0;  //sck置低
		   
	}	   

	MAX6675_CS=1;  //关闭MAX6675
	EA = 0;
	return Value;
}

float K_threomcouple_ReadTemp(void)
{
	unsigned int tmp;
	float TempValue;
	char i = 0;

	tmp=ReadMAX6675();//读取MAX6675 转换后的温度数值；
	   
   	tmp=tmp<<1;//去掉第15位
	tmp=tmp>>4;//去掉第0~2位
	TempValue=(float)tmp*1023.75/4095;//MAX6675精度为0.25℃；

	return 	TempValue;
}


