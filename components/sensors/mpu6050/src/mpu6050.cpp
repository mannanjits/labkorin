#include "mpu6050.hpp"
#include "esp_log.h"

MPU6050::MPU6050(i2c_master_dev_handle_t device)
:device_(device){}

esp_err_t MPU6050::begin()
{
    uint8_t identity = 0;
    uint8_t power = 0;

    esp_err_t err = readRegister(0x75, &identity);

    if (err != ESP_OK)
        return err;

    ESP_LOGI("MPU6050", "WHO_AM_I: 0x%02X", identity);

    err = readRegister(0x6B, &power);

    if (err != ESP_OK)
        return err;

    ESP_LOGI("MPU6050", "PWR_MGMT_1: 0x%02X", power);

    return ESP_OK;
}

esp_err_t MPU6050::readRegister(uint8_t reg, uint8_t* val)
{
    if (val == nullptr)
        return ESP_ERR_INVALID_ARG;

    return i2c_master_transmit_receive(
        device_,
        &reg,
        1,
        val,
        1,
        1000
    );
}

esp_err_t MPU6050::readRegisters(
    uint8_t startReg,
    uint8_t* buffer,
    size_t length
)
{
    if (buffer == nullptr || length == 0)
        return ESP_ERR_INVALID_ARG;

    return i2c_master_transmit_receive(
        device_,
        &startReg,
        1,
        buffer,
        length,
        1000
    );
}

esp_err_t MPU6050::readAcceleration(float *x_g, float *y_g, float *z_g){

    uint8_t bytes[6];

    if (x_g == nullptr || y_g == nullptr || z_g == nullptr) {
    return ESP_ERR_INVALID_ARG;
    }

    esp_err_t err=readRegisters(0x3B, bytes, 6);

    if(err!=ESP_OK){
        return err;
    }
    
    uint16_t rawX = (static_cast<uint16_t>(bytes[0]) << 8) | bytes[1];
    uint16_t rawY = (static_cast<uint16_t>(bytes[2]) << 8) | bytes[3];
    uint16_t rawZ = (static_cast<uint16_t>(bytes[4]) << 8) | bytes[5];
    
    int16_t accelX = static_cast<int16_t>(rawX);
    int16_t accelY = static_cast<int16_t>(rawY);
    int16_t accelZ = static_cast<int16_t>(rawZ);

    *x_g = accelX / 16384.0f;
    *y_g = accelY / 16384.0f;
    *z_g = accelZ / 16384.0f;

    return ESP_OK;
}