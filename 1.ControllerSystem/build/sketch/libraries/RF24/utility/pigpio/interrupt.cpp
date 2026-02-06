#line 1 "D:\\BaiduSyncdisk\\esp32yk\\yk2\\1.ControllerSystem\\libraries\\RF24\\utility\\pigpio\\interrupt.cpp"
/**
 * Interrupt implementation wrapped from pigpio library
 */
#include <pigpio.h>
#include "interrupt.h"

#ifdef __cplusplus
extern "C" {
#endif

int attachInterrupt(int pin, int mode, void (*function)(void))
{
    gpioInitialise();
    return gpioSetISRFunc(pin, mode, 0, (gpioISRFunc_t)function);
}

int detachInterrupt(int pin)
{
    return gpioSetISRFunc(pin, 0, 0, NULL);
}

void rfNoInterrupts()
{
}

void rfInterrupts()
{
}

#ifdef __cplusplus
}
#endif
