#include "canmonitor.h"
#include "flashUserData.h"

       
uint8_t extCmdBuf[255];             // буфер под расширенную команду
uint8_t newDeviceDescription[DEVICE_DESCRIPTION_MAX_LEN];
uint8_t newDeviceDescriptionLen = 0;


// Принят пакет от монитора
void canMonitor_rxFrameHandler(uint8_t *buf, uint8_t len)
{
  switch(buf[0])
  {
     // Запрос состояния 
     case CANMONITOR_STATE_REQUEST:
       canMonitor_sendWorkingTime();            // отправить время наработки
       break;
     
    // Принять расширенную команду
    case CANMONITOR_EXTENDED_CMD:
       canMonitor_rxExtCmd(buf, len);
       break;
            
    // Сброс микроконтроллера
    case CANMONITOR_RESET_CONTROLLER:
      {
        uint32_t key = *((uint32_t*)(buf+1));
        if((len == 5) && (key == 0xDEC0ADDE))
        {
          SYSTEM_RESET();
          while(1);
        }
      }
      break;
      
      /*
    // Сохранить ноль аппаратной шкалы
    case CANMONITOR_SAVE_NEW_HARDWARE_SCALE:
      if(len!=1) return;
      canMonitor_saveNewHardwareScale();
      break;
      */
  }
}

// Отправить данные CAN-монитору
void canMonitor_send(uint8_t *pData, uint8_t len)
{
   can2_send_packet(CAN_MONITOR_TX_BUF, CANID_MONITOR_TX, pData, len);  
}

// Отправить время наработки
void canMonitor_sendWorkingTime()
{ 
  extern volatile uint32_t system_time;          // системное время, мс
  uint32_t currentSystemTime = system_time;
  
  uint8_t msg[5];    
  msg[0] = CANMONITOR_WORKING_TIME_MS;
  msg[1] = currentSystemTime>>24; 
  msg[2] = currentSystemTime>>16;
  msg[3] = currentSystemTime>>8;
  msg[4] = currentSystemTime;
  canMonitor_send(msg, sizeof(msg));
}

// Принять расширенную команду
void canMonitor_rxExtCmd(uint8_t *pData, uint8_t len)
{  
   static uint8_t index=0;
   uint8_t rxDone = 0;          // завершен прием команды
   uint8_t needAnswer = 0;      // необхо
   
   switch(pData[1] & 0xf)       // проверка типа операции
   {
     case EXT_CMD_START: 
       index = 0;       
       break;
     
     case EXT_CMD_CONT: break;
     
     case EXT_CMD_END:
       rxDone = 1;       
       if(pData[1] & 1<<7) needAnswer = 1;
       break;
       
     default: return;
   }
   
   for(uint8_t i=2; i<len; i++) // первые 2 байта служебные
   {
      if(index == sizeof(extCmdBuf)) break; // защита от переполнения буфера
      extCmdBuf[index++] = pData[i];
   }   
   if(rxDone)
   {        
       //PldCmdId_t cmdId = needAnswer ? PLD_CMD_EXTENDED_WITH_ANS : PLD_CMD_EXTENDED_WITHOUT_ANS;
       //enqueque(cmdId, extCmdBuf, index);
       (void)needAnswer;
       index = 0;
   }
}

// Отправить ответ на расширенную команду 
void canMonitor_sendExtAnswer(uint8_t *pBuf, uint8_t len)
{
   uint8_t msg[8];
   uint8_t needToSend = 0;          // сколько байт осталось отправить
   uint8_t sendedBytes = 0;         // кол-во отправленных байт данных
   uint8_t dataBytesInFrame = 0;    // кол-во байт в передаваемом пакете
   uint8_t indx = 0;                // индекс

   msg[0] = CANMONITOR_EXTENDED_ANSWER;

#define MAX_DATA_BYTES  6
   
   while(sendedBytes != len)
   {
      needToSend = len - sendedBytes;
      if(needToSend <= MAX_DATA_BYTES) // если оставшиеся данные влезают в один пакет - то он последний
      {
          msg[1] = EXT_CMD_END;
      }
      else
      {
          if(sendedBytes == 0) msg[1] = EXT_CMD_START;
          else msg[1] = EXT_CMD_CONT;
      }

      dataBytesInFrame = needToSend <= MAX_DATA_BYTES ? needToSend : MAX_DATA_BYTES;
      sendedBytes += dataBytesInFrame;

      for(uint8_t i=0; i<dataBytesInFrame; i++)
      {
         msg[i+2] = pBuf[indx++];
      }
      canMonitor_send(msg, dataBytesInFrame+2);  
   }  
}

/*
// Сохранить новую аппаратную шкалу
void canMonitor_saveNewHardwareScale()
{
  extern uint32_t g_hardwareZeroAngleCorrection;   // ноль аппаратной шкалы
  extern uint32_t g_rawSysAngle;                   // датчик угла

  g_hardwareZeroAngleCorrection = g_rawSysAngle;
  saveUserDataInFlash();
}
*/