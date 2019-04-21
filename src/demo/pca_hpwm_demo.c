


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

#include 	"pca_hpwm_demo.h"
#include	"config.h"
#include	"PCA.h"
#include	"delay.h"

/*************	功能说明	**************

输出3路变化的PWM信号。类似"呼吸灯"的驱动.

PWM0  为8位PWM.
PWM1  为7位PWM.
PWM2  为6位PWM.


******************************************/

/*************	本地常量声明	**************/


/*************	本地变量声明	**************/

static u8	pwm0,pwm1,pwm2;
static bit	B_PWM0_Dir,B_PWM1_Dir,B_PWM2_Dir;	//方向, 0为+, 1为-.

/*************	本地函数声明	**************/



/*************  外部函数和变量声明 *****************/


static void	PCA_config(void)
{
	PCA_InitTypeDef		PCA_InitStructure;

	PCA_InitStructure.PCA_Clock    = PCA_Clock_1T;		//PCA_Clock_1T, PCA_Clock_2T, PCA_Clock_4T, PCA_Clock_6T, PCA_Clock_8T, PCA_Clock_12T, PCA_Clock_Timer0_OF, PCA_Clock_ECI
	PCA_InitStructure.PCA_IoUse    = PCA_P34_P35_P36_P37;	//PCA_P12_P11_P10_P37, PCA_P34_P35_P36_P37, PCA_P24_P25_P26_P27
	PCA_InitStructure.PCA_Interrupt_Mode = DISABLE;		//ENABLE, DISABLE
	PCA_InitStructure.PCA_Polity   = PolityLow;			//优先级设置	PolityHigh,PolityLow
	PCA_InitStructure.PCA_RUN      = DISABLE;			//ENABLE, DISABLE
	PCA_Init(PCA_Counter,&PCA_InitStructure);

	PCA_InitStructure.PCA_Mode     = PCA_Mode_PWM;		//PCA_Mode_PWM, PCA_Mode_Capture, PCA_Mode_SoftTimer, PCA_Mode_HighPulseOutput
	PCA_InitStructure.PCA_PWM_Wide = PCA_PWM_8bit;		//PCA_PWM_8bit, PCA_PWM_7bit, PCA_PWM_6bit
	PCA_InitStructure.PCA_Interrupt_Mode = DISABLE;		//PCA_Rise_Active, PCA_Fall_Active, ENABLE, DISABLE
	PCA_InitStructure.PCA_Value    = 128 << 8;			//对于PWM,高8位为PWM占空比
	PCA_Init(PCA0,&PCA_InitStructure);

	PCA_InitStructure.PCA_Mode     = PCA_Mode_PWM;		//PCA_Mode_PWM, PCA_Mode_Capture, PCA_Mode_SoftTimer, PCA_Mode_HighPulseOutput
	PCA_InitStructure.PCA_PWM_Wide = PCA_PWM_7bit;		//PCA_PWM_8bit, PCA_PWM_7bit, PCA_PWM_6bit
	PCA_InitStructure.PCA_Interrupt_Mode = DISABLE;		//PCA_Rise_Active, PCA_Fall_Active, ENABLE, DISABLE
	PCA_InitStructure.PCA_Value    = 64 << 8;			//对于PWM,高8位为PWM占空比
	PCA_Init(PCA1,&PCA_InitStructure);

	PCA_InitStructure.PCA_Mode     = PCA_Mode_PWM;		//PCA_Mode_PWM, PCA_Mode_Capture, PCA_Mode_SoftTimer, PCA_Mode_HighPulseOutput
	PCA_InitStructure.PCA_PWM_Wide = PCA_PWM_6bit;		//PCA_PWM_8bit, PCA_PWM_7bit, PCA_PWM_6bit
	PCA_InitStructure.PCA_Interrupt_Mode = DISABLE;		//PCA_Rise_Active, PCA_Fall_Active, ENABLE, DISABLE
	PCA_InitStructure.PCA_Value    = 32 << 8;			//对于PWM,高8位为PWM占空比
	PCA_Init(PCA2,&PCA_InitStructure);

	CR = 1;
}


/******************** task A **************************/
void PCA_Hpwd_Demo(void)
{
	P3M1 &= ~(0x38);	//P3.5 P3.6 P3.7 设置为推挽输出
	P3M0 |=  (0x38);
	PCA_config();
	pwm0 = 128;
	pwm1 = 64;
	pwm2 = 32;
	B_PWM0_Dir = 0;
	B_PWM1_Dir = 0;
	B_PWM2_Dir = 0;

	UpdatePwm(PCA0,pwm0);
	UpdatePwm(PCA1,pwm1);
	UpdatePwm(PCA2,pwm2);
//	EA = 1;
	
	while (1)
	{
		delay_ms(5);

		if(B_PWM0_Dir)
		{
				pwm0--;
				if(pwm0 <= 20)	B_PWM0_Dir = 0;	//8位PWM
		}
		else{
			pwm0++;
			if(pwm0 >= 220)	B_PWM0_Dir = 1;	//8位PWM
		}	
		UpdatePwm(PCA0,pwm0);

		if(B_PWM1_Dir)
		{
				pwm1--;
				if(pwm1 <= 8)		B_PWM1_Dir = 0;	//7位PWM
		}
		else{
			pwm1++;
			if(pwm1 >= 120)	B_PWM1_Dir = 1;	//7位PWM
		}	
		UpdatePwm(PCA1,pwm1);

		if(B_PWM2_Dir)
		{
				pwm2--;
				if(pwm2 <= 4)		B_PWM2_Dir = 0;	//6位PWM
		}
		else{
			pwm2++;
		 	if(pwm2 >= 60)	B_PWM2_Dir = 1;	//6位PWM
		}
		UpdatePwm(PCA2,pwm2);
	}
}




