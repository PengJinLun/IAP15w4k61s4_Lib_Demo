#include 	"led_demo.h"
#include	"config.h"
#include	"GPIO.h"
#include	"delay.h"




/******************** IO配置函数 **************************/
void	LED_GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
	GPIO_InitStructure.Pin  = GPIO_Pin_5;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	//初始化
}



/******************** led测试函数 **************************/
void 	Led_Demo(void)
{	
	LED_GPIO_config();
	
	while(1)
	{
	   P35 = !P35;
	   delay_ms(1000);
	}
}