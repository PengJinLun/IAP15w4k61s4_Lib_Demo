


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

/*************	����˵��	**************

���3·�仯��PWM�źš�����"������"������.

PWM0  Ϊ8λPWM.
PWM1  Ϊ7λPWM.
PWM2  Ϊ6λPWM.


******************************************/

/*************	���س�������	**************/


/*************	���ر�������	**************/

static u8	pwm0,pwm1,pwm2;
static bit	B_PWM0_Dir,B_PWM1_Dir,B_PWM2_Dir;	//����, 0Ϊ+, 1Ϊ-.

/*************	���غ�������	**************/



/*************  �ⲿ�����ͱ������� *****************/


static void	PCA_config(void)
{
	PCA_InitTypeDef		PCA_InitStructure;

	PCA_InitStructure.PCA_Clock    = PCA_Clock_1T;		//PCA_Clock_1T, PCA_Clock_2T, PCA_Clock_4T, PCA_Clock_6T, PCA_Clock_8T, PCA_Clock_12T, PCA_Clock_Timer0_OF, PCA_Clock_ECI
	PCA_InitStructure.PCA_IoUse    = PCA_P34_P35_P36_P37;	//PCA_P12_P11_P10_P37, PCA_P34_P35_P36_P37, PCA_P24_P25_P26_P27
	PCA_InitStructure.PCA_Interrupt_Mode = DISABLE;		//ENABLE, DISABLE
	PCA_InitStructure.PCA_Polity   = PolityLow;			//���ȼ�����	PolityHigh,PolityLow
	PCA_InitStructure.PCA_RUN      = DISABLE;			//ENABLE, DISABLE
	PCA_Init(PCA_Counter,&PCA_InitStructure);

	PCA_InitStructure.PCA_Mode     = PCA_Mode_PWM;		//PCA_Mode_PWM, PCA_Mode_Capture, PCA_Mode_SoftTimer, PCA_Mode_HighPulseOutput
	PCA_InitStructure.PCA_PWM_Wide = PCA_PWM_8bit;		//PCA_PWM_8bit, PCA_PWM_7bit, PCA_PWM_6bit
	PCA_InitStructure.PCA_Interrupt_Mode = DISABLE;		//PCA_Rise_Active, PCA_Fall_Active, ENABLE, DISABLE
	PCA_InitStructure.PCA_Value    = 128 << 8;			//����PWM,��8λΪPWMռ�ձ�
	PCA_Init(PCA0,&PCA_InitStructure);

	PCA_InitStructure.PCA_Mode     = PCA_Mode_PWM;		//PCA_Mode_PWM, PCA_Mode_Capture, PCA_Mode_SoftTimer, PCA_Mode_HighPulseOutput
	PCA_InitStructure.PCA_PWM_Wide = PCA_PWM_7bit;		//PCA_PWM_8bit, PCA_PWM_7bit, PCA_PWM_6bit
	PCA_InitStructure.PCA_Interrupt_Mode = DISABLE;		//PCA_Rise_Active, PCA_Fall_Active, ENABLE, DISABLE
	PCA_InitStructure.PCA_Value    = 64 << 8;			//����PWM,��8λΪPWMռ�ձ�
	PCA_Init(PCA1,&PCA_InitStructure);

	PCA_InitStructure.PCA_Mode     = PCA_Mode_PWM;		//PCA_Mode_PWM, PCA_Mode_Capture, PCA_Mode_SoftTimer, PCA_Mode_HighPulseOutput
	PCA_InitStructure.PCA_PWM_Wide = PCA_PWM_6bit;		//PCA_PWM_8bit, PCA_PWM_7bit, PCA_PWM_6bit
	PCA_InitStructure.PCA_Interrupt_Mode = DISABLE;		//PCA_Rise_Active, PCA_Fall_Active, ENABLE, DISABLE
	PCA_InitStructure.PCA_Value    = 32 << 8;			//����PWM,��8λΪPWMռ�ձ�
	PCA_Init(PCA2,&PCA_InitStructure);

	CR = 1;
}


/******************** task A **************************/
void PCA_Hpwd_Demo(void)
{
	P3M1 &= ~(0x38);	//P3.5 P3.6 P3.7 ����Ϊ�������
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
				if(pwm0 <= 20)	B_PWM0_Dir = 0;	//8λPWM
		}
		else{
			pwm0++;
			if(pwm0 >= 220)	B_PWM0_Dir = 1;	//8λPWM
		}	
		UpdatePwm(PCA0,pwm0);

		if(B_PWM1_Dir)
		{
				pwm1--;
				if(pwm1 <= 8)		B_PWM1_Dir = 0;	//7λPWM
		}
		else{
			pwm1++;
			if(pwm1 >= 120)	B_PWM1_Dir = 1;	//7λPWM
		}	
		UpdatePwm(PCA1,pwm1);

		if(B_PWM2_Dir)
		{
				pwm2--;
				if(pwm2 <= 4)		B_PWM2_Dir = 0;	//6λPWM
		}
		else{
			pwm2++;
		 	if(pwm2 >= 60)	B_PWM2_Dir = 1;	//6λPWM
		}
		UpdatePwm(PCA2,pwm2);
	}
}




