#include "hardware_interrupts.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "portmacro.h"
#include <driver/gpio.h>

#define BUTTON_PIN GPIO_NUM_23
#define LED_PIN GPIO_NUM_22
#define DEBOUNCE_TIME_MS 10

// This queue stores the GPIO_NUM_n that triggered the interrupt
static QueueHandle_t gpio_evt_queue = NULL;

static void IRAM_ATTR gpio_isr_handler(void *arg) {
  uint32_t gpio_num = (uint32_t)arg;

  // Instead of xQueueSend(), we use xQueueSendFromISR() which is the
  // interrupt-safe counterpart of it.
  // xQueueSend (Task context) can block and wait for a specified timeout if the
  // queue is currently full.
  // xQueueSendFromISR (Interrupt context) never blocks. If the queue is full,
  // it immediately returns errQUEUE_FULL so the ISR can finish quickly.
  xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);

  // NOTE: Always use ...FromISR variants of FreeRTOS calls inside ISRs.
}

void toggle_led() {
  static bool led_state = false;
  led_state = !led_state;
  gpio_set_level(LED_PIN, led_state);
}

// Consume the items in the queue
static void gpio_task(void *arg) {
  uint32_t gpio_num;
  TickType_t lastWakeTime = xTaskGetTickCount();

  while (1) {
    if (xQueueReceive(gpio_evt_queue, &gpio_num, portMAX_DELAY)) {
      if (gpio_num == BUTTON_PIN) {
        if (xTaskGetTickCount() - lastWakeTime > DEBOUNCE_TIME_MS) {
          toggle_led();
        }
        lastWakeTime = xTaskGetTickCount();
      }
    }
  }
}

void hardware_interrupts_main() {
  gpio_reset_pin(BUTTON_PIN);
  gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
  gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);
  gpio_set_intr_type(BUTTON_PIN, GPIO_INTR_POSEDGE);

  gpio_reset_pin(LED_PIN);
  gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

  gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));

  xTaskCreate(gpio_task, "gpio_task", 2048, NULL, 10, NULL);

  gpio_install_isr_service(0);
  gpio_isr_handler_add(BUTTON_PIN, gpio_isr_handler, (void *)BUTTON_PIN);
}
