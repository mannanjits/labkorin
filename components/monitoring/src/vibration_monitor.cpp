#include"vibration_monitor.hpp"
#include"esp_log.h"
#include"freertos/FREERTOS.h"
#include"freertos/task.h"

static const char* TAG="vibration_monitor";

vibration_monitor::vibration_monitor(const VibrationMonitorConfig& config)
    : i2c(config.sda_pin, config.scl_pin),
      status_led(config.led_pin),
      alarm_buzzer(config.buzzer_pin),
      vibration_threshold(config.vibration_threshold)
{
}

esp_err_t vibration_monitor::begin()
{
    ESP_LOGI(TAG, "Initializing LED");

    esp_err_t err = status_led.begin();
    if (err != ESP_OK) return err;

    ESP_LOGI(TAG, "Initializing buzzer");

    err = alarm_buzzer.begin();
    if (err != ESP_OK) return err;

    status_led.off();
    alarm_buzzer.off();
    
    
    ESP_LOGI(TAG, "Initializing I2C");

    err = i2c.begin();
    if (err != ESP_OK) return err;

    ESP_LOGI(TAG, "Scanning for MPU6050 at 0x68");

    err = i2c.scanDevice(MPU6050::ADDRESS);

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "MPU6050 not found: %s",
                 esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(TAG, "MPU6050 detected!");

    ESP_LOGI(TAG, "Adding MPU6050 device");

    err = i2c.addDevice(
        MPU6050::ADDRESS,
        &mpu_device
    );

    if (err != ESP_OK) return err;

    ESP_LOGI(TAG, "Creating MPU6050 object");

    mpu = std::make_unique<MPU6050>(mpu_device);

    ESP_LOGI(TAG, "Checking MPU6050 identity");

    err = mpu->begin();

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "MPU6050 initialization failed: %s",
                 esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(TAG, "Labkorin hardware initialized successfully!");

    return ESP_OK;
}

void vibration_monitor::start()
{
    if (!mpu) {
        ESP_LOGE(TAG, "Monitor has not been initialised");
        return;
    }

    float x, y, z;
    TickType_t alarm_until = 0;

    while (true) {
        esp_err_t err = mpu->readAcceleration(&x, &y, &z);

        if (err != ESP_OK) {
            ESP_LOGE(TAG, "MPU6050 read failed: %s",
                     esp_err_to_name(err));
            vTaskDelay(pdMS_TO_TICKS(100));
            continue;
        }

        analyser.addSample(x, y, z);

        if (analyser.isReady()) {
            float rms = analyser.calculateRMS();

            ESP_LOGI(TAG, "Vibration RMS: %.4f g", rms);

            if (rms > vibration_threshold) {
                alarm_until = xTaskGetTickCount() + pdMS_TO_TICKS(1000);
            }

            if ((int32_t)(alarm_until - xTaskGetTickCount()) > 0) {
                status_led.on();
                alarm_buzzer.on();
            }
            else {
                status_led.off();
                alarm_buzzer.off();
            }
        }

        vTaskDelay(pdMS_TO_TICKS(5));
    }
}
