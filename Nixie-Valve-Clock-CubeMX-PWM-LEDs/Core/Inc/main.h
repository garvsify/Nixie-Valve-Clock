/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"
#include "custom_callbacks.h"
#include "multiplexer.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
void RTC_Time_Init(void);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

extern RTC_HandleTypeDef hrtc;
extern TIM_HandleTypeDef htim1; //valve LEDs
extern TIM_HandleTypeDef htim2; //rotary encoder
extern TIM_HandleTypeDef htim3; //rotary encoder LEDs
extern TIM_HandleTypeDef htim14; //Time adjust timer (for blinking valves being adjusted)
extern TIM_HandleTypeDef htim16; //anti-cathode poisoning timer
extern TIM_HandleTypeDef htim17; //multiplexer timer
extern UART_HandleTypeDef huart2; //VCP

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define GPIO_Output_BUZZER_Pin GPIO_PIN_9
#define GPIO_Output_BUZZER_GPIO_Port GPIOB
#define T_NRST_Pin GPIO_PIN_2
#define T_NRST_GPIO_Port GPIOF
#define TIM2_CH1_ENC_A_Pin GPIO_PIN_0
#define TIM2_CH1_ENC_A_GPIO_Port GPIOA
#define TIM2_CH2_ENC_B_Pin GPIO_PIN_1
#define TIM2_CH2_ENC_B_GPIO_Port GPIOA
#define T_VCP_TX_Pin GPIO_PIN_2
#define T_VCP_TX_GPIO_Port GPIOA
#define T_VCP_RX_Pin GPIO_PIN_3
#define T_VCP_RX_GPIO_Port GPIOA
#define GPIO_Output_AS_0_Pin GPIO_PIN_4
#define GPIO_Output_AS_0_GPIO_Port GPIOA
#define GPIO_Output_AS_1_Pin GPIO_PIN_5
#define GPIO_Output_AS_1_GPIO_Port GPIOA
#define GPIO_Output_AS_2_Pin GPIO_PIN_6
#define GPIO_Output_AS_2_GPIO_Port GPIOA
#define GPIO_Output_AS_3_Pin GPIO_PIN_7
#define GPIO_Output_AS_3_GPIO_Port GPIOA
#define TIM3_CH3_ENC_RED_LED_Pin GPIO_PIN_0
#define TIM3_CH3_ENC_RED_LED_GPIO_Port GPIOB
#define GPIO_EXTI1_FAULT_Pin GPIO_PIN_1
#define GPIO_EXTI1_FAULT_GPIO_Port GPIOB
#define GPIO_EXTI1_FAULT_EXTI_IRQn EXTI0_1_IRQn
#define GPIO_Output__HV_SHDN_Pin GPIO_PIN_2
#define GPIO_Output__HV_SHDN_GPIO_Port GPIOB
#define TIM1_CH1_T_L_SW_0_Pin GPIO_PIN_8
#define TIM1_CH1_T_L_SW_0_GPIO_Port GPIOA
#define TIM1_CH2_T_L_SW_1_Pin GPIO_PIN_9
#define TIM1_CH2_T_L_SW_1_GPIO_Port GPIOA
#define TIM3_CH1_ENC_GRN_LED_Pin GPIO_PIN_6
#define TIM3_CH1_ENC_GRN_LED_GPIO_Port GPIOC
#define TIM1_CH3_T_L_SW_2_Pin GPIO_PIN_10
#define TIM1_CH3_T_L_SW_2_GPIO_Port GPIOA
#define GPIO_Output_AS_4_Pin GPIO_PIN_11
#define GPIO_Output_AS_4_GPIO_Port GPIOA
#define GPIO_Output_AS_5_Pin GPIO_PIN_12
#define GPIO_Output_AS_5_GPIO_Port GPIOA
#define T_JTMS_Pin GPIO_PIN_13
#define T_JTMS_GPIO_Port GPIOA
#define T_JTCK_Pin GPIO_PIN_14
#define T_JTCK_GPIO_Port GPIOA
#define GPIO_EXTI15_SW_Pin GPIO_PIN_15
#define GPIO_EXTI15_SW_GPIO_Port GPIOA
#define GPIO_EXTI15_SW_EXTI_IRQn EXTI4_15_IRQn
#define GPIO_Output_IN_3_0_Pin GPIO_PIN_3
#define GPIO_Output_IN_3_0_GPIO_Port GPIOB
#define GPIO_Output_IN_3_1_Pin GPIO_PIN_4
#define GPIO_Output_IN_3_1_GPIO_Port GPIOB
#define GPIO_Output_BCD1_Pin GPIO_PIN_5
#define GPIO_Output_BCD1_GPIO_Port GPIOB
#define GPIO_Output_BCD2_Pin GPIO_PIN_6
#define GPIO_Output_BCD2_GPIO_Port GPIOB
#define GPIO_Output_BCD3_Pin GPIO_PIN_7
#define GPIO_Output_BCD3_GPIO_Port GPIOB
#define GPIO_Output_BCD0_Pin GPIO_PIN_8
#define GPIO_Output_BCD0_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
