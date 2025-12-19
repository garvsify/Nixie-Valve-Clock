/*

Issues:

With default RTC predscalers (127,255) the clock loses just over 3 minutes every ~8 hours

*/

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim14;
TIM_HandleTypeDef htim16;
TIM_HandleTypeDef htim17;
LPTIM_HandleTypeDef hlptim1;

UART_HandleTypeDef huart2;

struct Master master;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM17_Init(void);
static void MX_TIM16_Init(void);
static void MX_TIM14_Init(void);
static void MX_LPTIM1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	//for (volatile int i = 0; i < 100000; i++);

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_RTC_Init();
	MX_TIM2_Init();
	MX_TIM3_Init();
	MX_USART2_UART_Init();
	MX_TIM1_Init();
	MX_TIM17_Init();
	MX_TIM16_Init();
	MX_TIM14_Init();
	MX_LPTIM1_Init();
	/* USER CODE BEGIN 2 */

	//Assign custom callbacks
	HAL_TIM_RegisterCallback(&htim17, HAL_TIM_PERIOD_ELAPSED_CB_ID, &TIM17_Multiplexer_Sequencer_Callback);
	HAL_TIM_RegisterCallback(&htim16, HAL_TIM_PERIOD_ELAPSED_CB_ID, &TIM16_Anti_Cathode_Poisoning_Callback);
	HAL_TIM_RegisterCallback(&htim14, HAL_TIM_PERIOD_ELAPSED_CB_ID, &TIM14_Valve_Blink_Callback);
	//HAL_TIM_RegisterCallback(&htim1, HAL_TIM_PERIOD_ELAPSED_CB_ID, &TIM1_CH1_Valve_LED_0_Callback);
	//HAL_TIM_RegisterCallback(&htim1, HAL_TIM_PERIOD_ELAPSED_CB_ID, &TIM1_CH2_Valve_LED_1_Callback);
	//HAL_TIM_RegisterCallback(&htim1, HAL_TIM_PERIOD_ELAPSED_CB_ID, &TIM1_CH3_Valve_LED_2_Callback);
	HAL_LPTIM_RegisterCallback(&hlptim1, HAL_LPTIM_COMPARE_MATCH_CB_ID, &LPTIM1_Rotary_Encoder_Switch_Callback);
	HAL_RTC_RegisterCallback(&hrtc, HAL_RTC_ALARM_A_EVENT_CB_ID, &Alarm_Callback);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

	//Read_Time_From_Flash((RTC_TimeTypeDef*)&master.get_time);
	RTC_Time_Init();
	HAL_RTCEx_SetSmoothCalib(&hrtc, 32, 1, 161); //calibrate time - second number = 0 -> slows down clock using final number //second number = 1 -> quickens the clock using final number
	Master_Init(&master);
	Start_Multiplexer_Timer();
	Start_Anti_Cathode_Poisoning_Timer();
	Toggle_HV_Power_Supply(1);
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	HAL_LPTIM_SetOnce_Start_IT(&hlptim1, LPTIM1_CCR_CHECK, LPTIM1_CCR_CHECK);
	Initialise_Rotary_Encoder_LEDs();
	Initialise_Valve_LEDs();

	//__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 10000);
	//__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 10000);
	//__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 10000);

	while (1)
	{
		if(master.alarm.alarm_triggered != 1){

			HAL_Delay(50);

			master.encoder_first = master.encoder_second;
					master.encoder_second = __HAL_TIM_GET_COUNTER(&htim2) >> 1;
			    /* USER CODE END WHILE */
					if(master.system_mode_tracker.current_mode == HH_ADJUST_MODE ||
							master.system_mode_tracker.current_mode == MM_ADJUST_MODE ||
								master.system_mode_tracker.current_mode == SS_ADJUST_MODE){

						if(master.system_mode_tracker.current_mode == HH_ADJUST_MODE){

							if(master.encoder_second >= master.encoder_first){

								if((master.time_adjust.Hours_Bin + (master.encoder_second - master.encoder_first)) > 23){

									uint8_t temp = master.time_adjust.Hours_Bin + (master.encoder_second - master.encoder_first);
									temp -= 23;

									master.time_adjust.Hours_Bin = temp - 1;
								}
								else{

									master.time_adjust.Hours_Bin += (master.encoder_second - master.encoder_first);
								}
							}
							else{

								if(((int16_t)master.time_adjust.Hours_Bin - ((int16_t)master.encoder_first - (int16_t)master.encoder_second)) < 0){

									int16_t temp = (int16_t)master.time_adjust.Hours_Bin - ((int16_t)master.encoder_first - (int16_t)master.encoder_second);
									temp += 23;

									master.time_adjust.Hours_Bin = temp + 1;
								}
								else{

									master.time_adjust.Hours_Bin -= (master.encoder_first - master.encoder_second);
								}
							}
						}
						else if(master.system_mode_tracker.current_mode == MM_ADJUST_MODE){

							if(master.encoder_second >= master.encoder_first){

								if((master.time_adjust.Minutes_Bin + (master.encoder_second - master.encoder_first)) > 59){

									uint8_t temp = master.time_adjust.Minutes_Bin + (master.encoder_second - master.encoder_first);
									temp -= 59;

									master.time_adjust.Minutes_Bin = temp - 1;
								}
								else{

									master.time_adjust.Minutes_Bin += (master.encoder_second - master.encoder_first);
								}
							}
							else{

								if(((int16_t)master.time_adjust.Minutes_Bin - ((int16_t)master.encoder_first - (int16_t)master.encoder_second)) < 0){

									int16_t temp = (int16_t)master.time_adjust.Minutes_Bin - ((int16_t)master.encoder_first - (int16_t)master.encoder_second);
									temp += 59;

									master.time_adjust.Minutes_Bin = temp + 1;
								}
								else{

									master.time_adjust.Minutes_Bin -= (master.encoder_first - master.encoder_second);
								}
							}
						}
						else if(master.system_mode_tracker.current_mode == SS_ADJUST_MODE){

							if(master.encoder_second >= master.encoder_first){

								if((master.time_adjust.Seconds_Bin + (master.encoder_second - master.encoder_first)) > 59){

									uint8_t temp = master.time_adjust.Seconds_Bin + (master.encoder_second - master.encoder_first);
									temp -= 59;

									master.time_adjust.Seconds_Bin = temp - 1;
								}
								else{

									master.time_adjust.Seconds_Bin += (master.encoder_second - master.encoder_first);
								}
							}
							else{

								if(((int16_t)master.time_adjust.Seconds_Bin - ((int16_t)master.encoder_first - (int16_t)master.encoder_second)) < 0){

									int16_t temp = (int16_t)master.time_adjust.Seconds_Bin - ((int16_t)master.encoder_first - (int16_t)master.encoder_second);
									temp += 59;

									master.time_adjust.Seconds_Bin = temp + 1;
								}
								else{

									master.time_adjust.Seconds_Bin -= (master.encoder_first - master.encoder_second);
								}
							}
						}
						master.time_adjust.adjust_time.Hours = RTC_ByteToBcd2(master.time_adjust.Hours_Bin);
						master.time_adjust.adjust_time.Minutes = RTC_ByteToBcd2(master.time_adjust.Minutes_Bin);
						master.time_adjust.adjust_time.Seconds = RTC_ByteToBcd2(master.time_adjust.Seconds_Bin);
					}

					if(master.system_mode_tracker.current_mode == ALARM_SET_HH_MODE ||
									master.system_mode_tracker.current_mode == ALARM_SET_MM_MODE ||
										master.system_mode_tracker.current_mode == ALARM_SET_SS_MODE){

						if(master.system_mode_tracker.current_mode == ALARM_SET_HH_MODE){

							if(master.encoder_second >= master.encoder_first){

								if((master.alarm.Hours_Bin + (master.encoder_second - master.encoder_first)) > 23){

									uint8_t temp = master.alarm.Hours_Bin + (master.encoder_second - master.encoder_first);
									temp -= 23;

									master.alarm.Hours_Bin = temp - 1;
								}
								else{

									master.alarm.Hours_Bin += (master.encoder_second - master.encoder_first);
								}
							}
							else{

								if(((int16_t)master.alarm.Hours_Bin - ((int16_t)master.encoder_first - (int16_t)master.encoder_second)) < 0){

									int16_t temp = (int16_t)master.alarm.Hours_Bin - ((int16_t)master.encoder_first - (int16_t)master.encoder_second);
									temp += 23;

									master.alarm.Hours_Bin = temp + 1;
								}
								else{

									master.alarm.Hours_Bin -= (master.encoder_first - master.encoder_second);
								}
							}
						}
						else if(master.system_mode_tracker.current_mode == ALARM_SET_MM_MODE){

							if(master.encoder_second >= master.encoder_first){

								if((master.alarm.Minutes_Bin + (master.encoder_second - master.encoder_first)) > 59){

									uint8_t temp = master.alarm.Minutes_Bin + (master.encoder_second - master.encoder_first);
									temp -= 59;

									master.alarm.Minutes_Bin = temp - 1;
								}
								else{

									master.alarm.Minutes_Bin += (master.encoder_second - master.encoder_first);
								}
							}
							else{

								if(((int16_t)master.alarm.Minutes_Bin - ((int16_t)master.encoder_first - (int16_t)master.encoder_second)) < 0){

									int16_t temp = (int16_t)master.alarm.Minutes_Bin - ((int16_t)master.encoder_first - (int16_t)master.encoder_second);
									temp += 59;

									master.alarm.Minutes_Bin = temp + 1;
								}
								else{

									master.alarm.Minutes_Bin -= (master.encoder_first - master.encoder_second);
								}
							}
						}
						else if(master.system_mode_tracker.current_mode == ALARM_SET_SS_MODE){

							if(master.encoder_second >= master.encoder_first){

								if((master.alarm.Seconds_Bin + (master.encoder_second - master.encoder_first)) > 59){

									uint8_t temp = master.alarm.Seconds_Bin + (master.encoder_second - master.encoder_first);
									temp -= 59;

									master.alarm.Seconds_Bin = temp - 1;
								}
								else{

									master.alarm.Seconds_Bin += (master.encoder_second - master.encoder_first);
								}
							}
							else{

								if(((int16_t)master.alarm.Seconds_Bin - ((int16_t)master.encoder_first - (int16_t)master.encoder_second)) < 0){

									int16_t temp = (int16_t)master.alarm.Seconds_Bin - ((int16_t)master.encoder_first - (int16_t)master.encoder_second);
									temp += 59;

									master.alarm.Seconds_Bin = temp + 1;
								}
								else{

									master.alarm.Seconds_Bin -= (master.encoder_first - master.encoder_second);
								}
							}
						}
						master.alarm.alarm_time.Hours = RTC_ByteToBcd2(master.alarm.Hours_Bin);
						master.alarm.alarm_time.Minutes = RTC_ByteToBcd2(master.alarm.Minutes_Bin);
						master.alarm.alarm_time.Seconds = RTC_ByteToBcd2(master.alarm.Seconds_Bin);
					}
		}

		/* USER CODE BEGIN 3 */
		else{

			Sound_Alarm();
		}
	}
  /* USER CODE END 3 */
}

void RTC_Time_Init(void)
{
  //RTC_DateTypeDef sdatestructure;
  RTC_TimeTypeDef time;
  RTC_DateTypeDef date;

  /*##-1- Configure the Date #################################################*/
  /* Set Date: Tuesday February 18th 2016 */
  date.Year = 0x16;
  date.Month = RTC_MONTH_FEBRUARY;
  date.Date = 0x18;
  date.WeekDay = RTC_WEEKDAY_TUESDAY;

  if(HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }

  /*##-2- Configure the Time #################################################*/
  /* Set Time: 02:00:00 */
  time.Hours = 0x00;//master.get_time.Hours;
  time.Minutes = 0x00;//master.get_time.Minutes;
  time.Seconds = 0x00;//master.get_time.Seconds;
  time.TimeFormat = RTC_HOURFORMAT12_AM;
  time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
  time.StoreOperation = RTC_STOREOPERATION_RESET;

  if (HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  //__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_HIGH);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  hrtc.Init.OutPutPullUp = RTC_OUTPUT_PULLUP_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  HAL_NVIC_SetPriority(RTC_TAMP_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(RTC_TAMP_IRQn);

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 4000;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.BreakAFMode = TIM_BREAK_AFMODE_INPUT;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.Break2AFMode = TIM_BREAK_AFMODE_INPUT;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  //htim1.Instance->BDTR |= TIM_BDTR_MOE;

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
  sConfig.IC1Polarity = TIM_ICPOLARITY_FALLING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 15;
  sConfig.IC2Polarity = TIM_ICPOLARITY_FALLING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}


/**
  * @brief TIM14 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM14_Init(void)
{

  /* USER CODE BEGIN TIM14_Init 0 */

  /* USER CODE END TIM14_Init 0 */

  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM14_Init 1 */

  /* USER CODE END TIM14_Init 1 */
  htim14.Instance = TIM14;
  htim14.Init.Prescaler = TIME_ADJUST_BLINK_PRESCALER;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim14.Init.Period = TIME_ADJUST_BLINK_PERIOD_MINUS_ONE;
  htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
  htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OC_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_OC_ConfigChannel(&htim14, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM14_Init 2 */

  /* USER CODE END TIM14_Init 2 */

}

/**
  * @brief TIM16 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM16_Init(void)
{

  /* USER CODE BEGIN TIM16_Init 0 */

  /* USER CODE END TIM16_Init 0 */

  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM16_Init 1 */

  /* USER CODE END TIM16_Init 1 */
  htim16.Instance = TIM16;
  htim16.Init.Prescaler = ANTI_CATHODE_POISONING_TIMER_WAITING_MODE_PRESCALER;
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = ANTI_CATHODE_POISONING_TIMER_WAITING_MODE_PERIOD_MINUS_ONE;
  htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
  htim16.Init.RepetitionCounter = 0;
  htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OC_Init(&htim16) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_OC_ConfigChannel(&htim16, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim16, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM16_Init 2 */

  /* USER CODE END TIM16_Init 2 */

}

/**
  * @brief TIM17 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM17_Init(void)
{

  /* USER CODE BEGIN TIM17_Init 0 */

  /* USER CODE END TIM17_Init 0 */

  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM17_Init 1 */

  /* USER CODE END TIM17_Init 1 */
  htim17.Instance = TIM17;
  htim17.Init.Prescaler = MULTIPLEXER_TIMER_PRESCALER;
  htim17.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim17.Init.Period = MULTIPLEXER_TIMER_PERIOD_MINUS_ONE;
  htim17.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
  htim17.Init.RepetitionCounter = 0;
  htim17.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim17) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OC_Init(&htim17) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_OC_ConfigChannel(&htim17, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim17, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM17_Init 2 */

  /* USER CODE END TIM17_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pins : GPIO_Output_BUZZER_Pin GPIO_Output__HV_SHDN_Pin GPIO_Output_IN_3_0_Pin GPIO_Output_IN_3_1_Pin
                           GPIO_Output_BCD1_Pin GPIO_Output_BCD2_Pin GPIO_Output_BCD3_Pin GPIO_Output_BCD0_Pin */
  GPIO_InitStruct.Pin = GPIO_Output_BUZZER_Pin|GPIO_Output__HV_SHDN_Pin|GPIO_Output_IN_3_0_Pin|GPIO_Output_IN_3_1_Pin
                          |GPIO_Output_BCD1_Pin|GPIO_Output_BCD2_Pin|GPIO_Output_BCD3_Pin|GPIO_Output_BCD0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : T_NRST_Pin */
  GPIO_InitStruct.Pin = T_NRST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(T_NRST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : GPIO_Output_AS_0_Pin GPIO_Output_AS_1_Pin GPIO_Output_AS_2_Pin GPIO_Output_AS_3_Pin
                           GPIO_Output_AS_4_Pin GPIO_Output_AS_5_Pin */
  GPIO_InitStruct.Pin = GPIO_Output_AS_0_Pin|GPIO_Output_AS_1_Pin|GPIO_Output_AS_2_Pin|GPIO_Output_AS_3_Pin
                          |GPIO_Output_AS_4_Pin|GPIO_Output_AS_5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : GPIO_EXTI1_FAULT_Pin */
  GPIO_InitStruct.Pin = GPIO_EXTI1_FAULT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIO_EXTI1_FAULT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : GPIO_EXTI15_SW_Pin */
  /*GPIO_InitStruct.Pin = GPIO_EXTI15_SW_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIO_EXTI15_SW_GPIO_Port, &GPIO_InitStruct);*/

  /*Configure GPIO pin : GPIO_EXTI15_SW_Pin */
  GPIO_InitStruct.Pin = GPIO_EXTI15_SW_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIO_EXTI15_SW_GPIO_Port, &GPIO_InitStruct); //not using interrupts, can't be bothered to change GPIO name #define

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_Output_BUZZER_Pin|GPIO_Output__HV_SHDN_Pin|GPIO_Output_IN_3_0_Pin|GPIO_Output_IN_3_1_Pin
                          |GPIO_Output_BCD1_Pin|GPIO_Output_BCD2_Pin|GPIO_Output_BCD3_Pin|GPIO_Output_BCD0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_Output_AS_0_Pin|GPIO_Output_AS_1_Pin|GPIO_Output_AS_2_Pin|GPIO_Output_AS_3_Pin
                          |GPIO_Output_AS_4_Pin|GPIO_Output_AS_5_Pin, GPIO_PIN_SET);

#define T_JTMS_Pin GPIO_PIN_13
#define T_JTMS_GPIO_Port GPIOA
#define T_JTCK_Pin GPIO_PIN_14
#define T_JTCK_GPIO_Port GPIOA

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

  /*HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);*/ //not longer checking rotary encoder switch with interrupts

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

void MX_LPTIM1_Init(void)
{

  /* USER CODE BEGIN LPTIM1_Init 0 */

  /* USER CODE END LPTIM1_Init 0 */

  /* USER CODE BEGIN LPTIM1_Init 1 */

  /* USER CODE END LPTIM1_Init 1 */
  hlptim1.Instance = LPTIM1;
  hlptim1.Init.Clock.Source = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
  hlptim1.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV128;
  hlptim1.Init.Trigger.Source = LPTIM_TRIGSOURCE_SOFTWARE;
  hlptim1.Init.OutputPolarity = LPTIM_OUTPUTPOLARITY_HIGH;
  hlptim1.Init.UpdateMode = LPTIM_UPDATE_ENDOFPERIOD;
  hlptim1.Init.CounterSource = LPTIM_COUNTERSOURCE_INTERNAL;
  hlptim1.Init.Input1Source = LPTIM_INPUT1SOURCE_GPIO;
  hlptim1.Init.Input2Source = LPTIM_INPUT2SOURCE_GPIO;
  if (HAL_LPTIM_Init(&hlptim1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPTIM1_Init 2 */

  /* USER CODE END LPTIM1_Init 2 */

  HAL_NVIC_SetPriority(LPTIM1_IRQn, 2, 2);
  HAL_NVIC_EnableIRQ(LPTIM1_IRQn);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
