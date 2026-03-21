#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define GPIO_PWR_EN   GPIO_NUM_5    // 电源保持输出
#define GPIO_PWR_DS   GPIO_NUM_4    // 按键检测输入，低电平表示按下

#define GPIO_LED1     GPIO_NUM_8
#define GPIO_LED2     GPIO_NUM_9
#define GPIO_LED3     GPIO_NUM_47
#define GPIO_LED4     GPIO_NUM_48

#define LONG_PRESS_MS      3000
#define POLL_INTERVAL_MS   20

static const char *TAG = "PWR_KEY";

static bool power_key_pressed(void)
{
    return gpio_get_level(GPIO_PWR_DS) == 0;
}

void app_main(void)
{
    gpio_config_t out_conf = {
        .pin_bit_mask = (1ULL << GPIO_PWR_EN) |
                        (1ULL << GPIO_LED1)   |
                        (1ULL << GPIO_LED2)   |
                        (1ULL << GPIO_LED3)   |
                        (1ULL << GPIO_LED4),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&out_conf);

    gpio_config_t in_conf = {
        .pin_bit_mask = (1ULL << GPIO_PWR_DS),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&in_conf);

    // 上电保持
    gpio_set_level(GPIO_PWR_EN, 1);

    // LED全亮测试
    gpio_set_level(GPIO_LED1, 1);
    gpio_set_level(GPIO_LED2, 1);
    gpio_set_level(GPIO_LED3, 1);
    gpio_set_level(GPIO_LED4, 1);

    ESP_LOGI(TAG, "System on, PWR_EN=1");
    ESP_LOGI(TAG, "Hold IO4 low for %d ms to power off", LONG_PRESS_MS);

    int pressed_ms = 0;

    while (1) {
        if (power_key_pressed()) {
            pressed_ms += POLL_INTERVAL_MS;

            if (pressed_ms == POLL_INTERVAL_MS) {
                ESP_LOGI(TAG, "Power key pressed");
            }

            if (pressed_ms >= LONG_PRESS_MS) {
                ESP_LOGI(TAG, "Long press detected, power off");
                vTaskDelay(pdMS_TO_TICKS(50));
                gpio_set_level(GPIO_PWR_EN, 0);
                while (1) {
                    vTaskDelay(pdMS_TO_TICKS(1000));
                }
            }
        } else {
            if (pressed_ms > 0) {
                ESP_LOGI(TAG, "Power key released, pressed %d ms", pressed_ms);
            }
            pressed_ms = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(POLL_INTERVAL_MS));
    }
}