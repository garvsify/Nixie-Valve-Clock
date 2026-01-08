#include "system.h"

uint8_t Check_Rotary_Encoder(void){

	master.encoder_first = master.encoder_second;
	master.encoder_second = __HAL_TIM_GET_COUNTER(&htim2) >> 1;

	if(master.system_mode_tracker.current_mode == HH_ADJUST_MODE ||
		master.system_mode_tracker.current_mode == MM_ADJUST_MODE ||
		 master.system_mode_tracker.current_mode == SS_ADJUST_MODE){

		if(master.system_mode_tracker.current_mode == HH_ADJUST_MODE){

			if(master.encoder_second > master.encoder_first){

				if(master.encoder_first < 5 && master.encoder_second > 2147483642){ //roll over condition

					if((int32_t)master.time_adjust.Hours_Bin - ((2147483647 - (int32_t)master.encoder_second) + (int32_t)master.encoder_first + 1) < 0){

						int32_t temp = (int32_t)master.time_adjust.Hours_Bin - ((2147483647 - (int32_t)master.encoder_second) + (int32_t)master.encoder_first);
						temp += 23;

						master.time_adjust.Hours_Bin = temp;
					}
					else{

						master.time_adjust.Hours_Bin -= (uint8_t)((2147483647 - (int32_t)master.encoder_second) + (int32_t)master.encoder_first + 1);
					}
				}
				else{ //not a roll over

					if((master.time_adjust.Hours_Bin + (master.encoder_second - master.encoder_first)) > 23){

						uint8_t temp = master.time_adjust.Hours_Bin + (master.encoder_second - master.encoder_first);
						temp -= 23;

						master.time_adjust.Hours_Bin = temp - 1;
					}
					else{

						master.time_adjust.Hours_Bin += (master.encoder_second - master.encoder_first);
					}
				}
			}
			else if(master.encoder_second < master.encoder_first){

				if(master.encoder_second < 5 && master.encoder_first > 2147483642){ //roll over condition

					if((int32_t)master.time_adjust.Hours_Bin + ((2147483647 - (int32_t)master.encoder_first) + (int32_t)master.encoder_second + 1) > 23){

						int32_t temp = (int32_t)master.time_adjust.Hours_Bin + ((2147483647 - (int32_t)master.encoder_first) + (int32_t)master.encoder_second);
						temp -= 23;

						master.time_adjust.Hours_Bin = temp;
					}
					else{

						master.time_adjust.Hours_Bin += (uint8_t)((2147483647 - (int32_t)master.encoder_first) + (int32_t)master.encoder_second + 1);
					}
				}
				else{ //not a roll over

					if(((int32_t)master.time_adjust.Hours_Bin - ((int32_t)master.encoder_first - (int32_t)master.encoder_second)) < 0){

						int32_t temp = (int32_t)master.time_adjust.Hours_Bin - ((int32_t)master.encoder_first - (int32_t)master.encoder_second);
						temp += 23;

						master.time_adjust.Hours_Bin = (uint8_t)(temp + 1);
					}
					else{

						master.time_adjust.Hours_Bin -= (uint8_t)(master.encoder_first - master.encoder_second);
					}
				}
			}
		}
		if(master.system_mode_tracker.current_mode == MM_ADJUST_MODE){

			if(master.encoder_second > master.encoder_first){

				if(master.encoder_first < 5 && master.encoder_second > 2147483642){ //roll over condition

					if((master.time_adjust.Minutes_Bin - ((2147483647 - master.encoder_second) + master.encoder_first)) < 0){

						int16_t temp = (int16_t)master.time_adjust.Minutes_Bin - ((2147483647 - master.encoder_second) + master.encoder_first);
						temp += 23;

						master.time_adjust.Minutes_Bin = temp - 1;
					}
					else{

						master.time_adjust.Minutes_Bin -= ((2147483647 - master.encoder_second) + master.encoder_first);
					}
				}
				else{ //not a roll over

					if((master.time_adjust.Minutes_Bin + (master.encoder_second - master.encoder_first)) > 59){

						uint8_t temp = master.time_adjust.Minutes_Bin + (master.encoder_second - master.encoder_first);
						temp -= 59;

						master.time_adjust.Minutes_Bin = temp - 1;
					}
					else{

						master.time_adjust.Minutes_Bin += (master.encoder_second - master.encoder_first);
					}
				}
			}
			else if(master.encoder_second < master.encoder_first){

				if(master.encoder_second < 5 && master.encoder_first > 2147483642){ //roll over condition

					if((int16_t)master.time_adjust.Minutes_Bin + ((2147483647 - master.encoder_first) + master.encoder_second) > 59){

						uint8_t temp = master.time_adjust.Minutes_Bin + ((2147483647 - master.encoder_first) + master.encoder_second);
						temp -= 59;

						master.time_adjust.Minutes_Bin = temp - 1;
					}
					else{

						master.time_adjust.Minutes_Bin += ((2147483647 - master.encoder_first) + master.encoder_second);
					}
				}
				else{ //not a roll over

					if(((int16_t)master.time_adjust.Minutes_Bin - ((int16_t)master.encoder_first - (int16_t)master.encoder_second)) < 0){

						int16_t temp = (int16_t)master.time_adjust.Minutes_Bin - ((int16_t)master.encoder_first - (int16_t)master.encoder_second);
						temp += 59;

						master.time_adjust.Minutes_Bin = temp + 1;
					}
					else{

						master.time_adjust.Minutes_Bin -= (master.encoder_first - master.encoder_second);
					}
				}
			}
		}
		if(master.system_mode_tracker.current_mode == SS_ADJUST_MODE){

			if(master.encoder_second > master.encoder_first){

				if(master.encoder_first < 5 && master.encoder_second > 2147483642){ //roll over condition

					if((master.time_adjust.Seconds_Bin - ((2147483647 - master.encoder_second) + master.encoder_first)) < 0){

						int16_t temp = (int16_t)master.time_adjust.Seconds_Bin - ((2147483647 - master.encoder_second) + master.encoder_first);
						temp += 59;

						master.time_adjust.Seconds_Bin = temp - 1;
					}
					else{

						master.time_adjust.Seconds_Bin -= ((2147483647 - master.encoder_second) + master.encoder_first);
					}
				}
				else{ //not a roll over

					if((master.time_adjust.Seconds_Bin + (master.encoder_second - master.encoder_first)) > 59){

						uint8_t temp = master.time_adjust.Seconds_Bin + (master.encoder_second - master.encoder_first);
						temp -= 59;

						master.time_adjust.Seconds_Bin = temp - 1;
					}
					else{

						master.time_adjust.Seconds_Bin += (master.encoder_second - master.encoder_first);
					}
				}
			}
			else if(master.encoder_second < master.encoder_first){

				if(master.encoder_second < 5 && master.encoder_first > 2147483642){ //roll over condition

					if((int16_t)master.time_adjust.Seconds_Bin + ((2147483647 - master.encoder_first) + master.encoder_second) > 59){

						uint8_t temp = master.time_adjust.Seconds_Bin + ((2147483647 - master.encoder_first) + master.encoder_second);
						temp -= 59;

						master.time_adjust.Seconds_Bin = temp - 1;
					}
					else{

						master.time_adjust.Seconds_Bin += ((2147483647 - master.encoder_first) + master.encoder_second);
					}
				}
				else{ //not a roll over

					if(((int16_t)master.time_adjust.Seconds_Bin - ((int16_t)master.encoder_first - (int16_t)master.encoder_second)) < 0){

						int16_t temp = (int16_t)master.time_adjust.Seconds_Bin - ((int16_t)master.encoder_first - (int16_t)master.encoder_second);
						temp += 59;

						master.time_adjust.Seconds_Bin = temp + 1;
					}
					else{

						master.time_adjust.Seconds_Bin -= (master.encoder_first - master.encoder_second);
					}
				}
			}
		}
		master.time_adjust.adjust_time.Hours = RTC_ByteToBcd2(master.time_adjust.Hours_Bin);
		master.time_adjust.adjust_time.Minutes = RTC_ByteToBcd2(master.time_adjust.Minutes_Bin);
		master.time_adjust.adjust_time.Seconds = RTC_ByteToBcd2(master.time_adjust.Seconds_Bin);
	}
	if(master.system_mode_tracker.current_mode == ALARM_SET_HH_MODE ||
		master.system_mode_tracker.current_mode == ALARM_SET_MM_MODE ||
		 master.system_mode_tracker.current_mode == ALARM_SET_SS_MODE){

		if(master.system_mode_tracker.current_mode == ALARM_SET_HH_MODE){

			if(master.encoder_second > master.encoder_first){

				if(master.encoder_first < 5 && master.encoder_second > 2147483642){ //roll over condition

					if((master.alarm.Hours_Bin - ((2147483647 - master.encoder_second) + master.encoder_first)) < 0){

						int16_t temp = (int16_t)master.alarm.Hours_Bin - ((2147483647 - master.encoder_second) + master.encoder_first);
						temp += 23;

						master.alarm.Hours_Bin = temp - 1;
					}
					else{

						master.alarm.Hours_Bin -= ((2147483647 - master.encoder_second) + master.encoder_first);
					}
				}
				else{ //not a roll over

					if((master.alarm.Hours_Bin + (master.encoder_second - master.encoder_first)) > 23){

						uint8_t temp = master.alarm.Hours_Bin + (master.encoder_second - master.encoder_first);
						temp -= 23;

						master.alarm.Hours_Bin = temp - 1;
					}
					else{

						master.alarm.Hours_Bin += (master.encoder_second - master.encoder_first);
					}
				}
			}
			else if(master.encoder_second < master.encoder_first){

				if(master.encoder_second < 5 && master.encoder_first > 2147483642){ //roll over condition

					if((int16_t)master.alarm.Hours_Bin + ((2147483647 - master.encoder_first) + master.encoder_second) > 23){

						uint8_t temp = master.alarm.Hours_Bin + ((2147483647 - master.encoder_first) + master.encoder_second);
						temp -= 23;

						master.alarm.Hours_Bin = temp - 1;
					}
					else{

						master.alarm.Hours_Bin += ((2147483647 - master.encoder_first) + master.encoder_second);
					}
				}
				else{ //not a roll over

					if(((int16_t)master.alarm.Hours_Bin - ((int16_t)master.encoder_first - (int16_t)master.encoder_second)) < 0){

						int16_t temp = (int16_t)master.alarm.Hours_Bin - ((int16_t)master.encoder_first - (int16_t)master.encoder_second);
						temp += 23;

						master.alarm.Hours_Bin = temp + 1;
					}
					else{

						master.alarm.Hours_Bin -= (master.encoder_first - master.encoder_second);
					}
				}
			}
		}
		if(master.system_mode_tracker.current_mode == ALARM_SET_MM_MODE){

			if(master.encoder_second > master.encoder_first){

				if(master.encoder_first < 5 && master.encoder_second > 2147483642){ //roll over condition

					if((master.alarm.Minutes_Bin - ((2147483647 - master.encoder_second) + master.encoder_first)) < 0){

						int16_t temp = (int16_t)master.alarm.Minutes_Bin - ((2147483647 - master.encoder_second) + master.encoder_first);
						temp += 23;

						master.alarm.Minutes_Bin = temp - 1;
					}
					else{

						master.alarm.Minutes_Bin -= ((2147483647 - master.encoder_second) + master.encoder_first);
					}
				}
				else{ //not a roll over

					if((master.alarm.Minutes_Bin + (master.encoder_second - master.encoder_first)) > 59){

						uint8_t temp = master.alarm.Minutes_Bin + (master.encoder_second - master.encoder_first);
						temp -= 59;

						master.alarm.Minutes_Bin = temp - 1;
					}
					else{

						master.alarm.Minutes_Bin += (master.encoder_second - master.encoder_first);
					}
				}
			}
			else if(master.encoder_second < master.encoder_first){

				if(master.encoder_second < 5 && master.encoder_first > 2147483642){ //roll over condition

					if((int16_t)master.alarm.Minutes_Bin + ((2147483647 - master.encoder_first) + master.encoder_second) > 59){

						uint8_t temp = master.alarm.Minutes_Bin + ((2147483647 - master.encoder_first) + master.encoder_second);
						temp -= 59;

						master.alarm.Minutes_Bin = temp - 1;
					}
					else{

						master.alarm.Minutes_Bin += ((2147483647 - master.encoder_first) + master.encoder_second);
					}
				}
				else{ //not a roll over

					if(((int16_t)master.alarm.Minutes_Bin - ((int16_t)master.encoder_first - (int16_t)master.encoder_second)) < 0){

						int16_t temp = (int16_t)master.alarm.Minutes_Bin - ((int16_t)master.encoder_first - (int16_t)master.encoder_second);
						temp += 59;

						master.alarm.Minutes_Bin = temp + 1;
					}
					else{

						master.alarm.Minutes_Bin -= (master.encoder_first - master.encoder_second);
					}
				}
			}
		}
		if(master.system_mode_tracker.current_mode == ALARM_SET_SS_MODE){

			if(master.encoder_second > master.encoder_first){

				if(master.encoder_first < 5 && master.encoder_second > 2147483642){ //roll over condition

					if((master.alarm.Seconds_Bin - ((2147483647 - master.encoder_second) + master.encoder_first)) < 0){

						int16_t temp = (int16_t)master.alarm.Seconds_Bin - ((2147483647 - master.encoder_second) + master.encoder_first);
						temp += 23;

						master.alarm.Seconds_Bin = temp - 1;
					}
					else{

						master.alarm.Seconds_Bin -= ((2147483647 - master.encoder_second) + master.encoder_first);
					}
				}
				else{ //not a roll over

					if((master.alarm.Seconds_Bin + (master.encoder_second - master.encoder_first)) > 59){

						uint8_t temp = master.alarm.Seconds_Bin + (master.encoder_second - master.encoder_first);
						temp -= 59;

						master.alarm.Seconds_Bin = temp - 1;
					}
					else{

						master.alarm.Seconds_Bin += (master.encoder_second - master.encoder_first);
					}
				}
			}
			else if(master.encoder_second < master.encoder_first){

				if(master.encoder_second < 5 && master.encoder_first > 2147483642){ //roll over condition

					if((int16_t)master.alarm.Seconds_Bin + ((2147483647 - master.encoder_first) + master.encoder_second) > 59){

						uint8_t temp = master.alarm.Seconds_Bin + ((2147483647 - master.encoder_first) + master.encoder_second);
						temp -= 59;

						master.alarm.Seconds_Bin = temp - 1;
					}
					else{

						master.alarm.Seconds_Bin += ((2147483647 - master.encoder_first) + master.encoder_second);
					}
				}
				else{ //not a roll over

					if(((int16_t)master.alarm.Seconds_Bin - ((int16_t)master.encoder_first - (int16_t)master.encoder_second)) < 0){

						int16_t temp = (int16_t)master.alarm.Seconds_Bin - ((int16_t)master.encoder_first - (int16_t)master.encoder_second);
						temp += 59;

						master.alarm.Seconds_Bin = temp + 1;
					}
					else{

						master.alarm.Seconds_Bin -= (master.encoder_first - master.encoder_second);
					}
				}
			}
		}
		master.alarm.alarm_time.Hours = RTC_ByteToBcd2(master.alarm.Hours_Bin);
		master.alarm.alarm_time.Minutes = RTC_ByteToBcd2(master.alarm.Minutes_Bin);
		master.alarm.alarm_time.Seconds = RTC_ByteToBcd2(master.alarm.Seconds_Bin);
	}

	return 1;
}

uint8_t Check_Rotary_Encoder_Switch_State(volatile struct Rotary_Encoder_Switch_States *rotary_encoder_switch_states_ptr){

	static uint8_t extend_rising_edge = 0;

	static uint8_t rotary_encoder_switch_state_counter = ROTARY_ENCODER_SWITCH_CONFIDENCE_COUNT;

	uint8_t switch_state = (uint8_t)HAL_GPIO_ReadPin(GPIO_EXTI15_SW_GPIO_Port, GPIO_EXTI15_SW_Pin);

	if(switch_state == 0){

		if(rotary_encoder_switch_state_counter != 0){

			rotary_encoder_switch_state_counter--;

		}
	}
	else{

		if(rotary_encoder_switch_state_counter != ROTARY_ENCODER_SWITCH_CONFIDENCE_COUNT){

			if(extend_rising_edge == COUNT_TO_DELAY_RISING_ROTARY_ENCODER_EDGE){

				rotary_encoder_switch_state_counter++;
				extend_rising_edge = 0;
			}
			else{

				extend_rising_edge++;
			}
		}
	}

	if(rotary_encoder_switch_state_counter == 0){

		rotary_encoder_switch_states_ptr->rotary_encoder_switch_state = ROTARY_ENCODER_SWITCH_STATE_DEPRESSED;
	}
	else if(rotary_encoder_switch_state_counter == ROTARY_ENCODER_SWITCH_CONFIDENCE_COUNT){

		rotary_encoder_switch_states_ptr->rotary_encoder_switch_state = ROTARY_ENCODER_SWITCH_STATE_NOT_DEPRESSED;
	}

	return 1;
}

