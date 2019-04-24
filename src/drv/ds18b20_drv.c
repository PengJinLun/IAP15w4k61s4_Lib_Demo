#include "ds18b20_drv.h"
#include "delay.h"


sbit DQ=P1^3;       //ds18b20 �˿�

/*----------------------------------
- function:DS18B20��λ����	
----------------------------------*/
void ow_reset(void)
{

    DQ=1;                //�Ӹ�������
    DQ=0;
    delay_us(500);    //>480 us	  �͵�ƽ��λ�ź�
    DQ=1;
    delay_us(20);      //>15us ��������	 15-60us�ߵ�ƽ��  ��60-240us ��Ӧ���ź�
}



/*----------------------------------
-	function:	�ȴ�DS18B20�Ļ�Ӧ
-	output	:	����1:δ��⵽DS18B20�Ĵ���
-				����0:����
----------------------------------*/
unsigned char ds18B20_check(void) 	   
{   
	unsigned char retry=0;	 //���������
    while (DQ&&retry<40)
	{
		retry++;
		delay_us(5);		//���5us
	}; 
	retry=0;	 
    while (!DQ&&retry<45)	//����240us����ʱ ��������ĸ�λ����
	{
		retry++;
		delay_us(5);		//���5us
	};
	if(retry>=45)return 1;	    //û�нӵ�DS18B20Ӧ��
	return 0;					//�ӵ�Ӧ��
}



//

/*--------------------------------------------------
-	function:	��ʼ��DS18B20  ͬʱ���DS�Ĵ���
-   output	:	����1:������
-				����0:���� 
--------------------------------------------------*/  
unsigned char ds18b20_init(void)
{
  unsigned char m;
  ow_reset();         //��λ����
  m=ds18B20_check();  //�ȵ�dsӦ��
  return m;
}


//

/*----------------------------------
-	function:  DS18B20д�����
-			   ��1-WIRE ������д1���ֽ�
-	input   :  val Ҫд���ֽ�		  
----------------------------------*/
void write_byte(unsigned char val)
{
 unsigned char i,testb;
 for(i=8;i>0;i--)
 { 
   testb=val&0x01;            //���λ�Ƴ�
   val=val>>1;
   if(testb)				  //д1
   {
   	 DQ=0;
	 delay_us(4);			  //4us
	 DQ=1;
	 delay_us(60);			  //60us
   }
   else						  //д0
   {
     DQ=0;
	 delay_us(60);			  //60us
	 DQ=1;
	 delay_us(4);			  //4us
   }
 }
}


/*----------------------------------------------------------
-	function: DS18B20��1�ֽں���
-	output	: ����ֵΪ��ȡ�ֽ�ֵ
-	note	: һ��1bit�Ķ�ȡ������Ҫ60us
			  ���ζ�ȡ֮����Ҫ����1us�Ļָ�ʱ��
    		  ���ζ�ȡ1bit�������Ͳ��ܳ���15us Ȼ����������		
----------------------------------------------------------*/
unsigned char read_byte(void)
{
 unsigned char i;
 unsigned char value=0;
 for(i=8;i>0;i--)
 {
  DQ=1;
  value>>=1;   //value=value>>1
  DQ=0;                                  
  delay_us(2);							 //����2us 
  DQ=1;                                  //����
  delay_us(10);							 //����10us	 ׼���������ߵ�ǰ����
  if(DQ)value|=0x80;					 //����ǰ����ֵ������ʱ����
  delay_us(50);                          //50 us��ʱ  ���һ�ζ�ȡ����ʱ��һ�ζ�ȡ����60us������1us�Ļָ�ʱ��
 }
 DQ=1;
 return(value);
}

/*----------------------------------
-	function: �����¶Ⱥ���
-   output  : �¶�ֵΪshort���� ������	
----------------------------------*/
float Read_Temp(void)	  //short���Ա�ʾ-32768~+32767
{
  unsigned char TL,TH;
  unsigned char temp;
  short t;
  ow_reset();                  //���߸�λ
  ds18B20_check();			   //�ȴ�DSӦ��
  write_byte(0xcc);            //������
  write_byte(0x44);            //��ת������
  ow_reset();				   //��λ
  ds18B20_check();			   //�ȴ�DSӦ��
  write_byte(0xcc);            //������
  write_byte(0xbe);			   //���Ͷ��¶�����
  TL=read_byte();    //���¶�ֵ�ĵ��ֽ�
  TH=read_byte();    //���¶�ֵ�ĸ��ֽ�
  t=TH;

  if(TH>7)
  {
    TH=~TH;
    TL=~TL; 
    temp=0;//�¶�Ϊ��   
  }else temp=1;
  t<<=8;             
  t+=TL;             // ���ֽںϳ�һ�����ͱ�����
  t=(float)t*0.625;  //0.0625Ϊ12λ�¶Ȳɼ��ķֱ���	   tΪ�ɼ�����ֵ  ��������10����ȡС�����һλ
  if(temp)return t;
  else return -t;
}




