#pragma once

#include "driver/i2c_master.h"
#include "esp_err.h"
#include "driver/gpio.h"

class i2cbus {
private:
    gpio_num_t sda_;
    gpio_num_t scl_;

    i2c_master_bus_handle_t bus_;

public:
    i2cbus(gpio_num_t sda, gpio_num_t scl);

    esp_err_t begin();

    esp_err_t addDevice(
        uint8_t address,
        i2c_master_dev_handle_t* out_handle
    );

    esp_err_t scanDevice(uint8_t address);

    i2c_master_bus_handle_t getBus() const;
};