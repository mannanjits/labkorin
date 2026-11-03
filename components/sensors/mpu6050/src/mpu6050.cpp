#include "mpu6050.hpp"
#include "esp_log.h"

MPU6050::MPU6050(i2c_master_dev_handle_t device)
:device_(device){}

esp_err_t MPU6050::begin(){
    uint8_t identity=0;
    constexpr uint8_t WHO_AM_I_VALUE=0x68;

    esp_err_t err=readRegister(WHO_AM_I, &identity);

    if(err!=ESP_OK){
        return err;
    }

    uint16_t rawX=(static_cast<uint16_t>(bytes[0]<<8) | bytes[1]);
    int16_t accelX=static_cast<uint16_t>(rawX);
    float accelX_g=accelX/ 16384.0f;
    ESP_LOGI("MPU6050", "X acceleration: %.3f g", (double)accelX_g);

    if(identity!=WHO_AM_I_VALUE){
        return ESP_ERR_NOT_FOUND;
    }

    if((err=writeRegister(PWR_MGMT_1, 0x00))!=ESP_OK){
        return err;
    };
    
    return ESP_OK;
}

esp_err_t MPU6050::writeRegister(uint8_t reg, uint8_t val){
    uint8_t data[2]={reg, val};
    return i2c_master_transmit(device_, data, 2, 1000);
}

esp_err_t MPU6050::readRegister(uint8_t reg, uint8_t* val){
    uint8_t reg_addr=reg;
    return i2c_master_transmit_receive(device_, &reg_addr, 1, val, 1, 1000);
}

esp_err_t MPU6050::readRegisters(uint8_t startReg, uint8_t* buffer, size_t length){
    uint8_t startingReg=startReg;
    esp_err_t err=i2c_master_transmit_receive(device_, &startingReg, 1, buffer, length, 1000);
    return err;
}

esp_err_t MPU6050::readAcceleration(float *x_g, float *y_g, float *z_g){
    uint8_t bytes[6];

    esp_err_t err=readRegisters(0x3B, bytes, 6);

    if(err!=ESP_OK){
        return err;
    }
    
    int16_t rawX=static_cast<int16_t>(static_cast<int16_t>(bytes[0])<<8 | bytes[1]);
    int16_t rawY=static_cast<int16_t>(static_cast<int16_t>(bytes[2])<<8 | bytes[3]);
    int16_t rawZ=static_cast<int16_t>(static_cast<int16_t>(bytes[4])<<8 | bytes[5]);
    
    *x_g = rawX / 16384.0f;
    *y_g = rawY / 16384.0f;
    *z_g = rawZ / 16384.0f;

    return ESP_OK;
}