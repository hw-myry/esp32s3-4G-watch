#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_GPIO GPIO_NUM_8
#define TEST_GPIO GPIO_NUM_5   // 新增GPIO5

static const char *TAG = "GPIO_TEST";

void app_main(void)
{
    ESP_LOGI(TAG, "Start GPIO test");

    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << LED_GPIO) | (1ULL << TEST_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&io_conf);

    ESP_LOGI(TAG, "GPIO configured");

    // GPIO5一直拉高
    gpio_set_level(TEST_GPIO, 1);

    int level = 0;

    while (1) {
        level = !level;
        gpio_set_level(LED_GPIO, level);

        ESP_LOGI(TAG, "GPIO8 level: %d  GPIO5: HIGH", level);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}