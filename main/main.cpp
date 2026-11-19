#include "vibration_monitor.hpp"
#include "esp_check.h"
#include "driver/i2c.h"


extern "C" void app_main()
{
    const VibrationMonitorConfig config{
        .sda_pin = GPIO_NUM_16,
        .scl_pin = GPIO_NUM_17,
        .led_pin = GPIO_NUM_4,
        .buzzer_pin = GPIO_NUM_18,
        .vibration_threshold = 0.1f
    };

    vibration_monitor monitor(config);

    ESP_ERROR_CHECK(monitor.begin());
    monitor.start();
}