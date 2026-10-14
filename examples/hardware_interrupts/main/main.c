#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#define LED_PIN       GPIO_NUM_22
#define BUTTON_PIN    GPIO_NUM_23

static QueueHandle_t gpio_evt_queue = NULL;

// Minimal ISR: just passes the event and requests a task wakeup
static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    // Send to queue from ISR
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, &xHigherPriorityTaskWoken);
    
    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}

// Task handles the debounce by waiting out the chatter and checking the pin state
static void button_task(void* arg)
{
    uint32_t io_num;
    int led_state = 0;

    for (;;) {
        // Wait for any edge interrupt event
        if (xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            
            // Allow time for the mechanical bounce/chatter to settle (50ms)
            vTaskDelay(pdMS_TO_TICKS(50));

            // Check the actual state of the pin after settling.
            // Since it's pulled up: 
            // - HIGH means the button was RELEASED (rising edge settled)
            // - LOW means the button was PRESSED (falling edge settled)
            int pin_level = gpio_get_level(BUTTON_PIN);

            // If we want it to toggle ONLY on release (pin is now HIGH):
            if (pin_level == 1) {
                led_state = !led_state;
                gpio_set_level(LED_PIN, led_state);
            }

            // Flush any extra bounce events that piled up in the queue during the press/release
            xQueueReset(gpio_evt_queue);
        }
    }
}

void app_main(void)
{
    // Configure LED Pin as Output
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_PIN, 0);

    // Configure Button Pin as Input with Pull-Up
    gpio_reset_pin(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_pullup_en(BUTTON_PIN);
    
    // Trigger on ANY edge so we catch both press and release movements cleanly
    gpio_set_intr_type(BUTTON_PIN, GPIO_INTR_ANYEDGE);

    // Create Queue for Passing Events
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));

    // Start Button Handler Task
    xTaskCreate(button_task, "button_task", 2048, NULL, 10, NULL);

    // Install ISR Service and Add Handler
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_PIN, gpio_isr_handler, (void*) BUTTON_PIN);
}
