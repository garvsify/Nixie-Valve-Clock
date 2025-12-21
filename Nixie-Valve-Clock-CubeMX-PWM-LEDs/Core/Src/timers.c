/*
 * timers.c
 *
 *  Created on: 2 Dec 2025
 *      Author: James Garvey
 */

#include "system.h"

uint8_t Start_OC_TIM(TIM_HandleTypeDef *TIM, uint32_t OC_TIM_channel){

	uint8_t ok = HAL_TIM_OC_Start_IT(TIM, OC_TIM_channel);

	if(ok != HAL_OK){

		Error_Handler();
	}

	return ok;
}

uint8_t Stop_OC_TIM(TIM_HandleTypeDef *TIM, uint32_t OC_TIM_channel){

	uint8_t ok = HAL_TIM_OC_Stop_IT(TIM, OC_TIM_channel);

	if(ok != HAL_OK){

		Error_Handler();
	}

	return ok;
}


