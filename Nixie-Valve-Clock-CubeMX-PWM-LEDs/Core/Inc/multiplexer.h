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
#define MULTIPLEXER_TIMER_PERIOD_MINUS_ONE 65535 //with clkdivby4 should give 4.096ms overflow
#define MULTIPLEXER_TIMER_PRESCALER 0 //with clkdivby4 should give 4.096ms overflow
#define ANTI_CATHODE_POISONING_TIMER_WAITING_MODE_PRESCALER 65535 //with clkdivby4 should give ~4.5min overflow
#define ANTI_CATHODE_POISONING_TIMER_WAITING_MODE_PERIOD_MINUS_ONE 65535 //with clkdivby4 should give ~4.5min overflow
#define ANTI_CATHODE_POISONING_TIMER_ACTIVE_MODE_PRESCALER 49 //with clkdivby4 should give 204.8ms overflow
#define ANTI_CATHODE_POISONING_TIMER_ACTIVE_MODE_PERIOD_MINUS_ONE 65535 //with clkdivby4 should give 204.8ms overflow
#define ANTI_CATHODE_POISONING_MAX_COUNTER 9
#define ANTI_CATHODE_POISONING_MAX_CYCLES 3

#include <stdint.h>
#include "stm32g031xx.h"
#include "stm32g0xx_hal.h"
#include "timers.h"
#include "main.h"

enum Anti_Cathode_Poisoning_Timer_Mode{

	WAITING_MODE,
	ACTIVE_MODE
};

struct Anti_Cathode_Poisoning{

	enum Anti_Cathode_Poisoning_Timer_Mode timer_mode;
	uint8_t counter;
	uint8_t max_counter;
	uint8_t cycle;
	uint8_t max_cycles;

	uint8_t mode_changed;
};

extern struct Anti_Cathode_Poisoning anti_cathode_poisoning;

extern GPIO_TypeDef* Valve_Anode_Registers[NUM_VALVES];
extern uint16_t Valve_Anode_Pins[NUM_VALVES];

extern GPIO_TypeDef* BCD_Registers[NUM_BINARY_DIGITS_IN_BCD];
extern uint16_t BCD_Pins[NUM_BINARY_DIGITS_IN_BCD];

uint8_t Write_Digit_to_Valve(uint8_t valve_num, uint8_t BCD_of_digit);
uint8_t Start_Multiplexer_Timer(void);
uint8_t Start_Anti_Cathode_Poisoning_Timer(void);
uint8_t Anti_Cathode_Poisoning_Struct_Init(struct Anti_Cathode_Poisoning *anti_cathode_poisoning);

#endif /* INC_MULTIPLEXER_H_ */
