#pragma once
#include <memory>
#include "esp_err.h"
#include "driver/gpio.h"
#include "driver/i2c_master.h"
#include "i2c_bus.hpp"
#include "mpu6050.hpp"
#include "vibration_analyser.hpp"
#include "led.hpp"
#include "buzzer.hpp"

struct VibrationMonitorConfig
{
    gpio_num_t sda_pin;
    gpio_num_t scl_pin;

    gpio_num_t led_pin;
    gpio_num_t buzzer_pin;

    float vibration_threshold;
};

class vibration_monitor
{
private:
    i2cbus i2c;
    i2c_master_dev_handle_t mpu_device = nullptr;
    std::unique_ptr<MPU6050> mpu;

    vibration_analyser analyser;

    led status_led;
    buzzer alarm_buzzer;

    float vibration_threshold;

public:
    explicit vibration_monitor(const VibrationMonitorConfig& config);

    esp_err_t begin();
    void start();
};