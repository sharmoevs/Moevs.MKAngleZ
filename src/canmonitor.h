#ifndef __CANMONITOR_H__
#define __CANMONITOR_H__

#include "MDR32Fx.h"
#include "can.h"


// Идентификаторы команд
#define CANMONITOR_STATE_REQUEST                0x01    // запрос состояния
#define CANMONITOR_EXTENDED_CMD                 0xF0    // расширенная команда
#define CANMONITOR_SAVE_NEW_HARDWARE_SCALE      0x0C    // сохранить новый ноль аппаратаной шкалы

// Служебные команды
#define CANMONITOR_RESET_CONTROLLER             0xF5    // сброс контроллера
#define CANMONITOR_DEBUG                        0xDB    // отладочная команда

// Идентификаторы ответов
#define CANMONITOR_TANGAGE_ANGLE                0xA1    // текущий угол
#define CANMONITOR_WORKING_TIME_MS              0x59    // время в работы в мс от подачи питания
#define CANMONITOR_EXTENDED_ANSWER              0xF0    // расширенная команда


// Расширенная команда
#define EXT_CMD_START                           0x00
#define EXT_CMD_CONT                            0x01
#define EXT_CMD_END                             0x02


void canMonitor_rxFrameHandler(uint8_t *buf, uint8_t len);      // Принят пакет от монитора
void canMonitor_send(uint8_t *pData, uint8_t len);              // Отправить данные CAN-монитору




void canMonitor_sendStateWord();        // отправить слово состояния
void canMonitor_sendWorkingTime();      // время наработки
void canMonitor_rxExtCmd(uint8_t *pData, uint8_t len);      // принять расширенную команду
void canMonitor_sendExtAnswer(uint8_t *pBuf, uint8_t len);        // отправить ответ на расширенную команду

void canMonitor_saveNewHardwareScale();




#endif //__CANMONITOR_H__
