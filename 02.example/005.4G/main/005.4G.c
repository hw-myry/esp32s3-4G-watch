#include "driver/uart.h"
#include "esp_log.h"
#include "string.h"

#define UART_PORT UART_NUM_1
#define TXD_PIN 10
#define RXD_PIN 11
#define BUF_SIZE 1024

static const char *TAG = "UART_TEST";

void app_main(void)
{
    // 1️⃣ 配置UART参数
    uart_config_t uart_config = {
        .baud_rate = 9600,  // 根据ML370A调整
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    uart_driver_install(UART_PORT, BUF_SIZE, 0, 0, NULL, 0);
    uart_param_config(UART_PORT, &uart_config);
    uart_set_pin(UART_PORT, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    ESP_LOGI(TAG, "UART init done");

    uint8_t data[BUF_SIZE];

    while (1) {
        // 2️⃣ 发送测试指令（比如AT）
        const char *test_str = "AT\r\n";
        uart_write_bytes(UART_PORT, test_str, strlen(test_str));
        ESP_LOGI(TAG, "Send: %s", test_str);

        // 3️⃣ 读取返回
        int len = uart_read_bytes(UART_PORT, data, BUF_SIZE - 1, pdMS_TO_TICKS(1000));

        if (len > 0) {
            data[len] = '\0';
            ESP_LOGI(TAG, "Recv: %s", (char *)data);
        } else {
            ESP_LOGW(TAG, "No response");
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}