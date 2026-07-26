#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

#define LED_GPIO GPIO_NUM_2 // LED already included in most ESP32 development boards

bool ledIsOn = false;

void my_task(void *pvParameter) // p stands for pointer and v stands for void. It means "pointer to void".
{
    char *task_name = (char *)pvParameter; // () is for type conversion
    const char *TAG = "MY_TASK";

    while (1)
    {
        if (ledIsOn)
        {
            ledIsOn = false;
            gpio_set_level(LED_GPIO, 0);
            ESP_LOGI(TAG, "LED is on");
        }
        else
        {
            ledIsOn = true;
            gpio_set_level(LED_GPIO, 1);
            ESP_LOGI(TAG, "LED is off");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    // LED setup
    gpio_reset_pin(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_GPIO, 0);

    char *param = "Task 1"; // Passed to my_task after converting to void*

    // Crate the task (just ignore the "x" prefix)
    xTaskCreate(
        my_task,          // Task function
        "CustomTaskName", // Can be anything (used for debugging, logging and error tracking)
        2048,             // Stack size (amount of memory/RAM) allocated for the task, measured in bytes
        (void *)param,    // Parameter sent to my_task
        5,                // Task priority
        NULL              // Task handle
    );

    // The "convert, then convert back" dance is necessary,
    // because xTaskCreate only accepts a generic pointer (void *).
}
