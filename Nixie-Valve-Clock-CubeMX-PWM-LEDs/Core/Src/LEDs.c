#include "system.h"

uint8_t Double_Flash_Red_Rotary_Encoder_LED(void){

	if(master.leds.LED_counter[ROTARY_ENCODER_RED_LED_NUM] != LED_DOUBLE_FLASH_COUNT_MAX){

		if(master.leds.LED_counter[ROTARY_ENCODER_RED_LED_NUM] <= LED_SHORT_ON_COUNT){

			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, RED_LED_DOUBLE_FLASH_BRIGHTNESS_CCR);
		}
		else if(master.leds.LED_counter[ROTARY_ENCODER_RED_LED_NUM] > LED_SHORT_ON_COUNT
		          && master.leds.LED_counter[ROTARY_ENCODER_RED_LED_NUM] <= (LED_SHORT_ON_COUNT + LED_SHORT_OFF_COUNT)){

			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
		}
		else if(master.leds.LED_counter[ROTARY_ENCODER_RED_LED_NUM] > (LED_SHORT_ON_COUNT + LED_SHORT_OFF_COUNT)
				          && master.leds.LED_counter[ROTARY_ENCODER_RED_LED_NUM] <= (LED_SHORT_ON_COUNT + LED_SHORT_OFF_COUNT + LED_SHORT_ON_COUNT)){

			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, RED_LED_DOUBLE_FLASH_BRIGHTNESS_CCR);
		}
		else if(master.leds.LED_counter[ROTARY_ENCODER_RED_LED_NUM] > (LED_SHORT_ON_COUNT + LED_SHORT_OFF_COUNT + LED_SHORT_ON_COUNT)
						  && master.leds.LED_counter[ROTARY_ENCODER_RED_LED_NUM] <= (LED_SHORT_ON_COUNT + LED_SHORT_OFF_COUNT + LED_SHORT_ON_COUNT + LED_SHORT_OFF_COUNT)){

			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
		}

		master.leds.LED_counter[ROTARY_ENCODER_RED_LED_NUM]++;
	}
	else{

		master.leds.LED_counter[ROTARY_ENCODER_RED_LED_NUM] = 0;
		master.leds.Double_Flash_Red_LED = 0;
	}

	return 1;
}

uint8_t Set_Adjust_Time_LED_ON(void){

	//Set Rotary Encoder LED to Green

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, ADJUST_TIME_GREEN_LED_BRIGHTNESS_CCR);

	return 1;
}

uint8_t Set_Adjust_Time_LED_OFF(void){

	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);

	return 1;
}

uint8_t Set_Alarm_Set_LEDs_ON(void){

	//Set Rotary Encoder LEDs to Orange

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, ALARM_SET_GREEN_LED_BRIGHTNESS_CCR);

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, ALARM_SET_RED_LED_BRIGHTNESS_CCR);

	return 1;
}

uint8_t Set_Alarm_Set_LEDs_OFF(void){

	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);

	return 1;
}

uint8_t Initialise_Rotary_Encoder_LEDs(void){

	HAL_TIM_Base_Start(&htim3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);

	return 1;
}

uint8_t Initialise_Valve_LEDs(void){

	HAL_TIM_Base_Start(&htim1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);

	return 1;
}

uint8_t Set_Fault_LED_ON(void){

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, FAULT_RED_LED_BRIGHTNESS_CCR);

	return 1;
}

