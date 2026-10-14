#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

// object-like macro to define the GPIO to be blinked
#define BLINK_GPIO GPIO_NUM_22

// Used for logging
static const char *TAG = "MAIN";

void app_main(void) {
  gpio_reset_pin(BLINK_GPIO);
  gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

  ESP_LOGI(TAG, "Starting LED...");

  while (1) {
    gpio_set_level(BLINK_GPIO, 1);
    ESP_LOGI(TAG, "LED State: ON");
    vTaskDelay(pdMS_TO_TICKS(1000));

    gpio_set_level(BLINK_GPIO, 0);
    ESP_LOGI(TAG, "LED State: OFF");
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
