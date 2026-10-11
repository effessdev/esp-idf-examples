// Include required headers
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

#define BLINK_GPIO GPIO_NUM_22 // object-like macro to define gpio to be blinked

static const char *TAG = "MAIN"; // used for logging (inside `ESP_LOGI()`)

void app_main(void) {
  // Clean up previous configuration
  gpio_reset_pin(BLINK_GPIO);

  // Configure the pin for voltage output
  gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

  // Store the current state (0 = OFF, 1 = ON)
  uint8_t state = 0;

  ESP_LOGI("MAIN", "Starting LED...");

  // Infinite loop
  while (1) {
    // Set GPIO level (0 = OFF, 1 = ON)
    gpio_set_level(BLINK_GPIO, state);

    // Log the current state
    ESP_LOGI("MAIN", "LED State: %s", state ? "ON" : "OFF");

    // Toggle state for next iteration
    state = !state;

    // Stop execution for 1000 ms (1 second)
    // `pdMS_TO_TICKS` converts milliseconds to ticks
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
