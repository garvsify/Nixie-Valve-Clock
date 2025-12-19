#include "multiplexer.h"

GPIO_TypeDef* Valve_Anode_Registers[NUM_VALVES] = {GPIO_Output_AS_0_GPIO_Port, GPIO_Output_AS_1_GPIO_Port, GPIO_Output_AS_2_GPIO_Port, GPIO_Output_AS_3_GPIO_Port, GPIO_Output_AS_4_GPIO_Port, GPIO_Output_AS_5_GPIO_Port};
uint16_t Valve_Anode_Pins[NUM_VALVES] = {GPIO_Output_AS_0_Pin, GPIO_Output_AS_1_Pin, GPIO_Output_AS_2_Pin, GPIO_Output_AS_3_Pin, GPIO_Output_AS_4_Pin, GPIO_Output_AS_5_Pin};

GPIO_TypeDef* BCD_Registers[NUM_BINARY_DIGITS_IN_BCD] = {GPIO_Output_BCD0_GPIO_Port, GPIO_Output_BCD1_GPIO_Port, GPIO_Output_BCD2_GPIO_Port, GPIO_Output_BCD3_GPIO_Port};
uint16_t BCD_Pins[NUM_BINARY_DIGITS_IN_BCD] = {GPIO_Output_BCD0_Pin, GPIO_Output_BCD1_Pin, GPIO_Output_BCD2_Pin, GPIO_Output_BCD3_Pin};

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
	HAL_GPIO_WritePin(GPIO_Output_BUZZER_GPIO_Port, GPIO_Output_BUZZER_Pin, 0);
	master.alarm.alarm_triggered = 0;

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

	static uint32_t counter = 0;
	const uint32_t short_tone = BUZZER_SHORT_TONE_COUNT;
	const uint32_t short_off = BUZZER_SHORT_OFF_COUNT;
	const uint32_t long_off = BUZZER_LONG_OFF_COUNT;

	if(counter != BUZZER_COUNT_MAX){

		if(counter <= short_tone){

			HAL_GPIO_WritePin(GPIO_Output_BUZZER_GPIO_Port, GPIO_Output_BUZZER_Pin, 1);
		}
		else if(counter > short_tone && counter <= (short_tone + short_off)){

			HAL_GPIO_WritePin(GPIO_Output_BUZZER_GPIO_Port, GPIO_Output_BUZZER_Pin, 0);
		}
		else if(counter > (short_tone + short_off) && counter <= (short_tone + short_off + short_tone)){

			HAL_GPIO_WritePin(GPIO_Output_BUZZER_GPIO_Port, GPIO_Output_BUZZER_Pin, 1);
		}
		else if(counter > (short_tone + short_off + short_tone) && counter <= (short_tone + short_off + short_tone + long_off)){

			HAL_GPIO_WritePin(GPIO_Output_BUZZER_GPIO_Port, GPIO_Output_BUZZER_Pin, 0);
		}

		counter++;
	}
	else{

		counter = 0;
	}

	return 1;
}
