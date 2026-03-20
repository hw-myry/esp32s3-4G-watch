#include <stdio.h>
#include <string.h>
#include "driver/i2c.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define I2C_MASTER_SCL_IO 21
#define I2C_MASTER_SDA_IO 20
#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_FREQ_HZ 400000

#define OLED_ADDR 0x3C

static const char *TAG = "OLED";

static const uint8_t font6x8[][6] = {
    // ASCII 32~127

    [32] = {0,0,0,0,0,0}, // space

    ['H'] = {0x7F,0x08,0x08,0x08,0x7F,0x00},
    ['e'] = {0x38,0x54,0x54,0x54,0x18,0x00},
    ['l'] = {0x00,0x41,0x7F,0x40,0x00,0x00},
    ['o'] = {0x38,0x44,0x44,0x44,0x38,0x00},

    // ⭐ 正确的 W（你现在这个是错的）
    ['W'] = {0x7E, 0x01, 0x0E, 0x01, 0x7E, 0x00},

    // ⭐ 正确的 r
    ['r'] = {0x7C,0x08,0x04,0x04,0x08,0x00},

    // ⭐ 正确的 d
    ['d'] = {0x38,0x44,0x44,0x48,0x7F,0x00},
};


/**************** I2C 初始化 ****************/
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

/**************** OLED 基础函数 ****************/
void oled_cmd(uint8_t cmd)
{
    uint8_t data[2] = {0x00, cmd};

    i2c_master_write_to_device(
        I2C_MASTER_NUM,
        OLED_ADDR,
        data,
        2,
        1000 / portTICK_PERIOD_MS
    );
}

void oled_data(uint8_t *data, size_t len)
{
    uint8_t buffer[129]; // 最大128 + 1

    buffer[0] = 0x40;
    memcpy(&buffer[1], data, len);

    i2c_master_write_to_device(
        I2C_MASTER_NUM,
        OLED_ADDR,
        buffer,
        len + 1,
        1000 / portTICK_PERIOD_MS
    );
}

/**************** OLED 初始化 ****************/
void oled_init(void)
{
    vTaskDelay(pdMS_TO_TICKS(100));

    oled_cmd(0xAE); // display off

    oled_cmd(0x20); // addressing mode
    oled_cmd(0x00); // horizontal mode

    oled_cmd(0xB0);
    oled_cmd(0xC8);
    oled_cmd(0x00);
    oled_cmd(0x10);
    oled_cmd(0x40);

    oled_cmd(0x81);
    oled_cmd(0x7F);

    oled_cmd(0xA1);
    oled_cmd(0xA6);

    oled_cmd(0xA8);
    oled_cmd(0x3F);

    oled_cmd(0xA4);
    oled_cmd(0xD3);
    oled_cmd(0x00);

    oled_cmd(0xD5);
    oled_cmd(0x80);

    oled_cmd(0xD9);
    oled_cmd(0xF1);

    oled_cmd(0xDA);
    oled_cmd(0x12);

    oled_cmd(0xDB);
    oled_cmd(0x40);

    oled_cmd(0x8D);
    oled_cmd(0x14);

    oled_cmd(0xAF); // display ON
}

/**************** 清屏 ****************/
void oled_clear(void)
{
    uint8_t zero[128];
    memset(zero, 0x00, sizeof(zero));

    for (int page = 0; page < 8; page++) {
        oled_cmd(0xB0 + page);
        oled_cmd(0x00);
        oled_cmd(0x10);
        oled_data(zero, 128);
    }
}

/**************** 全亮测试 ****************/
void oled_full_on(void)
{
    uint8_t buf[128];
    memset(buf, 0xFF, sizeof(buf));

    for (int page = 0; page < 8; page++) {
        oled_cmd(0xB0 + page);
        oled_cmd(0x00);
        oled_cmd(0x10);
        oled_data(buf, 128);
    }
}

void oled_set_cursor(uint8_t page, uint8_t col)
{
    oled_cmd(0xB0 + page);               // 页
    oled_cmd(0x00 + (col & 0x0F));       // 列低位
    oled_cmd(0x10 + (col >> 4));         // 列高位
}

void oled_show_char(uint8_t page, uint8_t col, char c)
{
    oled_set_cursor(page, col);

    oled_data((uint8_t *)font6x8[(int)c], 6);
}

void oled_show_string(uint8_t page, uint8_t col, const char *str)
{
    while (*str) {
        oled_show_char(page, col, *str);
        col += 6;

        if (col > 122) { // 换行
            col = 0;
            page++;
        }

        str++;
    }
}

void oled_show_hello_center(void)
{
    const char *text = "Hello";

    uint8_t len = strlen(text);
    uint8_t col = (128 - len * 6) / 2;
    uint8_t page = 3;  // 中间位置（0~7）

    oled_show_string(page, col, text);
}


/**************** 主函数 ****************/
void app_main(void)
{
    i2c_master_init();

    vTaskDelay(pdMS_TO_TICKS(100));

    oled_init();
    oled_clear();

    oled_show_hello_center();
}