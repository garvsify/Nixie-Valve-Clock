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

	if(master.system_mode_tracker.current_mode == VALVES_OFF_MODE){

		valve_on = 0;
	}
	else if(master.system_mode_tracker.current_mode == NORMAL_MODE){

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

		if(master.time_adjust.blink_state == BLINK_OFF){
			Turn_All_Valves_Off(); // Turn off all anodes when the blinking segment is in the "off" state
			valve_on = 0; // Ensure no valve is turned on in the subsequent logic
		}
		else { // BLINK_ON state
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
				valve_on = 1;
				BCD = (master.time_adjust.adjust_time.Seconds >> 4) & 0xF;
			}
			else if(valve == 5){
				valve_on = 1;
				BCD = master.time_adjust.adjust_time.Seconds & 0xF;
			}
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

		//Write_Time_In_Flash((RTC_TimeTypeDef*)&master.get_time);

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

void LPTIM1_Rotary_Encoder_Switch_Callback(LPTIM_HandleTypeDef *hlptim){

	Check_Rotary_Encoder_Switch_State(&master.rotary_encoder_switch_states);

	static uint64_t depressed_num = 0;
	static uint8_t time_adjust_mode_is_active = 0;

	if(master.rotary_encoder_switch_states.rotary_encoder_switch_state == ROTARY_ENCODER_SWITCH_STATE_NOT_DEPRESSED){

		if(time_adjust_mode_is_active == 0){

			//enter time adjust mode
			if(depressed_num >= ROTARY_ENCODER_SWITCH_ENTER_SLASH_ADVANCE_TIME_ADJUST_MODE_COUNT_MIN
					&& depressed_num < ROTARY_ENCODER_SWITCH_ENTER_SLASH_ADVANCE_TIME_ADJUST_MODE_COUNT_MAX){

				time_adjust_mode_is_active = 1;
				Set_System_Mode_and_Store_Previous_Mode(&master.system_mode_tracker, HH_ADJUST_MODE);
				//Start timer to blink HH valves
				Start_Adjust_Mode_Timer();
				master.time_adjust.adjust_time = master.get_time;
				master.time_adjust.Hours_Bin = RTC_Bcd2ToByte(master.time_adjust.adjust_time.Hours);
				master.time_adjust.Minutes_Bin = RTC_Bcd2ToByte(master.time_adjust.adjust_time.Minutes);
				master.time_adjust.Seconds_Bin = RTC_Bcd2ToByte(master.time_adjust.adjust_time.Seconds);
			}
		}
		else if(time_adjust_mode_is_active == 1){

			//advance through HH:MM:SS
			if(depressed_num >= ROTARY_ENCODER_SWITCH_ENTER_SLASH_ADVANCE_TIME_ADJUST_MODE_COUNT_MIN
								&& depressed_num < ROTARY_ENCODER_SWITCH_ENTER_SLASH_ADVANCE_TIME_ADJUST_MODE_COUNT_MAX){

				if(master.system_mode_tracker.current_mode == HH_ADJUST_MODE){

					Set_System_Mode_and_Store_Previous_Mode(&master.system_mode_tracker, MM_ADJUST_MODE);
				}
				else if(master.system_mode_tracker.current_mode == MM_ADJUST_MODE){

					Set_System_Mode_and_Store_Previous_Mode(&master.system_mode_tracker, SS_ADJUST_MODE);
				}
				else if(master.system_mode_tracker.current_mode == SS_ADJUST_MODE){

					Set_System_Mode_and_Store_Previous_Mode(&master.system_mode_tracker, HH_ADJUST_MODE);
				}
			}
			else if(depressed_num >= ROTARY_ENCODER_SWITCH_SAVE_TIME_COUNT_MIN
					&& depressed_num < ROTARY_ENCODER_SWITCH_SAVE_TIME_COUNT_MAX){

				Set_System_Mode_and_Store_Previous_Mode(&master.system_mode_tracker, NORMAL_MODE);
				Stop_Adjust_Mode_Timer();
				time_adjust_mode_is_active = 0;

				HAL_RTC_SetTime(&hrtc, (RTC_TimeTypeDef*)&master.time_adjust.adjust_time, RTC_FORMAT_BCD);
			}
		}

		depressed_num = 0;
	}
	else if(master.rotary_encoder_switch_states.rotary_encoder_switch_state == ROTARY_ENCODER_SWITCH_STATE_DEPRESSED){

		depressed_num++;
	}

	HAL_LPTIM_SetOnce_Start_IT(&hlptim1, LPTIM1_CCR_CHECK, LPTIM1_CCR_CHECK);
}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin){

	//shutdown HV and turn on rotary encoder's red lED
	Toggle_HV_Power_Supply(0);
	Set_Fault_LED_ON();

}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	asm("NOP");
}

