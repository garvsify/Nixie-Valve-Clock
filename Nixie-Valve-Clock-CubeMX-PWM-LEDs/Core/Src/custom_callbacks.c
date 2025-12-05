/*
 * custom_callbacks.c
 *
 *  Created on: 2 Dec 2025
 *      Author: James Garvey
 */

#include "custom_callbacks.h"
#include "main.h"

void TIM17_Multiplexer_Sequencer_Callback(TIM_HandleTypeDef *htim){

	//__HAL_TIM_SET_AUTORELOAD(&htim17, MULTIPLEXER_TIMER_PERIOD_MINUS_ONE);

	static uint8_t valve = 0;
	uint8_t BCD = 0;
	static RTC_TimeTypeDef adjust_time;
	uint8_t valve_on = 1;

	if(master.system_mode_tracker.current_mode == NORMAL_MODE){

		//get current time
		HAL_RTC_GetTime(&hrtc, &get_time, RTC_FORMAT_BCD);

		valve_on = 1;

		if(valve == 0){

			BCD = (get_time.Hours >> 4) & 0xF;
		}
		else if(valve == 1){

			BCD = get_time.Hours & 0xF;
		}
		else if(valve == 2){

			BCD = (get_time.Minutes >> 4) & 0xF;
		}
		else if(valve == 3){

			BCD = get_time.Minutes & 0xF;
		}
		else if(valve == 4){

			BCD = (get_time.Seconds >> 4) & 0xF;
		}
		else if(valve == 5){

			BCD = get_time.Seconds & 0xF;
		}
	}
	else if(master.system_mode_tracker.current_mode == ANTI_CATHODE_POISONING_MODE){

		if(master.anti_cathode_poisoning.anti_cathode_poisoning_mode_enetered == 1){

			valve = 0;
			master.anti_cathode_poisoning.anti_cathode_poisoning_mode_enetered = 0;
		}

		valve_on = 1;
		BCD = master.anti_cathode_poisoning.counter & 0xF;
	}
	else if(master.system_mode_tracker.current_mode == HH_ADJUST_MODE){

		if(valve == 0){

			if(master.time_adjust.blink_state == 1){

				valve_on = 1;
			}
			else if(master.time_adjust.blink_state == 0){

				valve_on = 0;
			}

			BCD = (adjust_time.Hours >> 4) & 0xF;
		}
		else if(valve == 1){

			if(master.time_adjust.blink_state == 1){

				valve_on = 1;
			}
			else if(master.time_adjust.blink_state == 0){

				valve_on = 0;
			}

			BCD = adjust_time.Hours & 0xF;
		}
		else if(valve == 2){

			valve_on = 1;
			BCD = (adjust_time.Minutes >> 4) & 0xF;
		}
		else if(valve == 3){

			valve_on = 1;
			BCD = adjust_time.Minutes & 0xF;
		}
		else if(valve == 4){

			valve_on = 1;
			BCD = (adjust_time.Seconds >> 4) & 0xF;
		}
		else if(valve == 5){

			valve_on = 1;
			BCD = adjust_time.Seconds & 0xF;
		}

	}
	else if(master.system_mode_tracker.current_mode == MM_ADJUST_MODE){

		if(valve == 0){

			valve_on = 1;
			BCD = (adjust_time.Hours >> 4) & 0xF;
		}
		else if(valve == 1){

			valve_on = 1;
			BCD = adjust_time.Hours & 0xF;
		}
		else if(valve == 2){

			if(master.time_adjust.blink_state == 1){

				valve_on = 1;
			}
			else if(master.time_adjust.blink_state == 0){

				valve_on = 0;
			}

			BCD = (adjust_time.Minutes >> 4) & 0xF;
		}
		else if(valve == 3){

			if(master.time_adjust.blink_state == 1){

				valve_on = 1;
			}
			else if(master.time_adjust.blink_state == 0){

				valve_on = 0;
			}

			BCD = adjust_time.Minutes & 0xF;
		}
		else if(valve == 4){

			valve_on = 1;
			BCD = (adjust_time.Seconds >> 4) & 0xF;
		}
		else if(valve == 5){

			valve_on = 1;
			BCD = adjust_time.Seconds & 0xF;
		}
	}
	else if(master.system_mode_tracker.current_mode == SS_ADJUST_MODE){

		if(valve == 0){

			valve_on = 1;
			BCD = (adjust_time.Hours >> 4) & 0xF;
		}
		else if(valve == 1){

			valve_on = 1;
			BCD = adjust_time.Hours & 0xF;
		}
		else if(valve == 2){

			valve_on = 1;
			BCD = (adjust_time.Minutes >> 4) & 0xF;
		}
		else if(valve == 3){

			valve_on = 1;
			BCD = adjust_time.Minutes & 0xF;
		}
		else if(valve == 4){

			if(master.time_adjust.blink_state == 1){

				valve_on = 1;
			}
			else if(master.time_adjust.blink_state == 0){

				valve_on = 0;
			}

			BCD = (adjust_time.Seconds >> 4) & 0xF;
		}
		else if(valve == 5){

			if(master.time_adjust.blink_state == 1){

				valve_on = 1;
			}
			else if(master.time_adjust.blink_state == 0){

				valve_on = 0;
			}

			BCD = adjust_time.Seconds & 0xF;
		}
	}

	if(valve_on){

		Write_Digit_to_Valve(valve, BCD);
	}
	else if(!valve_on){


	}

	valve++;

	if(valve == NUM_VALVES){

		valve = 0;
	}
}

void TIM16_Anti_Cathode_Poisoning_Callback(TIM_HandleTypeDef *htim){

	if(master.system_mode_tracker.current_mode != ANTI_CATHODE_POISONING_MODE){

		Set_System_Mode_and_Store_Previous_Mode(&master.system_mode_tracker, ANTI_CATHODE_POISONING_MODE);
		master.anti_cathode_poisoning.anti_cathode_poisoning_mode_enetered = 1;
	}
	if((master.anti_cathode_poisoning.cycle == (master.anti_cathode_poisoning.max_cycles - 1))
			&& (master.anti_cathode_poisoning.counter == master.anti_cathode_poisoning.max_counter)){ //final cycle and final count

		__HAL_TIM_SET_AUTORELOAD(&htim16, ANTI_CATHODE_POISONING_TIMER_WAITING_MODE_PERIOD_MINUS_ONE);
		__HAL_TIM_SET_PRESCALER(&htim16, ANTI_CATHODE_POISONING_TIMER_WAITING_MODE_PRESCALER);

		Set_System_Mode_and_Store_Previous_Mode(&master.system_mode_tracker, master.system_mode_tracker.previous_mode); //anti cathode poisoning
		//mode over, return to previous mode

		master.anti_cathode_poisoning.counter = 0;
		master.anti_cathode_poisoning.cycle = 0;
	}
	else{

		__HAL_TIM_SET_AUTORELOAD(&htim16, ANTI_CATHODE_POISONING_TIMER_ACTIVE_MODE_PERIOD_MINUS_ONE);
		__HAL_TIM_SET_PRESCALER(&htim16, ANTI_CATHODE_POISONING_TIMER_ACTIVE_MODE_PRESCALER);

		if((master.anti_cathode_poisoning.counter == master.anti_cathode_poisoning.max_counter)
				&& (master.anti_cathode_poisoning.cycle != (master.anti_cathode_poisoning.max_cycles - 1))){

			master.anti_cathode_poisoning.counter = 0;
			master.anti_cathode_poisoning.cycle++;
		}
		else{

			master.anti_cathode_poisoning.counter++;
		}
	}
}


