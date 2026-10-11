#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"

// Declare the mutex handle globally so both tasks can access it
SemaphoreHandle_t my_mutex = NULL;

void task_a(void *pvParameters)
{
    while (1)
    {
        // Try to take the mutex.
        // `portMAX_DELAY` is a macro that means "wait forever".
        // That means, `xSemaphoreTake()` will try to take the mutex indefinitely until it becomes available.
        if (xSemaphoreTake(my_mutex, portMAX_DELAY) == pdTRUE)
        {
            ESP_LOGI("Task A", "Working...");
            // Mimic work by delaying for 1 second
            vTaskDelay(pdMS_TO_TICKS(1000));
            ESP_LOGI("Task A", "Finished work!");

            // Give the mutex back so Task B can take it
            xSemaphoreGive(my_mutex);
        }

        // Delay a bit so task_b has a chance to grab the mutex
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void task_b(void *pvParameters)
{
    while (1)
    {
        if (xSemaphoreTake(my_mutex, portMAX_DELAY) == pdTRUE)
        {
            ESP_LOGI("Task B", "Working...");
            vTaskDelay(pdMS_TO_TICKS(1000));
            ESP_LOGI("Task B", "Finished work");
            
            xSemaphoreGive(my_mutex);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void app_main(void)
{
    // Create the mutex before launching tasks
    my_mutex = xSemaphoreCreateMutex();

    if (my_mutex != NULL)
    {
        // Start both tasks with the same priority (5)
        xTaskCreate(task_a, "Task A", 2048, NULL, 5, NULL);
        xTaskCreate(task_b, "Task B", 2048, NULL, 5, NULL);
    }
    else // When mutex creation fails due to low memory
    {
        ESP_LOGE("APP_MAIN", "Failed to create mutex");
    }
}
