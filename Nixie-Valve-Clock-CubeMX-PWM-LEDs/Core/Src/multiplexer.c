#include "multiplexer.h"

GPIO_TypeDef* Valve_Anode_Registers[NUM_VALVES] = {GPIO_Output_AS_0_GPIO_Port, GPIO_Output_AS_1_GPIO_Port, GPIO_Output_AS_2_GPIO_Port, GPIO_Output_AS_3_GPIO_Port, GPIO_Output_AS_4_GPIO_Port, GPIO_Output_AS_5_GPIO_Port};
uint16_t Valve_Anode_Pins[NUM_VALVES] = {GPIO_Output_AS_0_Pin, GPIO_Output_AS_1_Pin, GPIO_Output_AS_2_Pin, GPIO_Output_AS_3_Pin, GPIO_Output_AS_4_Pin, GPIO_Output_AS_5_Pin};

GPIO_TypeDef* BCD_Registers[NUM_BINARY_DIGITS_IN_BCD] = {GPIO_Output_BCD0_GPIO_Port, GPIO_Output_BCD1_GPIO_Port, GPIO_Output_BCD2_GPIO_Port, GPIO_Output_BCD3_GPIO_Port};
uint16_t BCD_Pins[NUM_BINARY_DIGITS_IN_BCD] = {GPIO_Output_BCD0_Pin, GPIO_Output_BCD1_Pin, GPIO_Output_BCD2_Pin, GPIO_Output_BCD3_Pin};

uint8_t Write_Digit_to_Valve(uint8_t valve_num, uint8_t BCD_of_digit){

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

uint8_t Turn_Valve_Off(uint8_t valve){

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

uint8_t Master_Init(struct Master *master){

	master->anti_cathode_poisoning.counter = 0;
	master->anti_cathode_poisoning.cycle = 0;
	master->anti_cathode_poisoning.max_counter = ANTI_CATHODE_POISONING_MAX_COUNTER;
	master->anti_cathode_poisoning.max_cycles = ANTI_CATHODE_POISONING_MAX_CYCLES;

	master->system_mode_tracker.current_mode = NORMAL_MODE;
	master->system_mode_tracker.previous_mode = NONE;

	master->time_adjust.blink_state = BLINK_OFF;

	return 1;
}

uint8_t Set_System_Mode_and_Store_Previous_Mode(struct System_Mode_Tracker *system_mode_tracker, enum System_Mode desired_mode){

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

uint8_t Get_RTC_Time(void){

	HAL_RTC_GetTime(&hrtc, (RTC_TimeTypeDef*)&master.get_time, RTC_FORMAT_BCD);
	HAL_RTC_GetDate(&hrtc, (RTC_DateTypeDef*)&master.get_date, RTC_FORMAT_BCD);

	return 1;
}
