################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/LEDs.c \
../Core/Src/alarm.c \
../Core/Src/anti_cathode_poisoning.c \
../Core/Src/custom_callbacks.c \
../Core/Src/flash.c \
../Core/Src/main.c \
../Core/Src/multiplexer.c \
../Core/Src/rotary_encoder.c \
../Core/Src/stm32g0xx_hal_msp.c \
../Core/Src/stm32g0xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32g0xx.c \
../Core/Src/time.c \
../Core/Src/timers.c \
../Core/Src/utility.c \
../Core/Src/valves.c 

OBJS += \
./Core/Src/LEDs.o \
./Core/Src/alarm.o \
./Core/Src/anti_cathode_poisoning.o \
./Core/Src/custom_callbacks.o \
./Core/Src/flash.o \
./Core/Src/main.o \
./Core/Src/multiplexer.o \
./Core/Src/rotary_encoder.o \
./Core/Src/stm32g0xx_hal_msp.o \
./Core/Src/stm32g0xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32g0xx.o \
./Core/Src/time.o \
./Core/Src/timers.o \
./Core/Src/utility.o \
./Core/Src/valves.o 

C_DEPS += \
./Core/Src/LEDs.d \
./Core/Src/alarm.d \
./Core/Src/anti_cathode_poisoning.d \
./Core/Src/custom_callbacks.d \
./Core/Src/flash.d \
./Core/Src/main.d \
./Core/Src/multiplexer.d \
./Core/Src/rotary_encoder.d \
./Core/Src/stm32g0xx_hal_msp.d \
./Core/Src/stm32g0xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32g0xx.d \
./Core/Src/time.d \
./Core/Src/timers.d \
./Core/Src/utility.d \
./Core/Src/valves.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -DUSE_HAL_DRIVER -DSTM32G031xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/LEDs.cyclo ./Core/Src/LEDs.d ./Core/Src/LEDs.o ./Core/Src/LEDs.su ./Core/Src/alarm.cyclo ./Core/Src/alarm.d ./Core/Src/alarm.o ./Core/Src/alarm.su ./Core/Src/anti_cathode_poisoning.cyclo ./Core/Src/anti_cathode_poisoning.d ./Core/Src/anti_cathode_poisoning.o ./Core/Src/anti_cathode_poisoning.su ./Core/Src/custom_callbacks.cyclo ./Core/Src/custom_callbacks.d ./Core/Src/custom_callbacks.o ./Core/Src/custom_callbacks.su ./Core/Src/flash.cyclo ./Core/Src/flash.d ./Core/Src/flash.o ./Core/Src/flash.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/multiplexer.cyclo ./Core/Src/multiplexer.d ./Core/Src/multiplexer.o ./Core/Src/multiplexer.su ./Core/Src/rotary_encoder.cyclo ./Core/Src/rotary_encoder.d ./Core/Src/rotary_encoder.o ./Core/Src/rotary_encoder.su ./Core/Src/stm32g0xx_hal_msp.cyclo ./Core/Src/stm32g0xx_hal_msp.d ./Core/Src/stm32g0xx_hal_msp.o ./Core/Src/stm32g0xx_hal_msp.su ./Core/Src/stm32g0xx_it.cyclo ./Core/Src/stm32g0xx_it.d ./Core/Src/stm32g0xx_it.o ./Core/Src/stm32g0xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32g0xx.cyclo ./Core/Src/system_stm32g0xx.d ./Core/Src/system_stm32g0xx.o ./Core/Src/system_stm32g0xx.su ./Core/Src/time.cyclo ./Core/Src/time.d ./Core/Src/time.o ./Core/Src/time.su ./Core/Src/timers.cyclo ./Core/Src/timers.d ./Core/Src/timers.o ./Core/Src/timers.su ./Core/Src/utility.cyclo ./Core/Src/utility.d ./Core/Src/utility.o ./Core/Src/utility.su ./Core/Src/valves.cyclo ./Core/Src/valves.d ./Core/Src/valves.o ./Core/Src/valves.su

.PHONY: clean-Core-2f-Src

