#include "angleService.h"
#include "can.h"
#include "adcrt.h"
#include "dusFilter.h"

uint32_t g_rawSysAngle;      // неотфильтрованный код угла с АЦПВТ
uint32_t g_sysAngle;         // отфильтрованный код с АЦПВТ


uint32_t _dus_filter(uint32_t code);
uint32_t angle_deductCorrection(uint32_t angle, uint32_t deltaFi, uint32_t code360);


void angleService_send(uint8_t *pData, uint8_t len);

// Отправить данные CAN-монитору
void angleService_send(uint8_t *pData, uint8_t len)
{
  ////can2_send_packet(CAN_ANGLE_TX_BUF, CANID_ANGLE_TX, pData, len);  // блокирующая функция 
  can2_putDataInBuf(CAN_ANGLE_TX_BUF, CANID_ANGLE_TX, pData, len);
}

// Отправить угол по CAN
void angleService_sendCurrentAngle(uint32_t angleCode)
{
   uint8_t buf[3];
   buf[0] = (uint8_t)(angleCode>>16);
   buf[1] = (uint8_t)(angleCode>>8);
   buf[2] = (uint8_t)angleCode;
   angleService_send(buf, sizeof(buf));
}



// Получение значения с ДУС и его фильтрация
uint32_t angle_nextSample()
{
   g_rawSysAngle = _adcrt_read();
   g_sysAngle = _dus_filter(g_rawSysAngle);
   return g_sysAngle;
}



// Оцифровать и отфильтровать следующее значение с ДУС
uint32_t _dus_filter(uint32_t code)
{  
  uint32_t filteredCode;
  
#ifdef DUS_FILTER_NONE
  filteredCode = code;                                       // без фильтра
/*
#elif defined DUS_FILTER_F4kHz_B100Hz
  filteredCode = _dus_4kHz_DigitalFilter_100Hz(code);        // F=4kHz b=100Hz
    
#elif defined DUS_FILTER_F4kHz_B200Hz
  filteredCode = _dus_4kHz_DigitalFilter_200Hz(code);        // F=4kHz b=200Hz
  
#elif defined DUS_FILTER_F4kHz_B300Hz
  filteredCode = _dus_4kHz_DigitalFilter_300Hz(code);        // F=4kHz b=300Hz
  
#elif defined DUS_FILTER_F4kHz_B500Hz
  filteredCode = _dus_4kHz_DigitalFilter_500Hz(code);        // F=4kHz b=500Hz
*/
  
  
#elif defined DUS_FILTER_F2kHz_B200Hz_500Hz_20dB_80dB
  filteredCode = _dus_2kHz_DigitalFilter_200Hz_500Hz_20dB_80dB(code);           // F=4kHz b=200Hz
  
#elif defined DUS_FILTER_F2kHz_B200Hz_SPECIFY_ORDER_2
  filteredCode = _dus_2kHz_DigitalFilter_200Hz_order2(code);                    // F=4kHz b=200Hz ver2    P=5!
  
#elif defined DUS_FILTER_F2kHz_B200Hz_500Hz_1dB_60dB
  filteredCode = _dus_2kHz_DigitalFilter_200Hz_500Hz_1dB_60dB(code);            // F=4kHz b=200Hz ver3    P=25
  
#elif defined DUS_FILTER_F2kHz_B500Hz_700Hz_1dB_60dB
  filteredCode = _dus_2kHz_DigitalFilter_500Hz_700Hz_1dB_60dB(code);            // F=4kHz b=500Hz         P=5

#elif defined DUS_PID_FILTER_F2kHz_B200Hz_700Hz_2dB_26dB
  filteredCode = _dus_2kHz_200Hz_700Hz_2dB_26dB(code);                          // F=4kHz b=500Hz         P=5
#endif
   
  return filteredCode;
}



// Вычесть из угла поправку deltaFi
uint32_t angle_deductCorrection(uint32_t angle, uint32_t deltaFi, uint32_t code360)
{
  uint32_t res = (angle >= deltaFi) ? 
                 (angle - deltaFi) :
                 (code360 - (deltaFi - angle));
  return res;
}