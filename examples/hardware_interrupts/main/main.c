
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#define LED_PIN       GPIO_NUM_22
#define BUTTON_PIN    GPIO_NUM_23

// Queue to send button events from ISR to the task
static QueueHandle_t gpio_evt_queue = NULL;

// ISR keeps work minimal (add the event to the queue)
static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

// Task to handle button presses from the queue
static void button_task(void* arg)
{
    uint32_t io_num;
    int led_state = 0;

    for (;;) {
        // xQueueRecieve recieves the first item added to the queue.
        // If there are no items, it waits until one is added.
        if (xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            // Toggle LED state on button press event
            led_state = !led_state;
            gpio_set_level(LED_PIN, led_state);
            
            // Simple software debounce delay
            vTaskDelay(pdMS_TO_TICKS(200));
        }
    }
}

void app_main(void)
{
    // Configure LED Pin as Output
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    // Configure Button Pin as Input with Pull-Up and Interrupt
    gpio_reset_pin(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_pullup_en(BUTTON_PIN);
    gpio_set_intr_type(BUTTON_PIN, GPIO_INTR_NEGEDGE);

    // Create Queue for Passing Events
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));

    // Start Button Handler Task
    xTaskCreate(button_task, "button_task", 2048, NULL, 10, NULL);

    // Install ISR Service and Add Handler
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_PIN, gpio_isr_handler, (void*) BUTTON_PIN);
}
