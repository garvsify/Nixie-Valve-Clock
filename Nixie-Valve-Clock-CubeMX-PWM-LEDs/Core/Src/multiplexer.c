#include "multiplexer.h"

GPIO_TypeDef* Valve_Anode_Registers[NUM_VALVES] = {GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA};
uint16_t Valve_Anode_Pins[NUM_VALVES] = {4, 5, 6, 7, 11, 12};

GPIO_TypeDef* BCD_Registers[NUM_BINARY_DIGITS_IN_BCD] = {GPIOB, GPIOB, GPIOB, GPIOB};
uint16_t BCD_Pins[NUM_BINARY_DIGITS_IN_BCD] = {8, 5, 6, 7};

struct Anti_Cathode_Poisoning anti_cathode_poisoning;

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
	master->anti_cathode_poisoning.anti_cathode_poisoning_mode_enetered = 0;

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
