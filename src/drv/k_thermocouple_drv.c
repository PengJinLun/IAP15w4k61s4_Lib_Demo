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
	MAX6675_CS=1; //�ر�MAX6675
	MAX6675_CS=0;//�õͣ�ʹ��MAX6675
	
	MAX6675_SCK=1;
	Value=0;

	for(count=16;count>0;count--) //��ȡ16λMSB
	{
		Value=Value<<1;     //����
		MAX6675_SCK=1;
		if(MAX6675_SO) //ȡ��ǰֵ
			Value|=0x0001;
		MAX6675_SCK=0;  //sck�õ�
		   
	}	   

	MAX6675_CS=1;  //�ر�MAX6675
	EA = 0;
	return Value;
}

float K_threomcouple_ReadTemp(void)
{
	unsigned int tmp;
	float TempValue;
	char i = 0;

	tmp=ReadMAX6675();//��ȡMAX6675 ת������¶���ֵ��
	   
   	tmp=tmp<<1;//ȥ����15λ
	tmp=tmp>>4;//ȥ����0~2λ
	TempValue=(float)tmp*1023.75/4095;//MAX6675����Ϊ0.25�棻

	return 	TempValue;
}


