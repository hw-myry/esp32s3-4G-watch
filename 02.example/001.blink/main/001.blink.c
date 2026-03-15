#include "driver/gpio.h"
#include "esp_log.h"

#define LED_GPIO GPIO_NUM_8   // 选择你要控制的GPIO

static const char *TAG = "GPIO_TEST";

void app_main(void)
{
    ESP_LOGI(TAG, "Start GPIO test");

    // 1. 配置GPIO为输出
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << LED_GPIO),  // 选择GPIO
        .mode = GPIO_MODE_OUTPUT,            // 输出模式
        .pull_up_en = GPIO_PULLUP_DISABLE,   
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&io_conf);
    ESP_LOGI(TAG, "GPIO %d configured as output", LED_GPIO);

    // 2. 拉高GPIO
    gpio_set_level(LED_GPIO, 1);
    ESP_LOGI(TAG, "GPIO %d set to HIGH", LED_GPIO);
}