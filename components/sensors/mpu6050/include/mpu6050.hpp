#pragma once
#include"inputdevice.hpp"
#include"cstdint"
#include"cstddef"
#include"esp_err.h"
#include "driver/i2c_master.h" 

class MPU6050: public InputDevice{
    private:
    static constexpr uint8_t WHO_AM_I=0x75;
    static constexpr uint8_t PWR_MGMT_1=0x6B;

    i2c_master_dev_handle_t device_;

    esp_err_t readRegister(uint8_t reg, uint8_t* val);

    esp_err_t writeRegister(uint8_t reg, uint8_t val);

    esp_err_t readRegisters(uint8_t startReg, uint8_t* buffer, size_t length);

    public:
    static constexpr uint8_t ADDRESS=0x68;
    explicit MPU6050(i2c_master_dev_handle_t device);

    esp_err_t begin() override;

    esp_err_t readAcceleration(float *x_g, float *y_g, float *z_g);
};