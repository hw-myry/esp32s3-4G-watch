#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"

#define ADC_CHANNEL ADC_CHANNEL_2 
#define ADC_UNIT    ADC_UNIT_1

static const char *TAG = "BAT_ADC";

void app_main(void)
{
    adc_oneshot_unit_handle_t adc_handle;

    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT,
    };
    adc_oneshot_new_unit(&init_config, &adc_handle);

    adc_oneshot_chan_cfg_t config = {
        .atten = ADC_ATTEN_DB_11,   // 测量范围到 ~3.3V
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    adc_oneshot_config_channel(adc_handle, ADC_CHANNEL, &config);

    while (1) {
        int adc_raw = 0;
        adc_oneshot_read(adc_handle, ADC_CHANNEL, &adc_raw);

        // 转换为电压（mV）
        float voltage = adc_raw * 3.3 / 4095.0;

        // 还原电池电压（因为分压1/2）
        float battery_voltage = voltage * 2;

        ESP_LOGI(TAG, "ADC: %d, Vbat: %.2f V", adc_raw, battery_voltage);

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}