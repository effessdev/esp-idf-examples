#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

#define LED_GPIO GPIO_NUM_22

// Define the task
void blink_task(void *pvParameter) {
  while (1) {
    gpio_set_level(LED_GPIO, 1);
    ESP_LOGI("LED", "LED turned ON!");
    vTaskDelay(pdMS_TO_TICKS(1000));

    gpio_set_level(LED_GPIO, 0);
    ESP_LOGI("LED", "LED turned OFF!");
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void app_main(void) {
  gpio_reset_pin(LED_GPIO);
  gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);

  // Start the task
  xTaskCreate(blink_task,   /* Function pointer to the task */
              "blink_task", /* Descriptive name for debugging */
              2048,         /* Stack size allocated for the task in bytes */
              NULL, /* Pointer to input parameters passed to the task (NULL
                       since we aren't giving any input) */
              1,    /* Priority of the task (higher number = higher priority) */
              NULL  /* Task handle to track or control the task later (we aren't
                       using this as of now) */
  );

  // Now you can do anything else here.
  // `blink_task` will run in parallel.
}
