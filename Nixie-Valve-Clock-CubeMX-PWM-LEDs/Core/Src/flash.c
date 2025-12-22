#include "system.h"

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


