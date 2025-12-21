/*
 * utility.c
 *
 *  Created on: 21 Dec 2025
 *      Author: James Garvey
 */

#include "system.h"

uint8_t Master_Init(struct Master *master){

	master->anti_cathode_poisoning.counter = 0;
	master->anti_cathode_poisoning.cycle = 0;
	master->anti_cathode_poisoning.max_counter = ANTI_CATHODE_POISONING_MAX_COUNTER;
	master->anti_cathode_poisoning.max_cycles = ANTI_CATHODE_POISONING_MAX_CYCLES;

	master->system_mode_tracker.current_mode = NORMAL_MODE;
	master->system_mode_tracker.previous_mode = NONE;

	master->valve_blink_state = BLINK_OFF;

	master->encoder_first = 0;
	master->encoder_second = 0;

	master->time_adjust.adjust_time.Hours = 0x00;
	master->time_adjust.adjust_time.Minutes = 0x00;
	master->time_adjust.adjust_time.Seconds = 0x00;
	master->time_adjust.adjust_time.TimeFormat = RTC_HOURFORMAT12_AM;
	master->time_adjust.adjust_time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
	master->time_adjust.adjust_time.StoreOperation = RTC_STOREOPERATION_RESET;

	master->alarm.alarm_time.Hours = 0x00;
	master->alarm.alarm_time.Minutes = 0x00;
	master->alarm.alarm_time.Seconds = 0x00;
	master->alarm.alarm_time.TimeFormat = RTC_HOURFORMAT12_AM;
	master->alarm.alarm_time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
	master->alarm.alarm_time.StoreOperation = RTC_STOREOPERATION_RESET;

	master->alarm.alarm_triggered = 0;
	master->alarm.alarm_counter = 0;

	master->RX_buffer[0] = 0x00;
	master->RX_buffer[1] = 0x00;
	master->TX_buffer[0] = 0x00;

	master->dead_time = DEAD_TIME;

	master->calibration.CALP = CALP_INCREASE_FREQUENCY;
    master->calibration.CALM = CALM_DEFAULT;

	return 1;
}

uint8_t Set_System_Mode_and_Store_Previous_Mode(struct System_Mode_Tracker *system_mode_tracker, enum System_Mode desired_mode){

	system_mode_tracker->previous_mode = system_mode_tracker->current_mode;
	system_mode_tracker->current_mode = desired_mode;

	return 1;
}


