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

void __RAM_FUNC TIM17_Multiplexer_Sequencer_Callback(TIM_HandleTypeDef *htim);
void __RAM_FUNC TIM16_Anti_Cathode_Poisoning_Callback(TIM_HandleTypeDef *htim);
void __RAM_FUNC TIM14_Time_Adjust_Valve_Blink_Callback(TIM_HandleTypeDef *htim);
void TIM1_CH1_Valve_LED_0_Callback(TIM_HandleTypeDef *htim);
void TIM1_CH2_Valve_LED_1_Callback(TIM_HandleTypeDef *htim);
void TIM1_CH3_Valve_LED_2_Callback(TIM_HandleTypeDef *htim);
void LPTIM1_Rotary_Encoder_Switch_Callback(LPTIM_HandleTypeDef *hlptim);
#endif /* INC_CUSTOM_CALLBACKS_H_ */
