#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

// Declare the mutex handle globally so both tasks can access it
SemaphoreHandle_t print_mutex = NULL;

void task_a(void *pvParameters)
{
    while (1)
    {
        // 1. Try to take the mutex. Wait forever (portMAX_DELAY) if Task B has it.
        if (xSemaphoreTake(print_mutex, portMAX_DELAY) == pdTRUE)
        {

            // --- CRITICAL SECTION START ---
            // Only ONE task can execute code inside here at any given time.
            printf("\n[TASK A] Working...\n");
            vTaskDelay(pdMS_TO_TICKS(1000));
            printf("[TASK A] Finished work!\n");
            // --- CRITICAL SECTION END ---

            // 2. Always give the mutex back so Task B can use the printer!
            xSemaphoreGive(print_mutex);
        }

        // Delay so Task B has a chance to grab the mutex
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void task_b(void *pvParameters)
{
    while (1)
    {
        // 1. Try to take the mutex.
        if (xSemaphoreTake(print_mutex, portMAX_DELAY) == pdTRUE)
        {

            // --- CRITICAL SECTION START ---
            printf("\n  [TASK B] >>> Working...\n");
            vTaskDelay(pdMS_TO_TICKS(1000));
            printf("  [TASK B] >>> Finished work!\n");
            // --- CRITICAL SECTION END ---

            // 2. Give the mutex back
            xSemaphoreGive(print_mutex);
        }

        // Delay so Task A has a chance to grab the mutex
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void app_main(void)
{
    // Step 1: Create the mutex before launching tasks
    print_mutex = xSemaphoreCreateMutex();

    if (print_mutex != NULL)
    {
        // Step 2: Create two tasks with identical priority (5)
        xTaskCreate(task_a, "Task A", 2048, NULL, 5, NULL);
        xTaskCreate(task_b, "Task B", 2048, NULL, 5, NULL);
    }
}
