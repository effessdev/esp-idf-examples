#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include <stdio.h>

#define LED_PIN 22

void blink_main(void) {
  gpio_reset_pin(LED_PIN);
  gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

  while (1) {
    gpio_set_level(LED_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(1000));
    gpio_set_level(LED_PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
