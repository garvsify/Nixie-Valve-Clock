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
#define TIME_ADJUST_BLINK_PERIOD_MINUS_ONE 65535 //with clkdivby4 should give 500ms overflow
#define TIME_ADJUST_BLINK_PRESCALER 122 //with clkdivby4 should give 500ms overflow

#include <stdint.h>
#include "stm32g031xx.h"
#include "stm32g0xx_hal.h"
#include "timers.h"
#include "main.h"

enum Blink_State{

	BLINK_ON,
	BLINK_OFF
};

struct Time_Adjust{

	enum Blink_State blink_state;
	RTC_TimeTypeDef adjust_time;
};

enum System_Mode{

	NONE,
	NORMAL_MODE,
	ANTI_CATHODE_POISONING_MODE,
	HH_ADJUST_MODE,
	MM_ADJUST_MODE,
	SS_ADJUST_MODE,
};

struct System_Mode_Tracker{

	enum System_Mode current_mode;
	enum System_Mode previous_mode;
};

struct Anti_Cathode_Poisoning{

	uint8_t counter;
	uint8_t max_counter;
	uint8_t cycle;
	uint8_t max_cycles;

	uint8_t anti_cathode_poisoning_mode_enetered;
};

struct Master{

	struct Anti_Cathode_Poisoning anti_cathode_poisoning;
	struct System_Mode_Tracker system_mode_tracker;
	struct Time_Adjust time_adjust;
	RTC_TimeTypeDef get_time;
	RTC_DateTypeDef get_date;
};

extern GPIO_TypeDef* Valve_Anode_Registers[NUM_VALVES];
extern uint16_t Valve_Anode_Pins[NUM_VALVES];

extern GPIO_TypeDef* BCD_Registers[NUM_BINARY_DIGITS_IN_BCD];
extern uint16_t BCD_Pins[NUM_BINARY_DIGITS_IN_BCD];

extern struct Master master;

uint8_t Write_Digit_to_Valve(uint8_t valve_num, uint8_t BCD_of_digit);
uint8_t Start_Multiplexer_Timer(void);
uint8_t Start_Anti_Cathode_Poisoning_Timer(void);
uint8_t Master_Init(struct Master *master);
uint8_t Set_System_Mode_and_Store_Previous_Mode(struct System_Mode_Tracker *system_mode_tracker, enum System_Mode desired_mode);
uint8_t Turn_Valve_Off(uint8_t valve_num);
uint8_t Toggle_HV_Power_Supply(uint8_t toggle);
uint8_t Get_RTC_Time(void);

#endif /* INC_MULTIPLEXER_H_ */
