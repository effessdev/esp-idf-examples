#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_timer.h"

#define LED_PIN       GPIO_NUM_22
#define BUTTON_PIN    GPIO_NUM_23
#define DEBOUNCE_TIME_US 200000  // 200ms debounce threshold in microseconds

// Queue to send button events from ISR to the task
static QueueHandle_t gpio_evt_queue = NULL;

// ISR with timestamp-based debouncing
static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    
    // Get current time in microseconds
    static int64_t last_interrupt_time = 0;
    int64_t current_time = esp_timer_get_time();

    // If interrupts are coming in faster than the debounce window, ignore them
    if (current_time - last_interrupt_time > DEBOUNCE_TIME_US) {
        last_interrupt_time = current_time;
        xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
    }
}

// Task to handle button presses from the queue
static void button_task(void* arg)
{
    uint32_t io_num;
    int led_state = 0;

    for (;;) {
        if (xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            // Toggle LED state when the button is released
            led_state = !led_state;
            gpio_set_level(LED_PIN, led_state);
        }
    }
}

void app_main(void)
{
    // Configure LED Pin as Output
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    // Configure Button Pin as Input with Pull-Up
    gpio_reset_pin(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_pullup_en(BUTTON_PIN);
    
    // Trigger on POSitive EDGE (Rising edge: when button is released and pin goes back HIGH)
    gpio_set_intr_type(BUTTON_PIN, GPIO_INTR_POSEDGE);

    // Create Queue for Passing Events
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));

    // Start Button Handler Task
    xTaskCreate(button_task, "button_task", 2048, NULL, 10, NULL);

    // Install ISR Service and Add Handler
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_PIN, gpio_isr_handler, (void*) BUTTON_PIN);
}
