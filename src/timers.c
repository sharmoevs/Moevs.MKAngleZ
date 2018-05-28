#include "timers.h"

volatile uint32_t system_time = 0;   // системное время


// Инициализация Timer1
void timer1_init(uint16_t us)
{
    MDR_RST_CLK->PER_CLOCK |= 1<<14;                              // тактирование TIMER2    
    MDR_RST_CLK->TIM_CLOCK |= 0x3<<0 |                            // HCLCK/8 - такктирование от 10MHz
                              1<<24;                              // разрешение тактирование таймера
            
    MDR_TIMER1->CNT = 0;
    MDR_TIMER1->PSG = 10-1;                                       // 1000000 kHz
    MDR_TIMER1->ARR = us;                                         // раз в msc миллисекунд при f=80MHz
    
    MDR_TIMER1->IE |= TIMER_IE_CNT_ARR_EVENT_IE;  
    NVIC_EnableIRQ(Timer1_IRQn);
}

// Включить/Выключить таймер
void timer_setEnable(MDR_TIMER_TypeDef *timer, uint8_t enable)
{
    if(enable) timer->CNTRL |= TIMER_CNTRL_CNT_EN;
    else timer->CNTRL &= ~(TIMER_CNTRL_CNT_EN);
}

// ================================ System timer ===============================
// Инициализация TIMER_2 в качестве системного таймера. T = 1c
void sysTimer_init()
{      
    MDR_RST_CLK->PER_CLOCK |= 1<<15;                              // тактирование TIMER2    
    MDR_RST_CLK->TIM_CLOCK |= 0x3<<8 |                            // HCLCK/8 - такктирование от 10MHz
                              1<<25;                              // разрешение тактирование таймера
    
    MDR_TIMER2->CNT = 0;
    MDR_TIMER2->PSG = 10000-1;                                    // 1000 Hz
    MDR_TIMER2->ARR = 1;                                          // T = 1мс при f=80MHz    
    MDR_TIMER2->IE = TIMER_IE_CNT_ARR_EVENT_IE;                   // прерывание при CNT == ARR       
    MDR_TIMER2->CNTRL |= TIMER_CNTRL_CNT_EN;                      // разрешение работы таймера 
    NVIC_EnableIRQ(Timer2_IRQn);        
}

// Прерывание системного таймера
void Timer2_IRQHandler(void)
{
   MDR_TIMER2->STATUS &= ~TIMER_STATUS_CNT_ARR_EVENT;                           // сбросить флаг
   MDR_TIMER2->CNT = 0;
   system_time++;
}


// ================================== Delay ====================================
// Миллиссекундная и микросекундная задержка на основе таймера общего назначеня
// Инициализация Timer3. Т = 1 мкс
void delayTimer_init()
{      
   MDR_RST_CLK->PER_CLOCK |= 1<<16;                              // тактирование TIMER2    
   MDR_RST_CLK->TIM_CLOCK |= 0x3<<16 |                           // HCLCK/8 - такктирование от 10MHz
                             1<<26;                              // разрешение тактирование таймера
            
   MDR_TIMER3->CNT = 0;
   MDR_TIMER3->PSG = 10-1;                                       // 1000000 kHz
   MDR_TIMER3->ARR = 0xFFFF;                                     // раз в msc миллисекунд при f=80MHz
    
   timer_setEnable(MDR_TIMER3, 1);
}

// Задержка в итерациях. При HCLK=80МГц, Т=12.5нс
void delay_tics(uint32_t tics)
{ 
   for(volatile uint32_t i=0; i<tics; i++);
}

// Задержка на mcs микросекунд
__ramfunc void delay_mcs(uint16_t mcs)
{    
   // TimFreq = 10МГц
   MDR_TIMER3->PSG = 10-1;              // 1 MHz  T=1мкс
   MDR_TIMER3->CNT = 0;    
   while(MDR_TIMER3->CNT < mcs);
}

// Задержка на ms миллисекунд
__ramfunc void delay_ms(uint16_t ms)
{
   // TimFreq = 10МГц
   MDR_TIMER3->PSG = 10000-1;           // 1000 Hz  T=1мс
   MDR_TIMER3->CNT = 0;
   while(MDR_TIMER3->CNT < ms);
}


// Возвращает 1, если прошло delay_ms от момента *var, относительно системного времени
__ramfunc uint8_t elapsed(uint32_t *var, const uint32_t delay_ms) 
{
   if ((system_time - *var) >= delay_ms) 
   {
      *var = system_time; // сброс
      return 1;
   }
   else return 0;
}

