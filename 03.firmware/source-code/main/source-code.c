#include "driver/gpio.h"

#define GPIO1 GPIO_NUM_5
#define GPIO2 GPIO_NUM_8

void app_main(void)
{
    gpio_set_direction(GPIO1, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO2, GPIO_MODE_OUTPUT);

    // 拉高多个GPIO
    gpio_set_level(GPIO1, 1);
    gpio_set_level(GPIO2, 1);
}