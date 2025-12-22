#include "system.h"

void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
}

void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(hrtc->Instance==RTC)
  {
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;

    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_RTC_ENABLE();
    __HAL_RCC_RTCAPB_CLK_ENABLE();
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* hrtc)
{
  if(hrtc->Instance==RTC)
  {
    __HAL_RCC_RTC_DISABLE();
    __HAL_RCC_RTCAPB_CLK_DISABLE();
  }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(htim_base->Instance==TIM1)
  {
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_TIM1;
    PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_RCC_TIM1_CLK_ENABLE();
  }
  else if(htim_base->Instance==TIM3)
  {
    __HAL_RCC_TIM3_CLK_ENABLE();
  }
  else if(htim_base->Instance==TIM14)
  {
    __HAL_RCC_TIM14_CLK_ENABLE();

    /* TIM14 interrupt Init */
    HAL_NVIC_SetPriority(TIM14_IRQn, VALVE_BLINK_TIMER_INTERRUPT_PRIORITY, VALVE_BLINK_TIMER_INTERRUPT_PRIORITY);
    HAL_NVIC_EnableIRQ(TIM14_IRQn);
  }
  else if(htim_base->Instance==TIM16)
  {
    /* Peripheral clock enable */
    __HAL_RCC_TIM16_CLK_ENABLE();

    /* TIM16 interrupt Init */
    HAL_NVIC_SetPriority(TIM16_IRQn, ANTI_CATHODE_POISONING_TIMER_INTERRUPT_PRIORITY, ANTI_CATHODE_POISONING_TIMER_INTERRUPT_PRIORITY);
    HAL_NVIC_EnableIRQ(TIM16_IRQn);
  }
  else if(htim_base->Instance==TIM17)
  {
    /* Peripheral clock enable */
    __HAL_RCC_TIM17_CLK_ENABLE();

    /* TIM17 interrupt Init */
    HAL_NVIC_SetPriority(TIM17_IRQn, MULTIPLEXER_TIMER_INTERRUPT_PRIORITY, MULTIPLEXER_TIMER_INTERRUPT_PRIORITY);
    HAL_NVIC_EnableIRQ(TIM17_IRQn);
  }
}

void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef* htim_encoder)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(htim_encoder->Instance==TIM2)
  {
    /* Peripheral clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();

    //TIM2 GPIO Configuration
    GPIO_InitStruct.Pin = TIM2_CH1_ENC_A_Pin|TIM2_CH2_ENC_B_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  if(htim->Instance==TIM1)
  {
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /**TIM1 GPIO Configuration
    PA8     ------> TIM1_CH1
    PA9     ------> TIM1_CH2
    PA10     ------> TIM1_CH3
    */
    GPIO_InitStruct.Pin = TIM1_CH1_T_L_SW_0_Pin|TIM1_CH2_T_L_SW_1_Pin|TIM1_CH3_T_L_SW_2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }

  else if(htim->Instance==TIM3)
  {
      __HAL_RCC_GPIOB_CLK_ENABLE();
      __HAL_RCC_GPIOC_CLK_ENABLE();

      /**TIM3 GPIO Configuration
      PB0     ------> TIM3_CH3
      PC6     ------> TIM3_CH1
      */
      GPIO_InitStruct.Pin = TIM3_CH3_ENC_RED_LED_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      GPIO_InitStruct.Alternate = GPIO_AF1_TIM3;
      HAL_GPIO_Init(TIM3_CH3_ENC_RED_LED_GPIO_Port, &GPIO_InitStruct);

      GPIO_InitStruct.Pin = TIM3_CH1_ENC_GRN_LED_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      GPIO_InitStruct.Alternate = GPIO_AF1_TIM3;
      HAL_GPIO_Init(TIM3_CH1_ENC_GRN_LED_GPIO_Port, &GPIO_InitStruct);

    }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==TIM1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_TIM1_CLK_DISABLE();
  }
  else if(htim_base->Instance==TIM3)
  {
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();
  }
  else if(htim_base->Instance==TIM14)
  {
    /* Peripheral clock disable */
    __HAL_RCC_TIM14_CLK_DISABLE();

    /* TIM14 interrupt DeInit */
    HAL_NVIC_DisableIRQ(TIM14_IRQn);
  }
  else if(htim_base->Instance==TIM16)
  {
    /* Peripheral clock disable */
    __HAL_RCC_TIM16_CLK_DISABLE();

    /* TIM16 interrupt DeInit */
    HAL_NVIC_DisableIRQ(TIM16_IRQn);
  }
  else if(htim_base->Instance==TIM17)
  {
    /* Peripheral clock disable */
    __HAL_RCC_TIM17_CLK_DISABLE();

    /* TIM17 interrupt DeInit */
    HAL_NVIC_DisableIRQ(TIM17_IRQn);
  }
}

void HAL_TIM_Encoder_MspDeInit(TIM_HandleTypeDef* htim_encoder)
{
  if(htim_encoder->Instance==TIM2)
  {
    /* Peripheral clock disable */
    __HAL_RCC_TIM2_CLK_DISABLE();

    /**TIM2 GPIO Configuration
    PA0     ------> TIM2_CH1
    PA1     ------> TIM2_CH2
    */
    HAL_GPIO_DeInit(GPIOA, TIM2_CH1_ENC_A_Pin|TIM2_CH2_ENC_B_Pin);
  }
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(huart->Instance==USART2)
  {
    /* Peripheral clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = T_VCP_TX_Pin|T_VCP_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(USART2_IRQn, UART_INTERRUPT_PRIORITY, UART_INTERRUPT_PRIORITY);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
  if(huart->Instance==USART2)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, T_VCP_TX_Pin|T_VCP_RX_Pin);
  }
}

void HAL_LPTIM_MspInit(LPTIM_HandleTypeDef* hlptim)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  if(hlptim->Instance==LPTIM1)
  {
  /** Initializes the peripherals clocks
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_LPTIM1;
    PeriphClkInit.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_LPTIM1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
  }
  else if(hlptim->Instance==LPTIM2)
  {
  /** Initializes the peripherals clocks
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_LPTIM2;
    PeriphClkInit.Lptim2ClockSelection = RCC_LPTIM2CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_LPTIM2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();

    /**LPTIM2 GPIO Configuration
    PA8     ------> LPTIM2_OUT
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_LPTIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}

void HAL_LPTIM_MspDeInit(LPTIM_HandleTypeDef* hlptim)
{
  if(hlptim->Instance==LPTIM1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_LPTIM1_CLK_DISABLE();

    /**LPTIM1 GPIO Configuration
    PB0     ------> LPTIM1_OUT
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0);
  }
}
