#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "esp_err.h"

#define LED_GPIO 1
#define LED_NUM 3

void app_main(void)
{
    led_strip_handle_t strip;

    // 基础配置（兼容所有版本）
    led_strip_config_t strip_config = {
        .strip_gpio_num = LED_GPIO,
        .max_leds = LED_NUM,
    };

    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
    };

    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &strip));

    while (1)
    {
        // 红
        for (int i = 0; i < LED_NUM; i++)
        {
            led_strip_set_pixel(strip, i, 5, 0, 0);
        }
        led_strip_refresh(strip);
        vTaskDelay(pdMS_TO_TICKS(1000));

        // 绿
        for (int i = 0; i < LED_NUM; i++)
        {
            led_strip_set_pixel(strip, i, 0, 5, 0);
        }
        led_strip_refresh(strip);
        vTaskDelay(pdMS_TO_TICKS(1000));

        // 蓝
        for (int i = 0; i < LED_NUM; i++)
        {
            led_strip_set_pixel(strip, i, 0, 0, 5);
        }
        led_strip_refresh(strip);
        vTaskDelay(pdMS_TO_TICKS(1000));

        // 关
        led_strip_clear(strip);
        led_strip_refresh(strip);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}