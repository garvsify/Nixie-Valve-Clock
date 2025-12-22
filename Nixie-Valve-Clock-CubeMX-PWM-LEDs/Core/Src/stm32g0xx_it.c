#include "system.h"
#include "stm32g0xx_it.h"

void NMI_Handler(void)
{
	while(1){}
}

void HardFault_Handler(void)
{
	while(1){}
}

void SVC_Handler(void)
{

}

void PendSV_Handler(void)
{

}

void SysTick_Handler(void)
{
	HAL_IncTick();
}

void EXTI0_1_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_EXTI1_FAULT_Pin);
}

void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart2);
}

void EXTI4_15_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_EXTI15_SW_Pin);
}

void TIM14_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim14);
}

void TIM16_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim16);
}

void TIM17_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim17);
}

void RTC_TAMP_IRQHandler(void)
{
    HAL_RTC_AlarmIRQHandler(&hrtc);
}

void LPTIM1_IRQHandler(void)
{
	HAL_LPTIM_IRQHandler(&hlptim1);
}
