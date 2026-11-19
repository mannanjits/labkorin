#pragma once
#include"device.hpp"
#include"driver/gpio.h"

class OutputDevice:public Device{
    public:
    gpio_num_t pin;
    OutputDevice(gpio_num_t pin);
    virtual void on()=0;
    virtual void off()=0;
    virtual void toggle()=0;
};