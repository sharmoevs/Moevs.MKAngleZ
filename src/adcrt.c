#include "adcrt.h"
#include "timers.h"
#include "mkpinout.h"


uint32_t __adcrt_putTogetherAngleData();

// Собрать 18-разрядный код угла с порта
uint32_t __adcrt_putTogetherAngleData()
{
  uint32_t portf = MDR_PORTF->RXTX;
  uint32_t portc = MDR_PORTC->RXTX;
  uint32_t portb = MDR_PORTB->RXTX;
  uint32_t data = (portf & 0x3) |
                  (portc & 0x3)<<2 |
                  ((portb>>5)&0x3F)<<4 |
                  (MDR_PORTA->RXTX & 0xFF)<<10;
  return data;
}

// Получить угол
uint32_t _adcrt_read()
{
  ADC_ANGLE_SET_RDYA;
  while(!ADC_ANGLE_ERDA);
  uint32_t data = __adcrt_putTogetherAngleData();
  ADC_ANGLE_CLR_RDYA;
  return data;
}


// Отладка. Преобразовать код угла (uint32_t) в угол (float)
float dbg_angle_convertToFloat(uint32_t code)
{
  float angleF = (float)(code*360.0/0x3FFFF);
  return angleF;
}
