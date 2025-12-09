/*
 * custom_callbacks.c
 *
 *  Created on: 2 Dec 2025
 *      Author: James Garvey
 */

#include "custom_callbacks.h"
#include "main.h"

void TIM17_Multiplexer_Sequencer_Callback(TIM_HandleTypeDef *htim){

	static uint8_t valve = 0;
	uint8_t BCD = 0;
	uint8_t valve_on = 1;

	if(master.system_mode_tracker.current_mode == NORMAL_MODE){

		Get_RTC_Time();

		valve_on = 1;

		if(valve == 0){

			BCD = (master.get_time.Hours >> 4) & 0xF;
		}
		else if(valve == 1){

			BCD = master.get_time.Hours & 0xF;
		}
		else if(valve == 2){

			BCD = (master.get_time.Minutes >> 4) & 0xF;
		}
		else if(valve == 3){

			BCD = master.get_time.Minutes & 0xF;
		}
		else if(valve == 4){

			BCD = (master.get_time.Seconds >> 4) & 0xF;
		}
		else if(valve == 5){

			BCD = master.get_time.Seconds & 0xF;
		}
	}
	else if(master.system_mode_tracker.current_mode == ANTI_CATHODE_POISONING_MODE){

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

			BCD = (master.time_adjust.adjust_time.Hours >> 4) & 0xF;
		}
		else if(valve == 1){

			if(master.time_adjust.blink_state == 1){

				valve_on = 1;
			}
			else if(master.time_adjust.blink_state == 0){

				valve_on = 0;
			}

			BCD = master.time_adjust.adjust_time.Hours & 0xF;
		}
		else if(valve == 2){

			valve_on = 1;
			BCD = (master.time_adjust.adjust_time.Minutes >> 4) & 0xF;
		}
		else if(valve == 3){

			valve_on = 1;
			BCD = master.time_adjust.adjust_time.Minutes & 0xF;
		}
		else if(valve == 4){

			valve_on = 1;
			BCD = (master.time_adjust.adjust_time.Seconds >> 4) & 0xF;
		}
		else if(valve == 5){

			valve_on = 1;
			BCD = master.time_adjust.adjust_time.Seconds & 0xF;
		}

	}
	else if(master.system_mode_tracker.current_mode == MM_ADJUST_MODE){

		if(valve == 0){

			valve_on = 1;
			BCD = (master.time_adjust.adjust_time.Hours >> 4) & 0xF;
		}
		else if(valve == 1){

			valve_on = 1;
			BCD = master.time_adjust.adjust_time.Hours & 0xF;
		}
		else if(valve == 2){

			if(master.time_adjust.blink_state == 1){

				valve_on = 1;
			}
			else if(master.time_adjust.blink_state == 0){

				valve_on = 0;
			}

			BCD = (master.time_adjust.adjust_time.Minutes >> 4) & 0xF;
		}
		else if(valve == 3){

			if(master.time_adjust.blink_state == 1){

				valve_on = 1;
			}
			else if(master.time_adjust.blink_state == 0){

				valve_on = 0;
			}

			BCD = master.time_adjust.adjust_time.Minutes & 0xF;
		}
		else if(valve == 4){

			valve_on = 1;
			BCD = (master.time_adjust.adjust_time.Seconds >> 4) & 0xF;
		}
		else if(valve == 5){

			valve_on = 1;
			BCD = master.time_adjust.adjust_time.Seconds & 0xF;
		}
	}
	else if(master.system_mode_tracker.current_mode == SS_ADJUST_MODE){

		if(valve == 0){

			valve_on = 1;
			BCD = (master.time_adjust.adjust_time.Hours >> 4) & 0xF;
		}
		else if(valve == 1){

			valve_on = 1;
			BCD = master.time_adjust.adjust_time.Hours & 0xF;
		}
		else if(valve == 2){

			valve_on = 1;
			BCD = (master.time_adjust.adjust_time.Minutes >> 4) & 0xF;
		}
		else if(valve == 3){

			valve_on = 1;
			BCD = master.time_adjust.adjust_time.Minutes & 0xF;
		}
		else if(valve == 4){

			if(master.time_adjust.blink_state == 1){

				valve_on = 1;
			}
			else if(master.time_adjust.blink_state == 0){

				valve_on = 0;
			}

			BCD = (master.time_adjust.adjust_time.Seconds >> 4) & 0xF;
		}
		else if(valve == 5){

			if(master.time_adjust.blink_state == 1){

				valve_on = 1;
			}
			else if(master.time_adjust.blink_state == 0){

				valve_on = 0;
			}

			BCD = master.time_adjust.adjust_time.Seconds & 0xF;
		}
	}

	if(valve_on){

		Write_Digit_to_Valve(valve, BCD);
	}
	else if(!valve_on){

		Turn_Valve_Off(valve);
	}

	valve++;

	if(valve == NUM_VALVES){

		valve = 0;
	}
}

void TIM16_Anti_Cathode_Poisoning_Callback(TIM_HandleTypeDef *htim){

	static uint8_t timer_length = 0;
	static uint8_t master_counter = 0;

	if(master.system_mode_tracker.current_mode != ANTI_CATHODE_POISONING_MODE){

		Set_System_Mode_and_Store_Previous_Mode(&master.system_mode_tracker, ANTI_CATHODE_POISONING_MODE);
	}

	if(timer_length == 0){

		//HAL_GPIO_TogglePin(MONITOR_1_Port, MONITOR_1_Pin);

		__HAL_TIM_SET_PRESCALER(&htim16, 250);
		__HAL_TIM_SET_AUTORELOAD(&htim16, 65535);
		TIM16->EGR |= TIM_EGR_UG;
		TIM16->SR &= ~TIM_SR_UIF;

		timer_length = 1;
		master.anti_cathode_poisoning.counter = 2;
	}
	else if(timer_length == 1){

		//HAL_GPIO_TogglePin(MONITOR_1_Port, MONITOR_1_Pin);

		__HAL_TIM_SET_PRESCALER(&htim16, 1000);
		__HAL_TIM_SET_AUTORELOAD(&htim16, 65535);
		TIM16->EGR |= TIM_EGR_UG;
		TIM16->SR &= ~TIM_SR_UIF;

		timer_length = 0;
		master.anti_cathode_poisoning.counter = 0;
	}

	if(master_counter != 10){

		master_counter++;
	}
	else{

		Set_System_Mode_and_Store_Previous_Mode(&master.system_mode_tracker, master.system_mode_tracker.previous_mode);
	}
}


