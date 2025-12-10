/*
 * custom_callbacks.h
 *
 *  Created on: 2 Dec 2025
 *      Author: James Garvey
 */

#ifndef INC_CUSTOM_CALLBACKS_H_
#define INC_CUSTOM_CALLBACKS_H_

#include "stm32g031xx.h"
#include "stm32g0xx_hal.h"

void TIM17_Multiplexer_Sequencer_Callback(TIM_HandleTypeDef *htim);
void TIM16_Anti_Cathode_Poisoning_Callback(TIM_HandleTypeDef *htim);
void TIM14_Time_Adjust_Valve_Blink_Callback(TIM_HandleTypeDef *htim);

#endif /* INC_CUSTOM_CALLBACKS_H_ */
