#line 1 "D:\\BaiduSyncdisk\\esp32yk\\yk2\\1.ControllerSystem\\libraries\\RF24\\utility\\pigpio\\gpio.cpp"
/*
 * GPIO Functions
 */
#include <pigpio.h>
#include "gpio.h"

bool initialized = 0;

GPIO::GPIO()
{
}

GPIO::~GPIO()
{
    gpioTerminate();
}

void GPIO::open(int port, int DDR)
{
    if (!initialized) {
        gpioInitialise();
    }
    initialized = true;
    gpioSetMode(port, PI_OUTPUT);
}

void GPIO::close(int port)
{
}

int GPIO::read(int port)
{
    return gpioRead(port);
}

void GPIO::write(int port, int value)
{
    gpioWrite(port, value);
}
