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

uint8_t Anti_Cathode_Poisoning_Struct_Init(struct Anti_Cathode_Poisoning *anti_cathode_poisoning){

	anti_cathode_poisoning->counter = 0;
	anti_cathode_poisoning->cycle = 0;
	anti_cathode_poisoning->max_counter = ANTI_CATHODE_POISONING_MAX_COUNTER;
	anti_cathode_poisoning->max_cycles = ANTI_CATHODE_POISONING_MAX_CYCLES;
	anti_cathode_poisoning->timer_mode = WAITING_MODE;
	anti_cathode_poisoning->mode_changed = 0;

	return 1;
}
