#ifndef __FLASH_USER_DATA_H__
#define __FLASH_USER_DATA_H__

#include "MDR32Fx.h"
#include "flash1986ve9x.h"


#define DEVICE_DESCRIPTION_MAX_LEN              256


#define USER_DATA_BASE_FLASH_ADDR       GET_FLASH1986VE9x_PAGE_ADDR(27)         // базовый адрес во флешь-памяти для пользовательских настроек
//#define HARDWARE_ZERO_ANGLE_ADDR        (USER_DATA_BASE_FLASH_ADDR + 0)         // значения сдвигов по X и Y




void readUserDataFromFlash();
void saveUserDataInFlash();


#endif //__FLASH_USER_DATA_H__