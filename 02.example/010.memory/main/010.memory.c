#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_heap_caps.h"
#include <string.h>

// =======================
// 打印内存信息
// =======================
void print_memory_info()
{
    printf("\n========== Memory Info ==========\n");

    printf("Free heap: %d bytes\n",
           heap_caps_get_free_size(MALLOC_CAP_DEFAULT));

    printf("Min free heap: %d bytes\n",
           heap_caps_get_minimum_free_size(MALLOC_CAP_DEFAULT));

    printf("Largest free block: %d bytes\n",
           heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT));

    printf("Internal RAM free: %d bytes\n",
           heap_caps_get_free_size(MALLOC_CAP_INTERNAL));

#ifdef CONFIG_SPIRAM
    printf("PSRAM free: %d bytes\n",
           heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
#endif

    printf("=================================\n");
}

// =======================
// malloc 测试（检测极限）
// =======================
void test_malloc()
{
    printf("\n--- malloc test start ---\n");

    size_t size = 1024;
    void *ptr;

    while (1) {
        ptr = malloc(size);
        if (!ptr) {
            printf("malloc FAILED at %d bytes\n", size);
            break;
        }

        printf("malloc OK: %d bytes\n", size);

        // 防止编译器优化
        memset(ptr, 0, size);

        size += 1024;

        vTaskDelay(pdMS_TO_TICKS(100));
    }

    printf("--- malloc test end ---\n");
}

// =======================
// 任务栈检测
// =======================
void print_stack_info()
{
    UBaseType_t stack = uxTaskGetStackHighWaterMark(NULL);
    printf("Stack remaining: %d words (%d bytes)\n",
           stack, stack * sizeof(StackType_t));
}

// =======================
// 周期监控任务
// =======================
void monitor_task(void *arg)
{
    while (1) {
        print_memory_info();
        print_stack_info();

        vTaskDelay(pdMS_TO_TICKS(3000)); // 每3秒打印一次
    }
}

// =======================
// 主函数
// =======================
void app_main(void)
{
    printf("\nESP32-S3 Memory Monitor Start\n");

    // 初始打印一次
    print_memory_info();

    // 创建监控任务
    xTaskCreate(monitor_task, "monitor_task", 4096, NULL, 5, NULL);

    // 等几秒再做malloc测试（避免和日志混乱）
    vTaskDelay(pdMS_TO_TICKS(5000));

    // malloc压力测试（可注释掉）
    // test_malloc();
}