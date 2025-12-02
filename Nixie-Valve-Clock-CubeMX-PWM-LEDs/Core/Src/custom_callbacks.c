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

	Write_Digit_to_Valve(valve, BCD);

	valve++;

	if(valve == NUM_VALVES){

		valve = 0;
	}
}


