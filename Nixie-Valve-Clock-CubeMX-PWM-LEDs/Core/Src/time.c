/*
 * time.c
 *
 *  Created on: 21 Dec 2025
 *      Author: James Garvey
 */

#include "system.h"

uint8_t Get_RTC_Time(void){

	HAL_RTC_GetTime(&hrtc, (RTC_TimeTypeDef*)&master.get_time, RTC_FORMAT_BCD);
	HAL_RTC_GetDate(&hrtc, (RTC_DateTypeDef*)&master.get_date, RTC_FORMAT_BCD);

	return 1;
}
