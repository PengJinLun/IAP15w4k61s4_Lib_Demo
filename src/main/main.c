#include	"config.h"
#include 	"demo.h"
#include 	"temperature_controller.h"


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

    Tprt_ctlr_Init();
	while(1)
	{
			
		//delay_ms(100);
	}
}
