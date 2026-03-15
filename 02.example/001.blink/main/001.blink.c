#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_GPIO GPIO_NUM_8   // 控制的GPIO

static const char *TAG = "GPIO_BLINK";

void app_main(void)
{
    ESP_LOGI(TAG, "Start GPIO blink test");

    // 配置GPIO为输出
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << LED_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&io_conf);
    ESP_LOGI(TAG, "GPIO %d configured as output", LED_GPIO);

    int level = 0;

    while (1) {
        level = !level;                      // 翻转电平
        gpio_set_level(LED_GPIO, level);     // 设置GPIO电平

        ESP_LOGI(TAG, "GPIO %d level: %d", LED_GPIO, level);

        vTaskDelay(pdMS_TO_TICKS(500));      // 延时500ms
    }
}