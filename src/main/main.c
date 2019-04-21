#include	"config.h"
#include 	"demo.h"
//#include 	"temperature_controller.h"

int main(void)
{
	#ifdef LED_DEMO_EN
		Led_Demo();
	#endif

	#ifdef TIMER_DEMO_EN
		Timer_Demo();
	#endif

	#ifdef ADC_DEMO
		ADC_Demo();
	#endif

	#ifdef ADC_BANDGAP_EN
		ADC_BandGap_demo();
	#endif

	#ifdef UART1_DEMO_EN
		UART1_Demo();
	#endif

	#ifdef DS18B20_DEMO_EN
		DS18b20_Demo();
	#endif

	#ifdef EEPROMO_DEMO_EN
		EEPROM_Demo();
	#endif
	
	#ifdef PCA_PWD_DEMO_EN
	   PCA_Pwm_Demo();
	#endif

	#ifdef PCA_HPWD_DEMO_EN
	   PCA_Hpwd_Demo();
	#endif

	#ifdef PCA_PWM_SOFTTIME_CAPCTURE_DEMO_EN
		PCA_Pwm_SoftTime_Capcture_Demo();
	#endif

	#ifdef GUI_DEMO
		GUI_Demo();
	#endif
	
	#ifdef GUI_FONT_DEMO
		GUI_Font_Demo();
	#endif

    //Tprt_ctlr_Init();
	while(1)
	{
	
	}
}
