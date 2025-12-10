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


	if(master.system_mode_tracker.current_mode == NORMAL_MODE || (master.system_mode_tracker.current_mode == ANTI_CATHODE_POISONING_MODE && master.system_mode_tracker.previous_mode == NORMAL_MODE)){

		HAL_GPIO_WritePin(GPIO_Output_IN_3_0_GPIO_Port, GPIO_Output_IN_3_0_Pin, 1);
		HAL_GPIO_WritePin(GPIO_Output_IN_3_1_GPIO_Port, GPIO_Output_IN_3_1_Pin, 1);
	}
	else{

		HAL_GPIO_WritePin(GPIO_Output_IN_3_0_GPIO_Port, GPIO_Output_IN_3_0_Pin, 0);
		HAL_GPIO_WritePin(GPIO_Output_IN_3_1_GPIO_Port, GPIO_Output_IN_3_1_Pin, 0);
	}
}

void TIM16_Anti_Cathode_Poisoning_Callback(TIM_HandleTypeDef *htim){

	if(master.system_mode_tracker.current_mode != ANTI_CATHODE_POISONING_MODE){

		Set_System_Mode_and_Store_Previous_Mode(&master.system_mode_tracker, ANTI_CATHODE_POISONING_MODE);
	}

	else{

		master.anti_cathode_poisoning.counter++;
	}
	if((master.anti_cathode_poisoning.cycle == (master.anti_cathode_poisoning.max_cycles - 1))
			&& (master.anti_cathode_poisoning.counter == master.anti_cathode_poisoning.max_counter + 1)){ //final cycle and final count + 1

		__HAL_TIM_SET_AUTORELOAD(&htim16, ANTI_CATHODE_POISONING_TIMER_WAITING_MODE_PERIOD_MINUS_ONE);
		__HAL_TIM_SET_PRESCALER(&htim16, ANTI_CATHODE_POISONING_TIMER_WAITING_MODE_PRESCALER);
		TIM16->EGR |= TIM_EGR_UG; //force an update event to transfer the preloaded registers into the active registers
		TIM16->SR &= ~TIM_SR_UIF; //clear the update interrupt flag generated by forcing an update event

		Set_System_Mode_and_Store_Previous_Mode(&master.system_mode_tracker, master.system_mode_tracker.previous_mode); //anti cathode poisoning
		//mode over, return to previous mode

		master.anti_cathode_poisoning.counter = 0;
		master.anti_cathode_poisoning.cycle = 0;
	}
	else{

		__HAL_TIM_SET_AUTORELOAD(&htim16, ANTI_CATHODE_POISONING_TIMER_ACTIVE_MODE_PERIOD_MINUS_ONE);
		__HAL_TIM_SET_PRESCALER(&htim16, ANTI_CATHODE_POISONING_TIMER_ACTIVE_MODE_PRESCALER);
		TIM16->EGR |= TIM_EGR_UG; //force an update event to transfer the preloaded registers into the active registers
		TIM16->SR &= ~TIM_SR_UIF; //clear the update interrupt flag generated by forcing an update event

		if((master.anti_cathode_poisoning.counter == master.anti_cathode_poisoning.max_counter + 1)
				&& (master.anti_cathode_poisoning.cycle != (master.anti_cathode_poisoning.max_cycles - 1))){

			master.anti_cathode_poisoning.counter = 0;
			master.anti_cathode_poisoning.cycle++;
		}
	}
}

void TIM14_Time_Adjust_Valve_Blink_Callback(TIM_HandleTypeDef *htim){

	if(master.time_adjust.blink_state == BLINK_OFF){

		master.time_adjust.blink_state = BLINK_ON;
	}
	else{

		master.time_adjust.blink_state = BLINK_OFF;
	}
}

void TIM1_CH1_Valve_LED_0_Callback(TIM_HandleTypeDef *htim){

}

void TIM1_CH2_Valve_LED_1_Callback(TIM_HandleTypeDef *htim){

}

void TIM1_CH3_Valve_LED_2_Callback(TIM_HandleTypeDef *htim){

}

