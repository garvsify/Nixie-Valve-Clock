/*
 * multiplexer.h
 *
 *  Created on: 2 Dec 2025
 *      Author: James Garvey
 */

#ifndef INC_MULTIPLEXER_H_
#define INC_MULTIPLEXER_H_

#define NUM_VALVES 6
#define VALVE_ANODE_OFF_STATE 1
#define VALVE_ANODE_ON_STATE 0
#define NUM_BINARY_DIGITS_IN_BCD 4
#define MULTIPLEXER_TIMER_PERIOD 65536
#define MULTIPLEXER_TIMER_PRESCALER 0

#include <stdint.h>
#include "stm32g031xx.h"
#include "stm32g0xx_hal.h"
#include "timers.h"
#include "main.h"

extern GPIO_TypeDef* Valve_Anode_Registers[NUM_VALVES];
extern uint16_t Valve_Anode_Pins[NUM_VALVES];

extern GPIO_TypeDef* BCD_Registers[NUM_BINARY_DIGITS_IN_BCD];
extern uint16_t BCD_Pins[NUM_BINARY_DIGITS_IN_BCD];

uint8_t Write_Digit_to_Valve(uint8_t valve_num, uint8_t BCD_of_digit);
uint8_t Start_Multiplexer_Timer(void);

#endif /* INC_MULTIPLEXER_H_ */
