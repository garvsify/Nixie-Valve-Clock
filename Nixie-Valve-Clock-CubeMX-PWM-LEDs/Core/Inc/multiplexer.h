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
#define MULTIPLEXER_TIMER_PERIOD_MINUS_ONE 40000 //with 0 prescaler and clkdivby4 should give 4.096ms overflow; with 2 prescaler and clkdivby4 should give 12.288ms overflow
#define MULTIPLEXER_TIMER_PRESCALER 3 //with 0 prescaler and clkdivby4 should give 4.096ms overflow; with 2 prescaler and clkdivby4 should give 12.288ms overflow
#define DEAD_TIME 2500 //max value is realistically 3000, otherwise time measurements on rotary encoder switch are too long
#define DEAD_TIME_MAX 2500

#define MULTIPLEXER_TIMER_INTERRUPT_PRIORITY 0 //has to be zero otherwise dead time fucks shit up, flickering starts occurring
#define ROTARY_ENCODER_SWITCH_INTERRUPT_PRIORITY 0// if not zero, adjusting dead time tends to extend the depression time for the rotary encoder switch
#define ANTI_CATHODE_POISONING_TIMER_INTERRUPT_PRIORITY 2
#define VALVE_BLINK_TIMER_INTERRUPT_PRIORITY 2
#define UART_INTERRUPT_PRIORITY 2

#define ANTI_CATHODE_POISONING_TIMER_WAITING_MODE_PRESCALER 65535  //with clkdivby4 should give ~4.5min overflow
#define ANTI_CATHODE_POISONING_TIMER_WAITING_MODE_PERIOD_MINUS_ONE 65535 //5000(test) //with clkdivby4 should give ~4.5min overflow
#define ANTI_CATHODE_POISONING_TIMER_ACTIVE_MODE_PRESCALER 25 //with clkdivby4 should give 204.8ms overflow //was 49
#define ANTI_CATHODE_POISONING_TIMER_ACTIVE_MODE_PERIOD_MINUS_ONE 65535 //with clkdivby4 should give 204.8ms overflow
#define ANTI_CATHODE_POISONING_MAX_COUNTER 9
#define ANTI_CATHODE_POISONING_MAX_CYCLES 3
#define TIME_ADJUST_BLINK_PERIOD_MINUS_ONE 65535
#define TIME_ADJUST_BLINK_PRESCALER 375
#define TIME_FLASH_BASE_ADDRESS 0x0800F800
#define LPTIM1_CCR_CHECK 250
#define ROTARY_ENCODER_SWITCH_CONFIDENCE_COUNT 14
#define COUNT_TO_DELAY_RISING_ROTARY_ENCODER_EDGE 4

#define ROTARY_ENCODER_MS_DELAY_COUNT_BETWEEN_CHECKS 10
#define NUM_ROTARY_ENCODER_LEDs 2
#define ROTARY_ENCODER_SWITCH_ENTER_SLASH_ADVANCE_TIME_ADJUST_MODE_COUNT_MIN 2000
#define ROTARY_ENCODER_SWITCH_ENTER_SLASH_ADVANCE_TIME_ADJUST_MODE_COUNT_MAX ROTARY_ENCODER_SWITCH_ENTER_SLASH_ADVANCE_TIME_ADJUST_MODE_COUNT_MIN + 2000
#define ROTARY_ENCODER_SWITCH_SAVE_TIME_COUNT_MIN 5500
#define ROTARY_ENCODER_SWITCH_SAVE_TIME_COUNT_MAX ROTARY_ENCODER_SWITCH_SAVE_TIME_COUNT_MIN + 2000
#define SET_ALARM_ADVANCE_COUNT_MIN ROTARY_ENCODER_SWITCH_ENTER_SLASH_ADVANCE_TIME_ADJUST_MODE_COUNT_MIN
#define SET_ALARM_ADVANCE_COUNT_MAX ROTARY_ENCODER_SWITCH_ENTER_SLASH_ADVANCE_TIME_ADJUST_MODE_COUNT_MAX
#define SET_ALARM_COUNT_MIN ROTARY_ENCODER_SWITCH_SAVE_TIME_COUNT_MIN
#define SET_ALARM_COUNT_MAX ROTARY_ENCODER_SWITCH_SAVE_TIME_COUNT_MAX
#define SET_ALARM_SAVE_COUNT_MIN ROTARY_ENCODER_SWITCH_SAVE_TIME_COUNT_MIN
#define SET_ALARM_SAVE_COUNT_MAX ROTARY_ENCODER_SWITCH_SAVE_TIME_COUNT_MAX
#define CLEAR_ALARM_COUNT_MIN 50
#define CLEAR_ALARM_COUNT_MAX 500
#define CANCEL_ALARM_COUNT_MIN ROTARY_ENCODER_SWITCH_SAVE_TIME_COUNT_MAX + 1000
#define CANCEL_ALARM_COUNT_MAX CANCEL_ALARM_COUNT_MIN + 2500
#define BUZZER_SHORT_TONE_COUNT 20000
#define BUZZER_SHORT_OFF_COUNT 20000
#define BUZZER_LONG_OFF_COUNT 40000
#define BUZZER_COUNT_MAX BUZZER_SHORT_TONE_COUNT + BUZZER_SHORT_TONE_COUNT + BUZZER_SHORT_OFF_COUNT + BUZZER_LONG_OFF_COUNT
#define ADJUST_TIME_GREEN_LED_BRIGHTNESS_CCR 10000
#define ALARM_SET_GREEN_LED_BRIGHTNESS_CCR 5000
#define ALARM_SET_RED_LED_BRIGHTNESS_CCR 20000
#define FAULT_RED_LED_BRIGHTNESS_CCR 65535
#define ROTARY_ENCODER_RED_LED_NUM 1
#define ROTARY_ENCODER_GREEN_LED_NUM 0
#define LED_SHORT_ON_COUNT 16
#define LED_SHORT_OFF_COUNT 32
#define LED_DOUBLE_FLASH_COUNT_MAX (LED_SHORT_ON_COUNT << 1) + (LED_SHORT_OFF_COUNT << 1)
#define RED_LED_DOUBLE_FLASH_BRIGHTNESS_CCR 30000

#define ADJUST_TIME_CAL_DECREASE_FREQUENCY 0
#define ADJUST_TIME_CAL_INCREASE_FREQUENCY 1
#define ADJUST_TIME_CAL_PPM 200

#define CHANGE_DEAD_TIME_COMMAND 0x00
#define CHANGE_PPM_COMMAND 0x01
#define PPM_MAX 475
#define CHANGE_PPM_ADJUST_POLARITY 0x02
#define COMMAND_PLUS_ARGUMENT_NUM_BYTES 2
#define COMMAND_INDEX 0
#define VALUE_INDEX 1
#define RESPONSE_NUM_BYTES 1
#define RESPONSE_INDEX 0

#define TRI_WAVETABLE_SIZE 256

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

	volatile RTC_TimeTypeDef adjust_time;
	volatile uint8_t Hours_Bin;
	volatile uint8_t Minutes_Bin;
	volatile uint8_t Seconds_Bin;
};

struct Alarm{

	volatile uint8_t alarm_triggered;
	volatile uint8_t alarm_set;
	volatile uint32_t alarm_counter;
	volatile RTC_TimeTypeDef alarm_time;
	volatile uint8_t Hours_Bin;
	volatile uint8_t Minutes_Bin;
	volatile uint8_t Seconds_Bin;
};

enum System_Mode{

	NONE,
	NORMAL_MODE,
	ANTI_CATHODE_POISONING_MODE,
	HH_ADJUST_MODE,
	MM_ADJUST_MODE,
	SS_ADJUST_MODE,
	VALVES_OFF_MODE,
	ALARM_SET_HH_MODE,
	ALARM_SET_MM_MODE,
	ALARM_SET_SS_MODE,
};

struct System_Mode_Tracker{

	volatile enum System_Mode current_mode;
	volatile enum System_Mode previous_mode;
};

struct Anti_Cathode_Poisoning{

	volatile uint8_t counter;
	uint8_t max_counter;
	volatile uint8_t cycle;
	uint8_t max_cycles;
};

struct Software_Timer{

	uint32_t count;
	uint32_t max_count;
	uint8_t enabled;
};

struct LEDs{

	volatile uint32_t LED_counter[NUM_ROTARY_ENCODER_LEDs];
	volatile uint8_t Double_Flash_Red_LED;
};

enum Rotary_Encoder_Switch_State{

	ROTARY_ENCODER_SWITCH_STATE_NOT_DEPRESSED,
	ROTARY_ENCODER_SWITCH_STATE_DEPRESSED,
};

struct Rotary_Encoder_Switch_States{

	volatile enum Rotary_Encoder_Switch_State rotary_encoder_switch_state;
	volatile enum Rotary_Encoder_Switch_State rotary_encoder_switch_prev_state;
};

struct Master{

	struct Anti_Cathode_Poisoning anti_cathode_poisoning;

	struct System_Mode_Tracker system_mode_tracker;

	struct Time_Adjust time_adjust;
	volatile RTC_TimeTypeDef get_time;
	volatile RTC_DateTypeDef get_date;

	struct Alarm alarm;

	struct Software_Timer software_timers[1]; //not yet used

	uint32_t encoder_first;
	uint32_t encoder_second;
	struct Rotary_Encoder_Switch_States rotary_encoder_switch_states;

	volatile uint8_t valve_blink_state;

	struct LEDs leds;

	volatile uint32_t depressed_num_monitor;

	volatile uint8_t RX_buffer[COMMAND_PLUS_ARGUMENT_NUM_BYTES];
	volatile uint8_t TX_buffer[RESPONSE_NUM_BYTES];

	volatile uint16_t dead_time;

	volatile uint8_t adjust_ppm_polarity;
	volatile uint16_t adjust_ppm;
};

extern GPIO_TypeDef* Valve_Anode_Registers[NUM_VALVES];
extern uint16_t Valve_Anode_Pins[NUM_VALVES];

extern GPIO_TypeDef* BCD_Registers[NUM_BINARY_DIGITS_IN_BCD];
extern uint16_t BCD_Pins[NUM_BINARY_DIGITS_IN_BCD];

extern const uint16_t triangle_wavetable[TRI_WAVETABLE_SIZE];
extern struct Master master;

uint8_t Write_Digit_to_Valve(uint8_t valve_num, uint8_t BCD_of_digit);
uint8_t Turn_All_Valves_Off(void);
uint8_t Start_Multiplexer_Timer(void);
uint8_t Start_Anti_Cathode_Poisoning_Timer(void);
uint8_t Start_Adjust_Time_Slash_Alarm_Set_Mode_Timer(void);
uint8_t Stop_Adjust_Time_Slash_Alarm_Set_Mode_Timer(void);
uint8_t Master_Init(struct Master *master);
uint8_t Set_System_Mode_and_Store_Previous_Mode(struct System_Mode_Tracker *system_mode_tracker, enum System_Mode desired_mode);
uint8_t Turn_Valve_Off(uint8_t valve_num);
uint8_t Toggle_HV_Power_Supply(uint8_t toggle);
uint8_t Get_RTC_Time(void);
uint8_t Write_Time_In_Flash(RTC_TimeTypeDef *time);
uint8_t Read_Time_From_Flash(RTC_TimeTypeDef *time);
uint8_t Pack_Time_Into_Doubleword(RTC_TimeTypeDef *time, uint64_t *doubleword);
uint8_t Check_Rotary_Encoder_Switch_State(volatile struct Rotary_Encoder_Switch_States *rotary_encoder_switch_states_ptr);
uint8_t Set_Fault_LED_ON(void);
uint8_t Set_Alarm(uint8_t BCD_HH, uint8_t BCD_MM, uint8_t BCD_SS);
uint8_t Clear_Alarm(void);
uint8_t Set_Adjust_Time_LED_ON(void);
uint8_t Set_Alarm_Set_LEDs_ON(void);
uint8_t Set_Adjust_Time_LED_OFF(void);
uint8_t Set_Alarm_Set_LEDs_OFF(void);
uint8_t Initialise_Rotary_Encoder_LEDs(void);
uint8_t Sound_Alarm(void);
uint8_t Initialise_Valve_LEDs(void);
uint8_t Double_Flash_Red_Rotary_Encoder_LED(void);
uint8_t Check_Rotary_Encoder(void);

#endif /* INC_MULTIPLEXER_H_ */
