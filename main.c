#include <cstdlib>
#include <stdio.h>
#include "can.h"
#include "MDR32Fx.h"
#include "timers.h"
#include "adcrt.h"
#include "angleService.h"
#include "canterminal.h"
#include "loaders.h"
#include "dusFilter.h"


void dbgSendCanTestMessage();

void main()
{
  timer1_init(500);             // таймер передачи угла по CAN
  delay_ms(200);
  loaders_init();               // инициализация загрузчиков и включение CAN-прерываний
  dusFilter_Init();             // инициализация фильтра ДУС
  timer_setEnable(MDR_TIMER1, 1);
    
  //uint32_t dbgTimer = 0;
  while(1)
  {
    //if(elapsed(&dbgTimer, 10))
    //{
      //extern uint16_t currentAngleX16;
      //extern uint32_t rawDusCode;
      //canTerminal_printf("%.4x", currentAngleX16);
      //canTerminal_printf("%.8x", rawDusCode);
    //}     
  }
}

// Передача угла
void Timer1_IRQHandler()
{
  MDR_TIMER1->STATUS &= ~TIMER_STATUS_CNT_ARR_EVENT;                            // сбросить флаг
  
  // Закомментировано для отладки
  uint32_t angle18 = angle_nextSample();
  angleService_sendCurrentAngle(angle18);
  
  // Загрузка шины CAN
  //dbgSendCanTestMessage();
  ///static uint8_t buf[] = {1,2,3,4};
  ///can2_putDataInBuf(CAN_TEST1_TX_BUF, 0xAA, buf, sizeof(buf));
}

// Отладка, загрузить шину CAN
void dbgSendCanTestMessage()
{
  static uint8_t frame1[8];
  static uint8_t frame2[8];
  static uint8_t frame3[8];
  static uint8_t CAN_ID1 = 0x11;
  static uint8_t CAN_ID2 = 0x12;
  static uint8_t CAN_ID3 = 0x13;
  static int val1 = 0;
  static int val2 = 0;
  static int val3 = 0;    

  //static uint32_t time = 0;
  //if(!elapsed(&time, 1)) return;
      
  if(!CAN_TX_REQ(MDR_CAN2, CAN_TEST1_TX_BUF))
  {
    val1++;
    frame1[0] = 1;
    frame1[4] = (uint8_t)(val1>>24);
    frame1[5] = (uint8_t)(val1>>16);
    frame1[6] = (uint8_t)(val1>>8);
    frame1[7] = (uint8_t)(val1);
    can2_putDataInBuf(CAN_TEST1_TX_BUF, CAN_ID1, frame1, sizeof(frame1));    
  }
  
  if(!CAN_TX_REQ(MDR_CAN2, CAN_TEST2_TX_BUF))
  {    
    val2++;
    frame2[0] = 2;
    frame2[4] = (uint8_t)(val2>>24);
    frame2[5] = (uint8_t)(val2>>16);
    frame2[6] = (uint8_t)(val2>>8);
    frame2[7] = (uint8_t)(val2);
    can2_putDataInBuf(CAN_TEST2_TX_BUF, CAN_ID2, frame2, sizeof(frame2));    
  }
  
  return;
  if(!CAN_TX_REQ(MDR_CAN2, CAN_TEST3_TX_BUF))
  {
    val3++;
    frame3[0] = 3;
    frame3[4] = (uint8_t)(val3>>24);
    frame3[5] = (uint8_t)(val3>>16);
    frame3[6] = (uint8_t)(val3>>8);
    frame3[7] = (uint8_t)(val3);
    can2_putDataInBuf(CAN_TEST3_TX_BUF, CAN_ID3, frame3, sizeof(frame3));    
  }
}
