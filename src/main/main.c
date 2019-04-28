#include	"config.h"
#include 	"demo.h"
#include 	"temperature_controller.h"

void IO_init(void)
{
  P0M0 = 0X00;
  P0M1 = 0X00;

  P1M0 = 0X00;
  P1M1 = 0X00;

  P2M0 = 0X00;
  P2M1 = 0X00;

  P3M0 = 0X00;
  P3M1 = 0X00;

  P4M0 = 0X00;
  P4M1 = 0X00;  
}

int main(void)
{
	IO_init();

	#ifdef LED_DEMO_EN
		Led_Demo();
	#endif

	#ifdef TIMER_DEMO_EN
		Timer_Demo();
	#endif

	#ifdef ADC_DEMO
		//ADC_Demo();
		ADC0_Demo();
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

	#ifdef TPRT_CTLR_EN
    	Tprt_ctlr_Init();
		Tprt_ctlr_Demo();
	#endif

	#ifdef NTC_TPRT_SENSOR_EN
		Ntc_Tprt_Sensor_Init();
		Ntc_Tprt_Sensor_Demo();
	#endif

	#ifdef K_THERMOCOUPLE_EN
		K_thermocouple_Init();
		K_thermocouple_Demo();
	#endif

	while(1)
	{
	
	}
}
