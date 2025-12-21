/*
 * alarm.c
 *
 *  Created on: 21 Dec 2025
 *      Author: James Garvey
 */

#include "system.h"

const uint16_t triangle_wavetable[TRI_WAVETABLE_SIZE] = {
      0,   512,  1024,  1536,  2048,  2560,  3072,  3584,
   4096,  4608,  5120,  5632,  6144,  6656,  7168,  7680,
   8192,  8704,  9216,  9728, 10240, 10752, 11264, 11776,
  12288, 12800, 13312, 13824, 14336, 14848, 15360, 15872,
  16384, 16896, 17408, 17920, 18432, 18944, 19456, 19968,
  20480, 20992, 21504, 22016, 22528, 23040, 23552, 24064,
  24576, 25088, 25600, 26112, 26624, 27136, 27648, 28160,
  28672, 29184, 29696, 30208, 30720, 31232, 31744, 32256,
  32768, 33280, 33792, 34304, 34816, 35328, 35840, 36352,
  36864, 37376, 37888, 38400, 38912, 39424, 39936, 40448,
  40960, 41472, 41984, 42496, 43008, 43520, 44032, 44544,
  45056, 45568, 46080, 46592, 47104, 47616, 48128, 48640,
  49152, 49664, 50176, 50688, 51200, 51712, 52224, 52736,
  53248, 53760, 54272, 54784, 55296, 55808, 56320, 56832,
  57344, 57856, 58368, 58880, 59392, 59904, 60416, 60928,
  61440, 61952, 62464, 62976, 63488, 64000, 64512, 65024,
  65535, 65024, 64512, 64000, 63488, 62976, 62464, 61952,
  61440, 60928, 60416, 59904, 59392, 58880, 58368, 57856,
  57344, 56832, 56320, 55808, 55296, 54784, 54272, 53760,
  53248, 52736, 52224, 51712, 51200, 50688, 50176, 49664,
  49152, 48640, 48128, 47616, 47104, 46592, 46080, 45568,
  45056, 44544, 44032, 43520, 43008, 42496, 41984, 41472,
  40960, 40448, 39936, 39424, 38912, 38400, 37888, 37376,
  36864, 36352, 35840, 35328, 34816, 34304, 33792, 33280,
  32768, 32256, 31744, 31232, 30720, 30208, 29696, 29184,
  28672, 28160, 27648, 27136, 26624, 26112, 25600, 25088,
  24576, 24064, 23552, 23040, 22528, 22016, 21504, 20992,
  20480, 19968, 19456, 18944, 18432, 17920, 17408, 16896,
  16384, 15872, 15360, 14848, 14336, 13824, 13312, 12800,
  12288, 11776, 11264, 10752, 10240,  9728,  9216,  8704,
   8192,  7680,  7168,  6656,  6144,  5632,  5120,  4608,
   4096,  3584,  3072,  2560,  2048,  1536,  1024,   512
};

uint8_t Sound_Alarm(void){

	const uint32_t short_tone = BUZZER_SHORT_TONE_COUNT;
	const uint32_t short_off = BUZZER_SHORT_OFF_COUNT;
	const uint32_t long_off = BUZZER_LONG_OFF_COUNT;

	if(master.alarm.alarm_counter != BUZZER_COUNT_MAX){

		if(master.alarm.alarm_counter <= short_tone){

			HAL_GPIO_WritePin(GPIO_Output_BUZZER_GPIO_Port, GPIO_Output_BUZZER_Pin, 1);
		}
		else if(master.alarm.alarm_counter > short_tone && master.alarm.alarm_counter <= (short_tone + short_off)){

			HAL_GPIO_WritePin(GPIO_Output_BUZZER_GPIO_Port, GPIO_Output_BUZZER_Pin, 0);
		}
		else if(master.alarm.alarm_counter > (short_tone + short_off) && master.alarm.alarm_counter <= (short_tone + short_off + short_tone)){

			HAL_GPIO_WritePin(GPIO_Output_BUZZER_GPIO_Port, GPIO_Output_BUZZER_Pin, 1);
		}
		else if(master.alarm.alarm_counter > (short_tone + short_off + short_tone) && master.alarm.alarm_counter <= (short_tone + short_off + short_tone + long_off)){

			HAL_GPIO_WritePin(GPIO_Output_BUZZER_GPIO_Port, GPIO_Output_BUZZER_Pin, 0);
		}

		master.alarm.alarm_counter++;
	}
	else{

		master.alarm.alarm_counter = 0;
	}

	if(master.alarm.alarm_triggered == 0){ //necessary as sometimes the buzzer gets stuck ON

		HAL_GPIO_WritePin(GPIO_Output_BUZZER_GPIO_Port, GPIO_Output_BUZZER_Pin, 0);
	}

	return 1;
}

uint8_t Set_Alarm(uint8_t BCD_HH, uint8_t BCD_MM, uint8_t BCD_SS){

	RTC_AlarmTypeDef alarm;

	alarm.Alarm = RTC_ALARM_A;
	alarm.AlarmDateWeekDay = RTC_WEEKDAY_MONDAY;
	alarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;
	alarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY; //Alarm masks the date, so alarm will sound every day at the same time, unless cleared by user
	alarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
	alarm.AlarmTime.TimeFormat = RTC_HOURFORMAT12_AM;
	alarm.AlarmTime.Hours = BCD_HH;
	alarm.AlarmTime.Minutes = BCD_MM;
	alarm.AlarmTime.Seconds = BCD_SS;
	alarm.AlarmTime.SubSeconds = 0x00; //subseconds masked so don't care

	if(HAL_RTC_SetAlarm_IT(&hrtc, &alarm, RTC_FORMAT_BCD) != HAL_OK)
	{
	  /* Initialization Error */
	  Error_Handler();
	}

	return 1;
}

uint8_t Clear_Alarm(void){

	HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);
	master.alarm.alarm_counter = 0;
	master.alarm.alarm_triggered = 0;
	HAL_GPIO_WritePin(GPIO_Output_BUZZER_GPIO_Port, GPIO_Output_BUZZER_Pin, 0);

	return 1;
}

