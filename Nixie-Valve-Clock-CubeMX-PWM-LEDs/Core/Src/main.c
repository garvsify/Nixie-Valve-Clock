#include "main.h"

int main(void){

	System_Init();

	while(1){

		if(master.alarm.alarm_triggered != 1){

			HAL_Delay(ROTARY_ENCODER_MS_DELAY_COUNT_BETWEEN_CHECKS);
			Check_Rotary_Encoder();
		}
		else{

			Sound_Alarm();
		}
		if(master.leds.Double_Flash_Red_LED == 1){

			Double_Flash_Red_Rotary_Encoder_LED();
		}
	}

	return 1;
}
