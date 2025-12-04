/*
 * custom_callbacks.c
 *
 *  Created on: 2 Dec 2025
 *      Author: James Garvey
 */

#include "custom_callbacks.h"
#include "main.h"

void TIM17_Multiplexer_Sequencer_Callback(TIM_HandleTypeDef *htim){

	__HAL_TIM_SET_AUTORELOAD(&htim17, MULTIPLEXER_TIMER_PERIOD_MINUS_ONE);

	static uint8_t valve = 0;
	uint8_t BCD = 0;

	if(anti_cathode_poisoning.timer_mode == WAITING_MODE){

		//get current time
		HAL_RTC_GetTime(&hrtc, &get_time, RTC_FORMAT_BCD);

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
	else if(anti_cathode_poisoning.timer_mode == ACTIVE_MODE){

		if(anti_cathode_poisoning.mode_changed == 1){

			valve = 0;
			anti_cathode_poisoning.mode_changed = 0;
		}

		BCD = anti_cathode_poisoning.counter & 0xF;
	}

	Write_Digit_to_Valve(valve, BCD);

	valve++;

	if(valve == NUM_VALVES){

		valve = 0;
	}
}

void TIM16_Anti_Cathode_Poisoning_Callback(TIM_HandleTypeDef *htim){

	if(anti_cathode_poisoning.timer_mode == WAITING_MODE){

		anti_cathode_poisoning.timer_mode = ACTIVE_MODE;
		anti_cathode_poisoning.mode_changed = 1;
	}
	if(anti_cathode_poisoning.timer_mode == ACTIVE_MODE){

		if((anti_cathode_poisoning.cycle == (anti_cathode_poisoning.max_cycles - 1))
				&& (anti_cathode_poisoning.counter == anti_cathode_poisoning.max_counter)){ //final cycle and final count

			__HAL_TIM_SET_AUTORELOAD(&htim16, ANTI_CATHODE_POISONING_TIMER_WAITING_MODE_PERIOD_MINUS_ONE);
			__HAL_TIM_SET_PRESCALER(&htim16, ANTI_CATHODE_POISONING_TIMER_WAITING_MODE_PRESCALER);

			anti_cathode_poisoning.timer_mode = WAITING_MODE;
			anti_cathode_poisoning.counter = 0;
			anti_cathode_poisoning.cycle = 0;
		}
		else{

			__HAL_TIM_SET_AUTORELOAD(&htim16, ANTI_CATHODE_POISONING_TIMER_ACTIVE_MODE_PERIOD_MINUS_ONE);
			__HAL_TIM_SET_PRESCALER(&htim16, ANTI_CATHODE_POISONING_TIMER_ACTIVE_MODE_PRESCALER);

			if((anti_cathode_poisoning.counter == anti_cathode_poisoning.max_counter)
					&& (anti_cathode_poisoning.cycle != (anti_cathode_poisoning.max_cycles - 1))){

				anti_cathode_poisoning.counter = 0;
				anti_cathode_poisoning.cycle++;
			}
			else{

				anti_cathode_poisoning.counter++;
			}
		}
	}
}


