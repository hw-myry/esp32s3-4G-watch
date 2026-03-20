#include <stdio.h>
#include "driver/i2c.h"
#include "esp_log.h"

#define I2C_MASTER_SCL_IO 21
#define I2C_MASTER_SDA_IO 20

#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_FREQ_HZ 100000

static const char *TAG = "I2C_SCAN";

void i2c_master_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
}

void i2c_scan(void)
{
    ESP_LOGI(TAG, "Scanning I2C bus...");

    for (uint8_t addr = 1; addr < 127; addr++) {
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();

        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, true);
        i2c_master_stop(cmd);

        esp_err_t ret = i2c_master_cmd_begin(
            I2C_MASTER_NUM,
            cmd,
            50 / portTICK_PERIOD_MS
        );

        i2c_cmd_link_delete(cmd);

        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "Found device at 0x%02X", addr);
        }
    }

    ESP_LOGI(TAG, "Scan done");
}


void app_main(void)
{
    i2c_master_init();
    vTaskDelay(100 / portTICK_PERIOD_MS);

    i2c_scan();
}



// #define I2C_MASTER_NUM I2C_NUM_0
// #define I2C_MASTER_FREQ_HZ 400000

// #define OLED_ADDR 0x3C

// void i2c_master_init(void)
// {
//     i2c_config_t conf = {
//         .mode = I2C_MODE_MASTER,
//         .sda_io_num = I2C_MASTER_SDA_IO,
//         .scl_io_num = I2C_MASTER_SCL_IO,
//         .sda_pullup_en = GPIO_PULLUP_ENABLE,
//         .scl_pullup_en = GPIO_PULLUP_ENABLE,
//         .master.clk_speed = I2C_MASTER_FREQ_HZ,
//     };

//     i2c_param_config(I2C_MASTER_NUM, &conf);
//     i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
// }



// void oled_cmd(uint8_t cmd)
// {
//     uint8_t data[2] = {0x00, cmd}; // 0x00 = command

//     i2c_master_write_to_device(
//         I2C_MASTER_NUM,
//         OLED_ADDR,
//         data,
//         2,
//         1000 / portTICK_PERIOD_MS
//     );
// }


// void oled_data(uint8_t *data, size_t len)
// {
//     uint8_t buffer[len + 1];
//     buffer[0] = 0x40; // data标志

//     memcpy(&buffer[1], data, len);

//     i2c_master_write_to_device(
//         I2C_MASTER_NUM,
//         OLED_ADDR,
//         buffer,
//         len + 1,
//         1000 / portTICK_PERIOD_MS
//     );
// }


// void oled_init(void)
// {
//     vTaskDelay(100 / portTICK_PERIOD_MS);

//     oled_cmd(0xAE); // display off
//     oled_cmd(0x20); // Memory Addressing Mode
//     oled_cmd(0x10);
//     oled_cmd(0xB0);
//     oled_cmd(0xC8);
//     oled_cmd(0x00);
//     oled_cmd(0x10);
//     oled_cmd(0x40);
//     oled_cmd(0x81);
//     oled_cmd(0x7F);
//     oled_cmd(0xA1);
//     oled_cmd(0xA6);
//     oled_cmd(0xA8);
//     oled_cmd(0x3F);
//     oled_cmd(0xA4);
//     oled_cmd(0xD3);
//     oled_cmd(0x00);
//     oled_cmd(0xD5);
//     oled_cmd(0xF0);
//     oled_cmd(0xD9);
//     oled_cmd(0x22);
//     oled_cmd(0xDA);
//     oled_cmd(0x12);
//     oled_cmd(0xDB);
//     oled_cmd(0x20);
//     oled_cmd(0x8D);
//     oled_cmd(0x14);
//     oled_cmd(0xAF); // display on
// }


// void oled_clear(void)
// {
//     uint8_t zero[128] = {0};

//     for (int page = 0; page < 8; page++) {
//         oled_cmd(0xB0 + page);
//         oled_cmd(0x00);
//         oled_cmd(0x10);
//         oled_data(zero, 128);
//     }
// }


// void oled_test_pattern(void)
// {
//     uint8_t buf[128];
//     memset(buf, 0xFF, 128);

//     for (int page = 0; page < 8; page++) {
//         oled_cmd(0xB0 + page);
//         oled_cmd(0x00);
//         oled_cmd(0x10);
//         oled_data(buf, 128);
//     }
// }

// void app_main(void)
// {
//     i2c_master_init();
//     oled_init();
//     oled_clear();

//     oled_test_pattern(); // 点亮测试
// }
