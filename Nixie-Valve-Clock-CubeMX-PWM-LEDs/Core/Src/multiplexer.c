#include "multiplexer.h"

GPIO_TypeDef* Valve_Anode_Registers[NUM_VALVES] = {GPIO_Output_AS_0_GPIO_Port, GPIO_Output_AS_1_GPIO_Port, GPIO_Output_AS_2_GPIO_Port, GPIO_Output_AS_3_GPIO_Port, GPIO_Output_AS_4_GPIO_Port, GPIO_Output_AS_5_GPIO_Port};
uint16_t Valve_Anode_Pins[NUM_VALVES] = {GPIO_Output_AS_0_Pin, GPIO_Output_AS_1_Pin, GPIO_Output_AS_2_Pin, GPIO_Output_AS_3_Pin, GPIO_Output_AS_4_Pin, GPIO_Output_AS_5_Pin};

GPIO_TypeDef* BCD_Registers[NUM_BINARY_DIGITS_IN_BCD] = {GPIO_Output_BCD0_GPIO_Port, GPIO_Output_BCD1_GPIO_Port, GPIO_Output_BCD2_GPIO_Port, GPIO_Output_BCD3_GPIO_Port};
uint16_t BCD_Pins[NUM_BINARY_DIGITS_IN_BCD] = {GPIO_Output_BCD0_Pin, GPIO_Output_BCD1_Pin, GPIO_Output_BCD2_Pin, GPIO_Output_BCD3_Pin};

uint8_t __RAM_FUNC Write_Digit_to_Valve(uint8_t valve_num, uint8_t BCD_of_digit){

	//turn off all anodes of valves except of valve of interest
	for(uint8_t valve = 0; valve < NUM_VALVES; valve++){

		if(valve != valve_num){

			HAL_GPIO_WritePin(Valve_Anode_Registers[valve], Valve_Anode_Pins[valve], VALVE_ANODE_OFF_STATE);
		}
		else{

			HAL_GPIO_WritePin(Valve_Anode_Registers[valve], Valve_Anode_Pins[valve], VALVE_ANODE_ON_STATE);
		}
	}

	//write BCD of digit to GPIOs that drive the BCD chip
	for(uint8_t i = 0; i < NUM_BINARY_DIGITS_IN_BCD; i++){

		HAL_GPIO_WritePin(BCD_Registers[i], BCD_Pins[i], ((BCD_of_digit >> i) & 0b1));
	}

	return 1;
}

uint8_t __RAM_FUNC Turn_Valve_Off(uint8_t valve){

	HAL_GPIO_WritePin(Valve_Anode_Registers[valve], Valve_Anode_Pins[valve], VALVE_ANODE_OFF_STATE);

	return 1;
}

uint8_t Start_Multiplexer_Timer(void){

	__HAL_TIM_ENABLE_IT(&htim17, TIM_IT_UPDATE); //make sure overflow (update) interrupt is enabled for TIM17

	uint8_t ok = Start_OC_TIM(&htim17, TIM_CHANNEL_1);

	if(ok != HAL_OK){

		Error_Handler();
	}

	return ok;
}

uint8_t Start_Anti_Cathode_Poisoning_Timer(void){

	__HAL_TIM_ENABLE_IT(&htim16, TIM_IT_UPDATE); //make sure overflow (update) interrupt is enabled for TIM16

	uint8_t ok = Start_OC_TIM(&htim16, TIM_CHANNEL_1);

	if(ok != HAL_OK){

		Error_Handler();
	}

	return ok;
}

uint8_t Start_Adjust_Mode_Timer(void){

	__HAL_TIM_ENABLE_IT(&htim14, TIM_IT_UPDATE); //make sure overflow (update) interrupt is enabled for TIM14

	uint8_t ok = Start_OC_TIM(&htim14, TIM_CHANNEL_1);

	if(ok != HAL_OK){

		Error_Handler();
	}

	return ok;
}

uint8_t Stop_Adjust_Mode_Timer(void){

	uint8_t ok = Stop_OC_TIM(&htim14, TIM_CHANNEL_1);

	if(ok != HAL_OK){

		Error_Handler();
	}

	return ok;
}

uint8_t Master_Init(struct Master *master){

	master->anti_cathode_poisoning.counter = 0;
	master->anti_cathode_poisoning.cycle = 0;
	master->anti_cathode_poisoning.max_counter = ANTI_CATHODE_POISONING_MAX_COUNTER;
	master->anti_cathode_poisoning.max_cycles = ANTI_CATHODE_POISONING_MAX_CYCLES;

	master->system_mode_tracker.current_mode = NORMAL_MODE;
	master->system_mode_tracker.previous_mode = NONE;

	master->time_adjust.blink_state = BLINK_OFF;

	master->separators.counter = 0;

	master->encoder_first = 0;
	master->encoder_second = 0;

	return 1;
}

uint8_t __RAM_FUNC Set_System_Mode_and_Store_Previous_Mode(struct System_Mode_Tracker *system_mode_tracker, enum System_Mode desired_mode){

	system_mode_tracker->previous_mode = system_mode_tracker->current_mode;
	system_mode_tracker->current_mode = desired_mode;

	return 1;
}

uint8_t Toggle_HV_Power_Supply(uint8_t toggle){

	if(toggle){

		HAL_GPIO_WritePin(GPIO_Output__HV_SHDN_GPIO_Port, GPIO_Output__HV_SHDN_Pin, 1);
	}
	else if(!toggle){

		HAL_GPIO_WritePin(GPIO_Output__HV_SHDN_GPIO_Port, GPIO_Output__HV_SHDN_Pin, 0);
	}

	return 1;
}

uint8_t __RAM_FUNC Get_RTC_Time(void){

	HAL_RTC_GetTime(&hrtc, (RTC_TimeTypeDef*)&master.get_time, RTC_FORMAT_BCD);
	HAL_RTC_GetDate(&hrtc, (RTC_DateTypeDef*)&master.get_date, RTC_FORMAT_BCD);

	return 1;
}

uint8_t __RAM_FUNC Write_Time_In_Flash(RTC_TimeTypeDef *time){

	uint64_t packed;
	uint32_t errors = 0;

	FLASH_EraseInitTypeDef erase_config = {.TypeErase = FLASH_CR_PER, .Banks = FLASH_CR_MER1, .Page = 31, .NbPages = 1};

	HAL_FLASH_Unlock();
	HAL_FLASHEx_Erase(&erase_config, &errors);

	Pack_Time_Into_Doubleword(time, &packed);

	HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, TIME_FLASH_BASE_ADDRESS, packed);

	HAL_FLASH_Lock();

	return 1;
}

uint8_t Read_Time_From_Flash(RTC_TimeTypeDef *time){

	uint32_t address_val = TIME_FLASH_BASE_ADDRESS;
	uint8_t* address = (uint8_t *)address_val;

	if(*address == 0xFF){

		time->Seconds = 0x00;
	}
	else{

		time->Seconds = *address;
	}
	if(*(address + 1) == 0xFF){

		time->Minutes = 0x00;
	}
	else{

		time->Minutes = *(address + 1);
	}
	if(*(address + 2) == 0xFF){

		time->Hours = 0x00;
	}
	else{

		time->Hours = *(address + 2);
	}

	return 1;
}

uint8_t __RAM_FUNC Pack_Time_Into_Doubleword(RTC_TimeTypeDef *time, uint64_t *doubleword){

	uint64_t packed = 0;

	packed |= (time->Hours << 16);
	packed |= (time->Minutes << 8);
	packed |= (time->Seconds);

	*doubleword = packed;

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
