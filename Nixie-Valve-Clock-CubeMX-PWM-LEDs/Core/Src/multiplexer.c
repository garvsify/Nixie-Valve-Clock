#include "multiplexer.h"

GPIO_TypeDef* Valve_Anode_Registers[NUM_VALVES] = {GPIO_Output_AS_0_GPIO_Port, GPIO_Output_AS_1_GPIO_Port, GPIO_Output_AS_2_GPIO_Port, GPIO_Output_AS_3_GPIO_Port, GPIO_Output_AS_4_GPIO_Port, GPIO_Output_AS_5_GPIO_Port};
uint16_t Valve_Anode_Pins[NUM_VALVES] = {GPIO_Output_AS_0_Pin, GPIO_Output_AS_1_Pin, GPIO_Output_AS_2_Pin, GPIO_Output_AS_3_Pin, GPIO_Output_AS_4_Pin, GPIO_Output_AS_5_Pin};

GPIO_TypeDef* BCD_Registers[NUM_BINARY_DIGITS_IN_BCD] = {GPIO_Output_BCD0_GPIO_Port, GPIO_Output_BCD1_GPIO_Port, GPIO_Output_BCD2_GPIO_Port, GPIO_Output_BCD3_GPIO_Port};
uint16_t BCD_Pins[NUM_BINARY_DIGITS_IN_BCD] = {GPIO_Output_BCD0_Pin, GPIO_Output_BCD1_Pin, GPIO_Output_BCD2_Pin, GPIO_Output_BCD3_Pin};

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

uint8_t Write_Digit_to_Valve(uint8_t valve_num, uint8_t BCD_of_digit){

	//turn off all anodes of valves except of valve of interest
	for(uint8_t valve = 0; valve < NUM_VALVES; valve++){

		if(valve != valve_num){

			HAL_GPIO_WritePin(Valve_Anode_Registers[valve], Valve_Anode_Pins[valve], VALVE_ANODE_OFF_STATE);
		}
		else{

			HAL_GPIO_WritePin(Valve_Anode_Registers[valve], Valve_Anode_Pins[valve], VALVE_ANODE_ON_STATE);
		}
	}

	//write BCD of digit to GPIOs that drive the BCD chip
	for(uint8_t i = 0; i < NUM_BINARY_DIGITS_IN_BCD; i++){

		HAL_GPIO_WritePin(BCD_Registers[i], BCD_Pins[i], ((BCD_of_digit >> i) & 0b1));
	}

	return 1;
}

uint8_t Turn_Valve_Off(uint8_t valve){

	HAL_GPIO_WritePin(Valve_Anode_Registers[valve], Valve_Anode_Pins[valve], VALVE_ANODE_OFF_STATE);

	return 1;
}

uint8_t Turn_All_Valves_Off(void){

	for(uint8_t valve = 0; valve < NUM_VALVES; valve++){

		HAL_GPIO_WritePin(Valve_Anode_Registers[valve], Valve_Anode_Pins[valve], VALVE_ANODE_OFF_STATE);
	}
	return 1;
}

uint8_t Start_Multiplexer_Timer(void){

	__HAL_TIM_ENABLE_IT(&htim17, TIM_IT_UPDATE); //make sure overflow (update) interrupt is enabled for TIM17

	uint8_t ok = Start_OC_TIM(&htim17, TIM_CHANNEL_1);

	if(ok != HAL_OK){

		Error_Handler();
	}

	return ok;
}

uint8_t Start_Anti_Cathode_Poisoning_Timer(void){

	__HAL_TIM_ENABLE_IT(&htim16, TIM_IT_UPDATE); //make sure overflow (update) interrupt is enabled for TIM16

	uint8_t ok = Start_OC_TIM(&htim16, TIM_CHANNEL_1);

	if(ok != HAL_OK){

		Error_Handler();
	}

	return ok;
}

uint8_t Start_Adjust_Time_Slash_Alarm_Set_Mode_Timer(void){

	__HAL_TIM_ENABLE_IT(&htim14, TIM_IT_UPDATE); //make sure overflow (update) interrupt is enabled for TIM14

	uint8_t ok = Start_OC_TIM(&htim14, TIM_CHANNEL_1);

	if(ok != HAL_OK){

		Error_Handler();
	}

	return ok;
}

uint8_t Stop_Adjust_Time_Slash_Alarm_Set_Mode_Timer(void){

	uint8_t ok = Stop_OC_TIM(&htim14, TIM_CHANNEL_1);

	if(ok != HAL_OK){

		Error_Handler();
	}

	return ok;
}

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

	master->dead_time = DEAD_TIME;

	master->adjust_ppm_polarity = ADJUST_TIME_CAL_DECREASE_FREQUENCY;
    master->adjust_ppm = ADJUST_TIME_CAL_PPM;

	return 1;
}

uint8_t Set_System_Mode_and_Store_Previous_Mode(struct System_Mode_Tracker *system_mode_tracker, enum System_Mode desired_mode){

	system_mode_tracker->previous_mode = system_mode_tracker->current_mode;
	system_mode_tracker->current_mode = desired_mode;

	return 1;
}

uint8_t Toggle_HV_Power_Supply(uint8_t toggle){

	if(toggle){

		HAL_GPIO_WritePin(GPIO_Output__HV_SHDN_GPIO_Port, GPIO_Output__HV_SHDN_Pin, 1);
	}
	else if(!toggle){

		HAL_GPIO_WritePin(GPIO_Output__HV_SHDN_GPIO_Port, GPIO_Output__HV_SHDN_Pin, 0);
	}

	return 1;
}

uint8_t Get_RTC_Time(void){

	HAL_RTC_GetTime(&hrtc, (RTC_TimeTypeDef*)&master.get_time, RTC_FORMAT_BCD);
	HAL_RTC_GetDate(&hrtc, (RTC_DateTypeDef*)&master.get_date, RTC_FORMAT_BCD);

	return 1;
}

uint8_t Write_Time_In_Flash(RTC_TimeTypeDef *time){

	uint64_t packed;
	uint32_t errors = 0;

	FLASH_EraseInitTypeDef erase_config = {.TypeErase = FLASH_CR_PER, .Banks = FLASH_CR_MER1, .Page = 31, .NbPages = 1};

	HAL_FLASH_Unlock();
	HAL_FLASHEx_Erase(&erase_config, &errors);

	Pack_Time_Into_Doubleword(time, &packed);

	HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, TIME_FLASH_BASE_ADDRESS, packed);

	HAL_FLASH_Lock();

	return 1;
}

uint8_t Read_Time_From_Flash(RTC_TimeTypeDef *time){

	uint32_t address_val = TIME_FLASH_BASE_ADDRESS;
	uint8_t* address = (uint8_t *)address_val;

	if(*address == 0xFF){

		time->Seconds = 0x00;
	}
	else{

		time->Seconds = *address;
	}
	if(*(address + 1) == 0xFF){

		time->Minutes = 0x00;
	}
	else{

		time->Minutes = *(address + 1);
	}
	if(*(address + 2) == 0xFF){

		time->Hours = 0x00;
	}
	else{

		time->Hours = *(address + 2);
	}

	return 1;
}

uint8_t Pack_Time_Into_Doubleword(RTC_TimeTypeDef *time, uint64_t *doubleword){

	uint64_t packed = 0;

	packed |= (time->Hours << 16);
	packed |= (time->Minutes << 8);
	packed |= (time->Seconds);

	*doubleword = packed;

	return 1;
}

uint8_t Check_Rotary_Encoder_Switch_State(volatile struct Rotary_Encoder_Switch_States *rotary_encoder_switch_states_ptr){

	static uint8_t extend_rising_edge = 0;

	static uint8_t rotary_encoder_switch_state_counter = ROTARY_ENCODER_SWITCH_CONFIDENCE_COUNT;

	uint8_t switch_state = (uint8_t)HAL_GPIO_ReadPin(GPIO_EXTI15_SW_GPIO_Port, GPIO_EXTI15_SW_Pin);

	if(switch_state == 0){

		if(rotary_encoder_switch_state_counter != 0){

			rotary_encoder_switch_state_counter--;

		}
	}
	else{

		if(rotary_encoder_switch_state_counter != ROTARY_ENCODER_SWITCH_CONFIDENCE_COUNT){

			if(extend_rising_edge == COUNT_TO_DELAY_RISING_ROTARY_ENCODER_EDGE){

				rotary_encoder_switch_state_counter++;
				extend_rising_edge = 0;
			}
			else{

				extend_rising_edge++;
			}
		}
	}

	if(rotary_encoder_switch_state_counter == 0){

		rotary_encoder_switch_states_ptr->rotary_encoder_switch_state = ROTARY_ENCODER_SWITCH_STATE_DEPRESSED;
	}
	else if(rotary_encoder_switch_state_counter == ROTARY_ENCODER_SWITCH_CONFIDENCE_COUNT){

		rotary_encoder_switch_states_ptr->rotary_encoder_switch_state = ROTARY_ENCODER_SWITCH_STATE_NOT_DEPRESSED;
	}

	return 1;
}

uint8_t Set_Fault_LED_ON(void){

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, FAULT_RED_LED_BRIGHTNESS_CCR);

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

uint8_t Set_Adjust_Time_LED_ON(void){

	//Set Rotary Encoder LED to Green

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, ADJUST_TIME_GREEN_LED_BRIGHTNESS_CCR);

	return 1;
}

uint8_t Set_Adjust_Time_LED_OFF(void){

	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);

	return 1;
}

uint8_t Set_Alarm_Set_LEDs_ON(void){

	//Set Rotary Encoder LEDs to Orange

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, ALARM_SET_GREEN_LED_BRIGHTNESS_CCR);

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, ALARM_SET_RED_LED_BRIGHTNESS_CCR);

	return 1;
}

uint8_t Set_Alarm_Set_LEDs_OFF(void){

	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);

	return 1;
}

uint8_t Initialise_Rotary_Encoder_LEDs(void){

	HAL_TIM_Base_Start(&htim3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);

	return 1;
}

uint8_t Initialise_Valve_LEDs(void){

	HAL_TIM_Base_Start(&htim1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);

	return 1;
}

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

	return 1;
}

uint8_t Double_Flash_Red_Rotary_Encoder_LED(void){

	if(master.leds.LED_counter[ROTARY_ENCODER_RED_LED_NUM] != LED_DOUBLE_FLASH_COUNT_MAX){

		if(master.leds.LED_counter[ROTARY_ENCODER_RED_LED_NUM] <= LED_SHORT_ON_COUNT){

			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, RED_LED_DOUBLE_FLASH_BRIGHTNESS_CCR);
		}
		else if(master.leds.LED_counter[ROTARY_ENCODER_RED_LED_NUM] > LED_SHORT_ON_COUNT
		          && master.leds.LED_counter[ROTARY_ENCODER_RED_LED_NUM] <= (LED_SHORT_ON_COUNT + LED_SHORT_OFF_COUNT)){

			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
		}
		else if(master.leds.LED_counter[ROTARY_ENCODER_RED_LED_NUM] > (LED_SHORT_ON_COUNT + LED_SHORT_OFF_COUNT)
				          && master.leds.LED_counter[ROTARY_ENCODER_RED_LED_NUM] <= (LED_SHORT_ON_COUNT + LED_SHORT_OFF_COUNT + LED_SHORT_ON_COUNT)){

			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, RED_LED_DOUBLE_FLASH_BRIGHTNESS_CCR);
		}
		else if(master.leds.LED_counter[ROTARY_ENCODER_RED_LED_NUM] > (LED_SHORT_ON_COUNT + LED_SHORT_OFF_COUNT + LED_SHORT_ON_COUNT)
						  && master.leds.LED_counter[ROTARY_ENCODER_RED_LED_NUM] <= (LED_SHORT_ON_COUNT + LED_SHORT_OFF_COUNT + LED_SHORT_ON_COUNT + LED_SHORT_OFF_COUNT)){

			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
		}

		master.leds.LED_counter[ROTARY_ENCODER_RED_LED_NUM]++;
	}
	else{

		master.leds.LED_counter[ROTARY_ENCODER_RED_LED_NUM] = 0;
		master.leds.Double_Flash_Red_LED = 0;
	}

	return 1;
}
