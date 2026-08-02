#include <stdio.h>

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define LED_PIN GPIO_NUM_2

void blink_task(void *pvParameters)
{
    uint32_t interval = (uint32_t)pvParameters;

    // Note: Actually, pvParameters is meant to be the pointer
    // to the parameters.
    // We are doing a trick to send an integer directly.
    // We create an integer and tells the computer:
    // "this is not an int, it's a pointer" buy converting it
    // to a pointer (not to be confused with getting the pointer
    // to that int).
    // The bytes can be directly converted since both are a set of
    // 32 bits (ESP32).
    // Then we convert it back 😎

    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    while (1)
    {
        gpio_set_level(LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(interval));
        gpio_set_level(LED_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(interval));
    }
}

void app_main(void)
{
    const char *TAG = "main_function";

    ESP_LOGI(TAG, "Creating task...");

    xTaskCreate(
        blink_task,
        "blink_task",
        2048,
        (void *)50,
        5,
        NULL);

    ESP_LOGI(TAG, "Task created.");
}
