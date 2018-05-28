#include "flash1986ve9x.h"
#include "timers.h"


// Стирание одной страницы основной флешь-памяти
__ramfunc void erase_flash_page(uint32_t addr)
{
   uint32_t enable_irq = NVIC->ISER[0];         // сохранение разрешенных прерываний 
   NVIC->ICER[0]  = 0xFFFFFFFF;                 // запрет всех прерываний

   MDR_EEPROM->KEY = EEPROM_KEY;   
   MDR_EEPROM->CMD |= 1<<CON;           // переключаемся в режим программирования
   MDR_EEPROM->CMD &= ~(1<<IFREN);      // выбор памяти (1 - для информационной, 0 - для основной)
   
   MDR_EEPROM->ADR = addr;
   
   for(uint8_t i=0; i<4; i++)           // стирание всей памяти для секторов A,B,C,D
   {
      MDR_EEPROM->ADR &= ~(0x3<<2);     // сброс номера сектора
      MDR_EEPROM->ADR |= i<<2;          // номер сектора (0-A, 1-B, 2-C, 3-D)
      
      MDR_EEPROM->CMD |= 1<<XE | 1<<ERASE;      
      delay_mcs(5);                     // задержка на 5мкс      
      MDR_EEPROM->CMD |= 1<<NVSTR;      
      delay_ms(40);                     // задержка на 40мс - полное стирание памяти
      MDR_EEPROM->CMD &= ~(1<<ERASE);   
      delay_mcs(5);                     // задержка на 5мкс      
      MDR_EEPROM->CMD &= ~(1<<XE | 1<<NVSTR);      
      delay_mcs(1);                     // задержка на 1мкс    
   }

   MDR_EEPROM->CMD &= ~(1<<CON);        // выход из режима программирования
   MDR_EEPROM->KEY = 0;

   NVIC->ISER[0] = enable_irq;          // разрешение прерываний
}

// Записать 32б слово data по адресу addr в основную флеш-память
__ramfunc void flash_write_word(uint32_t addr, uint32_t data)
{   
   uint32_t enable_irq = NVIC->ISER[0];         // сохранение разрешенных прерываний 
   NVIC->ICER[0]  = 0xFFFFFFFF;                 // запрет всех прерываний
   
   MDR_EEPROM->KEY = EEPROM_KEY;   
   MDR_EEPROM->CMD |= 1<<CON;                   // переключаемся в режим программирования
   MDR_EEPROM->CMD &= ~(1<<IFREN);              // основная память  
   
   MDR_EEPROM->ADR = addr;
   MDR_EEPROM->DI = data;
   
   MDR_EEPROM->CMD |= 1<<XE | 1<<PROG;
   delay_mcs(5);                                // задержка на 5мкс    
   MDR_EEPROM->CMD |= 1<<NVSTR;       
   delay_mcs(10);                               // задержка на 10мкс    
   MDR_EEPROM->CMD |= 1<<YE;      
   delay_mcs(40);                               // задержка на 40мкс  Запись в память
   MDR_EEPROM->CMD &= ~(1<<YE);      
   delay_mcs(1);                                // задержка на 20нс    
   MDR_EEPROM->CMD &= ~(1<<PROG);      
   delay_mcs(5);                                // задержка на 5мкс    
   MDR_EEPROM->CMD &= ~(1<<XE | 1<<NVSTR);      
   delay_mcs(1);                                // задержка на 1мкс    
      
   MDR_EEPROM->CMD &= ~(1<<CON | 1<<IFREN);     // выход из режима программирования                          
   MDR_EEPROM->KEY = 0;
      
   NVIC->ISER[0] = enable_irq;          // разрешение прерываний
}



/*
// Записать массив из cnt слов в основную флеш-память по адресу addr
__ramfunc void flash_write_array(uint32_t addr, uint32_t *pData, uint16_t cnt)
{  
   uint32_t enable_irq = NVIC->ISER[0];         // сохранение разрешенных прерываний 
   NVIC->ICER[0]  = 0xFFFFFFFF;                 // запрет всех прерываний
   
   MDR_EEPROM->KEY = EEPROM_KEY;   
   MDR_EEPROM->CMD |= 1<<CON;                   // переключаемся в режим программирования
   MDR_EEPROM->CMD &= ~(1<<IFREN);              // основная память  
   
   MDR_EEPROM->ADR = addr;
   //MDR_EEPROM->DI = *pData++;
   //addr+=4;
   
   MDR_EEPROM->CMD |= 1<<XE | 1<<PROG;
   delay_mcs(5);                                // задержка на 5мкс    
   MDR_EEPROM->CMD |= 1<<NVSTR;       
   delay_mcs(10);                               // задержка на 10мкс    
   //MDR_EEPROM->CMD |= 1<<YE;      
   //delay_mcs(40);                               // задержка на 40мкс  Запись в память
   //MDR_EEPROM->CMD &= ~(1<<YE);    
   //delay_mcs(1);                                // задержка на 20нс      
   
   for(uint16_t i=0; i<cnt-1; i++)
   {
      MDR_EEPROM->ADR = addr;
      MDR_EEPROM->DI = *pData++;
      addr+=4;
      
      MDR_EEPROM->CMD |= 1<<YE;      
      delay_mcs(40);                               // задержка на 40мкс  Запись в память
      MDR_EEPROM->CMD &= ~(1<<YE);        
      delay_mcs(1);                                // задержка на 20нс 
   }
   
   MDR_EEPROM->CMD &= ~(1<<PROG);      
   delay_mcs(5);                                // задержка на 5мкс    
   MDR_EEPROM->CMD &= ~(1<<XE | 1<<NVSTR);      
   delay_mcs(1);                                // задержка на 1мкс    
      
   MDR_EEPROM->CMD &= ~(1<<CON | 1<<IFREN);     // выход из режима программирования                          
   MDR_EEPROM->KEY = 0;
      
   NVIC->ISER[0] = enable_irq;          // разрешение прерываний
}
*/