#line 1 "D:\\BaiduSyncdisk\\esp32yk\\yk2\\1.ControllerSystem\\libraries\\RF24\\utility\\wiringPi\\interrupt.h"
/**
 * Interrupt functions wrapped from pigpio library
 */
#ifndef RF24_UTILITY_WIRINGPI_INTERRUPT_H__
#define RF24_UTILITY_WIRINGPI_INTERRUPT_H__

#include <wiringPi.h>

#define attachInterrupt wiringPiISR
#define detachInterrupt wiringPiISRStop

#endif // RF24_UTILITY_WIRINGPI_INTERRUPT_H__
