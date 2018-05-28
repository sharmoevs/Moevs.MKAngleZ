#include <stdio.h>
#include "MDR32Fx.h"
#include "settings.h"
#include "mkpinout.h"
#include "timers.h"
#include "can.h"
#include "dac.h"


void rcc_init();
void ports_init();

void SystemInit()
{
   SCB->VTOR = 0x08000000;             // смещение таблицы векторов
  
   rcc_init();                         // настройка тактирования ядра   
   sysTimer_init();                    // системный таймер (1мс)
   dac_init();
   delayTimer_init();                  // инициализация таймера для задержек (мкс, мс)
   ports_init();                       // инициализация портов
   can2_init();                        // инициализация can1
}

// Инициализация тактирования
void rcc_init()
{
    MDR_RST_CLK->CPU_CLOCK = 0;  

    MDR_RST_CLK->HS_CONTROL |= RST_CLK_HS_CONTROL_HSE_ON;                       // Включить внешний генератор
    while(!(MDR_RST_CLK->CLOCK_STATUS & RST_CLK_CLOCK_STATUS_HSE_RDY));         // Ждем 
    MDR_RST_CLK->CPU_CLOCK |= 0x2;                                              // CPU_C1 = HSE

    MDR_RST_CLK->PLL_CONTROL = (CPU_PLL_KOEF<<8);                               // CPU PLL         
    MDR_RST_CLK->PLL_CONTROL |= (1<<2);                                         // CPU PLL On
    while(!(MDR_RST_CLK->CLOCK_STATUS & RST_CLK_CLOCK_STATUS_PLL_CPU_RDY));     // waiting cpu pll ready
    MDR_RST_CLK->CPU_CLOCK |= 1<<2;                                             // CPU_C2 = PLLCPUo

    MDR_RST_CLK->CPU_CLOCK &= ~(1<<7);                                          // CPU_C3 = CPU_C2
    MDR_RST_CLK->CPU_CLOCK |= 1<<8;                                             // HCLK = CPU_C3

    MDR_EEPROM->CMD = 0x3<<3;         // 3 такта паузы при работе на частотах до 100МГц

    MDR_RST_CLK->PER_CLOCK = 1<<3 |   // EEPROM_CNTRL
                             1<<4;    // RST_CLK
}

// Инициализация портов
void ports_init()
{  
    MDR_RST_CLK->PER_CLOCK |= 1<<21 |     // PORTA
                              1<<22 |     // PORTB
                              1<<23 |     // PORTC
                              1<<24 |     // PORTD
                              1<<25 |     // PORTE
                              1<<29;      // PORTF
    
    // Can2, АЦПВТ
    MDR_PORTF->FUNC = 0<<0 |          // N1
                      0<<1 |          // N2
                      0x3<<4  |       // CAN2 RX
                      0x3<<6;         // CAN2 TX
    MDR_PORTF->OE = 0<<0 |
                    0<<1 |
                    0<<2 |        // CAN2 RX  in 
                    1<<3;         // CAN2 TX  out
    MDR_PORTF->ANALOG = 1<<0 | 1<<1 | 1<<2 | 1<<3;
    MDR_PORTF->PWR =  0x3<<0 | 0x3<<2 | 0x3<<4 | 0x3<<6;
    
    // АЦПВТ
    MDR_PORTC->FUNC = 0<<0 |          // N3
                      0<<2 |          // N4
                      0<<4;           // RDYA
    MDR_PORTC->OE = 0<<0 |
                    0<<1 |
                    0<<2;
    MDR_PORTC->ANALOG = 1<<0 | 1<<1 | 1<<2;
    MDR_PORTC->PWR =  0x3<<0 | 0x3<<2 | 0x3<<4;
    
    
    // АЦПВТ
    MDR_PORTB->FUNC = 0<<10 |          // N5
                      0<<12 |          // N6
                      0<<14 |          // N7
                      0<<16 |          // N8
                      0<<18 |          // N9
                      0<<20;           // N10
    MDR_PORTB->OE = 0;
    MDR_PORTB->ANALOG = 1<<5 | 1<<6 | 1<<7 | 1<<8 | 1<<9 | 1<<10;
    MDR_PORTB->PWR =  0x3<<10 | 0x3<<12 | 0x3<<14 | 0x3<<16 | 0x3<<18 | 0x3<<20;
    
    
    // АЦПВТ
    MDR_PORTA->FUNC = 0<<0 |          // N11
                      0<<2 |          // N12
                      0<<4 |          // N13
                      0<<6 |          // N14
                      0<<8 |          // N15
                      0<<10 |         // N16
                      0<<12 |         // N17
                      0<<14;          // N18
    MDR_PORTA->OE = 0;
    MDR_PORTA->ANALOG = 1<<0 | 1<<1 | 1<<2 | 1<<3 | 1<<4 | 1<<5 | 1<<6 | 1<<7;
    MDR_PORTA->PWR =  0x3<<0 | 0x3<<2 | 0x3<<4 | 0x3<<6 | 0x3<<8 | 0x3<<10 | 0x3<<12 | 0x3<<14;
    
        
    // АЦПВТ
    MDR_PORTD->FUNC = 0<<10;          // ERDA
    MDR_PORTD->OE = 1<<5;
    MDR_PORTD->ANALOG = 1<<5;
    MDR_PORTD->PWR = 0x3<<10;
    
    // ЦАП
    MDR_PORTE->FUNC = 0<<0;           // ЦАП
    MDR_PORTE->OE = 1<<0;
    MDR_PORTE->ANALOG = 0<<0;
    MDR_PORTE->PWR = 0x3<<0;
}
