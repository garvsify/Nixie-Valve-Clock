#include "system.h"

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

uint8_t Turn_All_Valves_Off(void){

	for(uint8_t valve = 0; valve < NUM_VALVES; valve++){

		HAL_GPIO_WritePin(Valve_Anode_Registers[valve], Valve_Anode_Pins[valve], VALVE_ANODE_OFF_STATE);
	}
	return 1;
}

uint8_t Start_Adjust_Time_Slash_Alarm_Set_Mode_Timer(void){

	__HAL_TIM_ENABLE_IT(&htim14, TIM_IT_UPDATE); //make sure overflow (update) interrupt is enabled for TIM14

	uint8_t ok = Start_OC_TIM(&htim14, TIM_CHANNEL_1);

	if(ok != HAL_OK){

		Error_Handler();
	}

	return ok;
}

uint8_t Stop_Adjust_Time_Slash_Alarm_Set_Mode_Timer(void){

	uint8_t ok = Stop_OC_TIM(&htim14, TIM_CHANNEL_1);

	if(ok != HAL_OK){

		Error_Handler();
	}

	return ok;
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

