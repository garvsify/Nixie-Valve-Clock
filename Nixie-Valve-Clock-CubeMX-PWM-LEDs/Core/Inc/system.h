#ifndef INC_SYSTEM_H_
#define INC_SYSTEM_H_

#include <stdint.h>
#include "stm32g031xx.h"
#include "stm32g0xx_hal.h"
#include "defines.h"

enum Blink_State{

	BLINK_ON,
	BLINK_OFF
};

struct Time_Adjust{

	volatile RTC_TimeTypeDef adjust_time;
	volatile uint8_t Hours_Bin;
	volatile uint8_t Minutes_Bin;
	volatile uint8_t Seconds_Bin;
};

struct Alarm{

	volatile uint8_t alarm_triggered;
	volatile uint8_t alarm_set;
	volatile uint32_t alarm_counter;
	volatile RTC_TimeTypeDef alarm_time;
	volatile uint8_t Hours_Bin;
	volatile uint8_t Minutes_Bin;
	volatile uint8_t Seconds_Bin;
};

enum System_Mode{

	NONE,
	NORMAL_MODE,
	ANTI_CATHODE_POISONING_MODE,
	HH_ADJUST_MODE,
	MM_ADJUST_MODE,
	SS_ADJUST_MODE,
	VALVES_OFF_MODE,
	ALARM_SET_HH_MODE,
	ALARM_SET_MM_MODE,
	ALARM_SET_SS_MODE,
};

struct System_Mode_Tracker{

	volatile enum System_Mode current_mode;
	volatile enum System_Mode previous_mode;
};

struct Anti_Cathode_Poisoning{

	volatile uint8_t counter;
	uint8_t max_counter;
	volatile uint8_t cycle;
	uint8_t max_cycles;
};

struct Software_Timer{

	uint32_t count;
	uint32_t max_count;
	uint8_t enabled;
};

struct Calibration{

	volatile uint8_t CALP;
	volatile uint16_t CALM;
};

struct LEDs{

	volatile uint32_t LED_counter[NUM_ROTARY_ENCODER_LEDs];
	volatile uint8_t Double_Flash_Red_LED;
};

enum Rotary_Encoder_Switch_State{

	ROTARY_ENCODER_SWITCH_STATE_NOT_DEPRESSED,
	ROTARY_ENCODER_SWITCH_STATE_DEPRESSED,
};

struct Rotary_Encoder_Switch_States{

	volatile enum Rotary_Encoder_Switch_State rotary_encoder_switch_state;
	volatile enum Rotary_Encoder_Switch_State rotary_encoder_switch_prev_state;
};

struct Master{

	struct Anti_Cathode_Poisoning anti_cathode_poisoning;

	struct System_Mode_Tracker system_mode_tracker;

	struct Time_Adjust time_adjust;
	volatile RTC_TimeTypeDef get_time;
	volatile RTC_DateTypeDef get_date;

	struct Alarm alarm;

	struct Software_Timer software_timers[1]; //not yet used

	uint32_t encoder_first;
	uint32_t encoder_second;
	struct Rotary_Encoder_Switch_States rotary_encoder_switch_states;

	volatile uint8_t valve_blink_state;

	struct LEDs leds;

	volatile uint32_t depressed_num_monitor;

	volatile uint8_t RX_buffer[COMMAND_PLUS_ARGUMENT_NUM_BYTES];
	volatile uint8_t TX_buffer[RESPONSE_NUM_BYTES];

	volatile uint16_t dead_time;

	struct Calibration calibration;
};

extern GPIO_TypeDef* Valve_Anode_Registers[NUM_VALVES];
extern uint16_t Valve_Anode_Pins[NUM_VALVES];

extern GPIO_TypeDef* BCD_Registers[NUM_BINARY_DIGITS_IN_BCD];
extern uint16_t BCD_Pins[NUM_BINARY_DIGITS_IN_BCD];

extern const uint16_t triangle_wavetable[TRI_WAVETABLE_SIZE];
extern struct Master master;

extern RTC_HandleTypeDef hrtc; //RTC
extern TIM_HandleTypeDef htim1; //valve LEDs
extern TIM_HandleTypeDef htim2; //rotary encoder
extern TIM_HandleTypeDef htim3; //rotary encoder LEDs
extern TIM_HandleTypeDef htim14; //time adjust timer (for blinking valves)
extern TIM_HandleTypeDef htim16; //anti-cathode poisoning timer
extern TIM_HandleTypeDef htim17; //multiplexer timer
extern UART_HandleTypeDef huart2; //VCP
extern LPTIM_HandleTypeDef hlptim1; //rotary encoder switch check

uint8_t System_Init(void);
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_RTC_Init(void);
void MX_TIM2_Init(void);
void MX_TIM3_Init(void);
void MX_USART2_UART_Init(void);
void MX_TIM1_Init(void);
void MX_TIM17_Init(void);
void MX_TIM16_Init(void);
void MX_TIM14_Init(void);
void MX_LPTIM1_Init(void);
void RTC_Time_Init(void);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *TIM);

void Error_Handler(void);

uint8_t Write_Digit_to_Valve(uint8_t valve_num, uint8_t BCD_of_digit);
uint8_t Turn_All_Valves_Off(void);
uint8_t Start_Multiplexer_Timer(void);
uint8_t Start_Anti_Cathode_Poisoning_Timer(void);
uint8_t Start_Adjust_Time_Slash_Alarm_Set_Mode_Timer(void);
uint8_t Stop_Adjust_Time_Slash_Alarm_Set_Mode_Timer(void);
uint8_t Master_Init(struct Master *master);
uint8_t Set_System_Mode_and_Store_Previous_Mode(struct System_Mode_Tracker *system_mode_tracker, enum System_Mode desired_mode);
uint8_t Turn_Valve_Off(uint8_t valve_num);
uint8_t Toggle_HV_Power_Supply(uint8_t toggle);
uint8_t Get_RTC_Time(void);
uint8_t Write_Time_In_Flash(RTC_TimeTypeDef *time);
uint8_t Read_Time_From_Flash(RTC_TimeTypeDef *time);
uint8_t Pack_Time_Into_Doubleword(RTC_TimeTypeDef *time, uint64_t *doubleword);
uint8_t Check_Rotary_Encoder_Switch_State(volatile struct Rotary_Encoder_Switch_States *rotary_encoder_switch_states_ptr);
uint8_t Set_Fault_LED_ON(void);
uint8_t Set_Alarm(uint8_t BCD_HH, uint8_t BCD_MM, uint8_t BCD_SS);
uint8_t Clear_Alarm(void);
uint8_t Set_Adjust_Time_LED_ON(void);
uint8_t Set_Alarm_Set_LEDs_ON(void);
uint8_t Set_Adjust_Time_LED_OFF(void);
uint8_t Set_Alarm_Set_LEDs_OFF(void);
uint8_t Initialise_Rotary_Encoder_LEDs(void);
uint8_t Sound_Alarm(void);
uint8_t Initialise_Valve_LEDs(void);
uint8_t Double_Flash_Red_Rotary_Encoder_LED(void);
uint8_t Check_Rotary_Encoder(void);
uint8_t Start_OC_TIM(TIM_HandleTypeDef *TIM, uint32_t OC_TIM_channel);
uint8_t Stop_OC_TIM(TIM_HandleTypeDef *TIM, uint32_t OC_TIM_channel);

void TIM17_Multiplexer_Sequencer_Callback(TIM_HandleTypeDef *htim);
void TIM16_Anti_Cathode_Poisoning_Callback(TIM_HandleTypeDef *htim);
void TIM14_Valve_Blink_Callback(TIM_HandleTypeDef *htim);
void TIM1_CH1_Valve_LED_0_Callback(TIM_HandleTypeDef *htim);
void TIM1_CH2_Valve_LED_1_Callback(TIM_HandleTypeDef *htim);
void TIM1_CH3_Valve_LED_2_Callback(TIM_HandleTypeDef *htim);
void LPTIM1_Rotary_Encoder_Switch_Callback(LPTIM_HandleTypeDef *hlptim);
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin);
void Alarm_Callback(RTC_HandleTypeDef *hrtc);
void UART2_RX_Transfer_Complete_Callback(UART_HandleTypeDef *huart);
void UART2_TX_Transfer_Complete_Callback(UART_HandleTypeDef *huart);

#endif /* INC_SYSTEM_H_ */
