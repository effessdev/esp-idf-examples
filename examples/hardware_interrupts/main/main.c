#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define LED_PIN GPIO_NUM_2
#define BUTTON_PIN GPIO_NUM_4
#define ESP_INTR_FLAG_DEFAULT 0

static const char *TAG = "WIRE_TOUCH";

// Semaphore handle to signal the task from the ISR
static SemaphoreHandle_t xWireTouchSemaphore = NULL;

// Interrupt Service Routine (ISR) - Runs instantly on pin edge change!
static void IRAM_ATTR gpio_isr_handler(void *arg)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    // Unblock the FreeRTOS task from the ISR
    xSemaphoreGiveFromISR(xWireTouchSemaphore, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken == pdTRUE)
    {
        // Make button_task start running immediately
        portYIELD_FROM_ISR();
    }
}

// FreeRTOS Task to handle debouncing and LED toggling
static void button_task(void *arg)
{
    bool led_state = false;

    while (1)
    {
        // Wait here indefinitely until the ISR sends a signal
        if (xSemaphoreTake(xWireTouchSemaphore, portMAX_DELAY) == pdTRUE)
        {

            // Check if the wire is actually touching GND (LOW)
            if (gpio_get_level(BUTTON_PIN) == 0)
            {
                // Debounce delay: Wait 50ms for wire contact noise to settle
                vTaskDelay(pdMS_TO_TICKS(50));

                // Re-check after delay to make sure it's still touching GND
                if (gpio_get_level(BUTTON_PIN) == 0)
                {
                    led_state = !led_state;
                    gpio_set_level(LED_PIN, led_state);
                    ESP_LOGI(TAG, "Wire touched GND! LED Toggled to: %s", led_state ? "ON" : "OFF");

                    // Wait until the wire is disconnected from GND before accepting new touches
                    while (gpio_get_level(BUTTON_PIN) == 0)
                    {
                        vTaskDelay(pdMS_TO_TICKS(10));
                    }
                }
            }
        }
    }
}

void app_main(void)
{
    // 1. Create a Binary Semaphore
    xWireTouchSemaphore = xSemaphoreCreateBinary();

    // 2. Configure the LED Pin (Output)
    gpio_config_t led_conf = {
        .pin_bit_mask = (1ULL << LED_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&led_conf);

    // 3. Configure the Touch Wire Pin (Input with Internal Pull-Up)
    gpio_config_t wire_conf = {
        .pin_bit_mask = (1ULL << BUTTON_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE, // Holds pin HIGH (3.3V) when untouched
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE, // Trigger interrupt when going HIGH -> LOW
    };
    gpio_config(&wire_conf);

    // 4. Create the handling task
    xTaskCreate(button_task, "button_task", 2048, NULL, 10, NULL);

    // 5. Install GPIO ISR Service and attach Handler
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(BUTTON_PIN, gpio_isr_handler, (void *)BUTTON_PIN);

    ESP_LOGI(TAG, "System Ready! Touch GPIO4 wire to GND pin to toggle LED.");
}
