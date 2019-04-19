#include 	"led_demo.h"
#include	"config.h"
#include	"GPIO.h"
#include	"delay.h"




/******************** IO���ú��� **************************/
void	LED_GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ����
	GPIO_InitStructure.Pin  = GPIO_Pin_5;		//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	//��ʼ��
}



/******************** led���Ժ��� **************************/
void 	Led_Demo(void)
{	
	LED_GPIO_config();
	
	while(1)
	{
	   P35 = !P35;
	   delay_ms(1000);
	}
}