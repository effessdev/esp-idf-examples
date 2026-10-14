#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_timer.h"

#define LED_PIN         GPIO_NUM_22
#define BUTTON_PIN      GPIO_NUM_23
#define DEBOUNCE_TIME_US 200000 // 200 milliseconds debounce window

static int led_state = 0;
static QueueHandle_t gpio_evt_queue = NULL;

// ISR handler with debounce and queue transmission
static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    static int64_t last_interrupt_time = 0;
    int64_t current_time = esp_timer_get_time();

    // Discard interrupts coming too close together (chatter/bounce)
    if (current_time - last_interrupt_time > DEBOUNCE_TIME_US) {
        last_interrupt_time = current_time;
        uint32_t gpio_num = (uint32_t) arg;
        xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
    }
}

// Background task to process button events safely
static void button_task(void* arg)
{
    uint32_t io_num;
    for (;;) {
        // Wait indefinitely until an event is received from the queue
        if (xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            led_state = !led_state;
            gpio_set_level(LED_PIN, led_state);
            printf("Button pressed on GPIO %lu! LED state: %d\n", io_num, led_state);
        }
    }
}

void app_main(void)
{
    // Configure LED pin
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    // Configure Button pin with internal pull-up
    gpio_reset_pin(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_pullup_en(BUTTON_PIN);

    // Set interrupt type to falling edge (HIGH to LOW when pressed)
    gpio_set_intr_type(BUTTON_PIN, GPIO_INTR_NEGEDGE);

    // Create a queue to handle GPIO events from the ISR
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));

    // Create the background button handling task
    xTaskCreate(button_task, "button_task", 2048, NULL, 10, NULL);

    // Install GPIO ISR service
    gpio_install_isr_service(0);

    // Attach the ISR handler to the button pin and pass the pin number as argument
    gpio_isr_handler_add(BUTTON_PIN, gpio_isr_handler, (void*) BUTTON_PIN);
}
