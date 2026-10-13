#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
#include <stdio.h>


void app_main(void) {
  led_init();

  while (1) {
    led_toggle();
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}
