
/*------------------------------------------------------------------*/
/* --- STC MCU International Limited -------------------------------*/
/* --- STC 1T Series MCU RC Demo -----------------------------------*/
/* --- Mobile: (86)13922805190 -------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ---------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ---------------------*/
/* --- Web: www.GXWMCU.com -----------------------------------------*/
/* --- QQ:  800003751 ----------------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/


#include	"config.h"
#include	"PCA.h"
#include	"delay.h"

/*************	功能说明	**************

输出3路9~16位变化的PWM信号。类似"呼吸灯"的驱动.

PWM频率 = MAIN_Fosc / PWM_DUTY, 假设 MAIN_Fosc = 24MHZ, PWM_DUTY = 6000, 则输出PWM频率为4000HZ.

******************************************/

/*************	本地常量声明	**************/


/*************	本地变量声明	**************/

u16	pwm0,pwm1,pwm2;


/*************	本地函数声明	**************/



/*************  外部函数和变量声明 *****************/


static void	PCA_config(void)
{
	PCA_InitTypeDef		PCA_InitStructure;

	PCA_InitStructure.PCA_Clock    = PCA_Clock_1T;		//PCA_Clock_1T, PCA_Clock_2T, PCA_Clock_4T, PCA_Clock_6T, PCA_Clock_8T, PCA_Clock_12T, PCA_Clock_Timer0_OF, PCA_Clock_ECI
	PCA_InitStructure.PCA_IoUse    = PCA_P34_P35_P36_P37;	//PCA_P12_P11_P10_P37, PCA_P34_P35_P36_P37, PCA_P24_P25_P26_P27
	PCA_InitStructure.PCA_Interrupt_Mode = DISABLE;		//ENABLE, DISABLE
	PCA_InitStructure.PCA_Polity   = PolityHigh;			//优先级设置	PolityHigh,PolityLow
	PCA_InitStructure.PCA_RUN      = DISABLE;			//ENABLE, DISABLE
	PCA_Init(PCA_Counter,&PCA_InitStructure);

	PCA_InitStructure.PCA_Mode     = PCA_Mode_HighPulseOutput;		//PCA_Mode_PWM, PCA_Mode_Capture, PCA_Mode_SoftTimer, PCA_Mode_HighPulseOutput
	PCA_InitStructure.PCA_PWM_Wide = 0;					//PCA_PWM_8bit, PCA_PWM_7bit, PCA_PWM_6bit
	PCA_InitStructure.PCA_Interrupt_Mode = ENABLE;		//PCA_Rise_Active, PCA_Fall_Active, ENABLE, DISABLE
	PCA_InitStructure.PCA_Value    = 65535;			//对于软件定时, 为匹配比较值
	PCA_Init(PCA0,&PCA_InitStructure);

	PCA_InitStructure.PCA_Mode     = PCA_Mode_HighPulseOutput;		//PCA_Mode_PWM, PCA_Mode_Capture, PCA_Mode_SoftTimer, PCA_Mode_HighPulseOutput
	PCA_InitStructure.PCA_PWM_Wide = 0;					//PCA_PWM_8bit, PCA_PWM_7bit, PCA_PWM_6bit
	PCA_InitStructure.PCA_Interrupt_Mode = ENABLE;		//PCA_Rise_Active, PCA_Fall_Active, ENABLE, DISABLE
	PCA_InitStructure.PCA_Value    = 65535;				//对于软件定时, 为匹配比较值
	PCA_Init(PCA1,&PCA_InitStructure);

	PCA_InitStructure.PCA_Mode     = PCA_Mode_HighPulseOutput;		//PCA_Mode_PWM, PCA_Mode_Capture, PCA_Mode_SoftTimer, PCA_Mode_HighPulseOutput
	PCA_InitStructure.PCA_PWM_Wide = 0;					//PCA_PWM_8bit, PCA_PWM_7bit, PCA_PWM_6bit
	PCA_InitStructure.PCA_Interrupt_Mode = ENABLE;		//PCA_Rise_Active, PCA_Fall_Active, ENABLE, DISABLE
	PCA_InitStructure.PCA_Value    = 65535;				//对于软件定时, 为匹配比较值
	PCA_Init(PCA2,&PCA_InitStructure);
}


/******************** task A **************************/
void PCA_Pwm_Demo(void)
{
	P3M1 &= ~(0x38);	//P3.5 P3.6 P3.7 设置为推挽输出
	P3M0 |=  (0x38);
	
	PCA_config();
	pwm0 = (PWM0_DUTY / 4 * 1);	//给PWM一个初值
	pwm1 = (PWM1_DUTY / 4 * 2);
	pwm2 = (PWM2_DUTY / 4 * 3);

	PWMn_Update(PCA0,pwm0);
	PWMn_Update(PCA1,pwm1);
	PWMn_Update(PCA2,pwm2);

	EA = 1;
	
	while (1)
	{
		delay_ms(50);

		 pwm0 += 100;
		 pwm1 += 100;
		 pwm2 += 100;
		if(pwm0 >= PWM0_HIGH_MAX)	pwm0 = PWM0_HIGH_MIN;
		PWMn_Update(PCA0,pwm0);

		if(pwm1 >= PWM1_HIGH_MAX)	pwm1 = PWM1_HIGH_MIN;
		PWMn_Update(PCA1,pwm1);

		if(pwm2 >= PWM2_HIGH_MAX)	pwm2 = PWM2_HIGH_MIN;
		PWMn_Update(PCA2,pwm2);
	}
}



