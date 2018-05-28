#ifndef __GLOBAL_H__
#define __GLOBAL_H__

// Управление переферией на плате формирования углового положения по тангажу


#define ADC_ANGLE_DATA          (((uint32_t)MDR_PORTF->RXTX) & 0x3) |\
                                (((uint32_t)MDR_PORTC->RXTX) & 0x3)<<2 |\
                                (((uint32_t)MDR_PORTB->RXTX) & 0x3F0) |\
                                (((uint32_t)MDR_PORTA->RXTX) & 0xFF)<<10

#define ADC_ANGLE_ERDA          ((MDR_PORTC->RXTX>>2) & 1)

#define ADC_ANGLE_SET_RDYA      (MDR_PORTD->RXTX |= 1<<5)
#define ADC_ANGLE_CLR_RDYA      (MDR_PORTD->RXTX &= ~(1<<5))






#endif // __GLOBAL_H__
