#ifndef __ANGLE_SERVICE_H__
#define __ANGLE_SERVICE_H__

#include "MDR32Fx.h"



#define MAX_TANGAGE_SYSANGLE_CODE      0x3FFFF  // максимальное значение угла тангажного контура (18 бит)




// Фильтр ДУС
//#define DUS_FILTER_NONE
//#define DUS_FILTER_F2kHz_B200Hz_500Hz_20dB_80dB
//#define DUS_FILTER_F2kHz_B200Hz_SPECIFY_ORDER_2
#define DUS_FILTER_F2kHz_B200Hz_500Hz_1dB_60dB                  // *
//#define DUS_FILTER_F2kHz_B500Hz_700Hz_1dB_60dB
//#define DUS_PID_FILTER_F2kHz_B200Hz_700Hz_2dB_26dB






void angleService_sendCurrentAngle(uint32_t angleCode);
uint32_t angle_nextSample();





#endif //__ANGLE_SERVICE_H__