#ifndef __MAIN_H
#define __MAIN_H

#include "system.h"

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

#endif /* __MAIN_H */
