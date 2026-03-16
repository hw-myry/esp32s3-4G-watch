#include <stdio.h>
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define PIN_CHG GPIO_NUM_13
#define PIN_PPR GPIO_NUM_14

static const char *TAG = "charger";

void charger_gpio_init()
{
    gpio_config_t io = {
        .pin_bit_mask = (1ULL << PIN_CHG) | (1ULL << PIN_PPR),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE};
        
    gpio_config(&io);
}

void app_main(void)
{
    charger_gpio_init();

    while (1)
    {
        int chg = gpio_get_level(PIN_CHG);
        int ppr = gpio_get_level(PIN_PPR);

        printf("PPR=%d CHG=%d\n", ppr, chg);

        if (ppr == 1)
        {
            ESP_LOGI(TAG, "No charger");
        }
        else
        {
            if (chg == 0)
                ESP_LOGI(TAG, "Charging");
            else
                ESP_LOGI(TAG, "Charge done");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}