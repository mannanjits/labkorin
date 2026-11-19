#include "i2c_bus.hpp"

i2cbus::i2cbus(gpio_num_t sda, gpio_num_t scl)
    : sda_(sda), scl_(scl), bus_(nullptr)
{
}

esp_err_t i2cbus::begin()
{
    i2c_master_bus_config_t config{};

    config.i2c_port = I2C_NUM_0;
    config.sda_io_num = sda_;
    config.scl_io_num = scl_;
    config.clk_source = I2C_CLK_SRC_DEFAULT;
    config.glitch_ignore_cnt = 7;
    config.flags.enable_internal_pullup = true;

    return i2c_new_master_bus(&config, &bus_);
}

esp_err_t i2cbus::addDevice(
    uint8_t address,
    i2c_master_dev_handle_t* out_handle)
{
    if (out_handle == nullptr) {
        return ESP_ERR_INVALID_ARG;
    }

    i2c_device_config_t config{};

    config.dev_addr_length = I2C_ADDR_BIT_LEN_7;
    config.device_address = address;
    config.scl_speed_hz = 100000;

    return i2c_master_bus_add_device(bus_, &config, out_handle);
}

esp_err_t i2cbus::scanDevice(uint8_t address)
{
    if (bus_ == nullptr)
        return ESP_ERR_INVALID_STATE;

    return i2c_master_probe(bus_, address, 100);
}

i2c_master_bus_handle_t i2cbus::getBus() const
{
    return bus_;
}