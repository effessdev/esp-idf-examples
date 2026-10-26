#include "stack_overflow.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "STACK_OVERFLOW";
static const int WAIT_TIME_SEC = 25;

void cause_stack_overflow(int depth) {
  // 256 bytes of stack space is allocated for the following array
  char buffer[256];

  // Store the depth in the array (so the compiler won't optimize it away)
  buffer[0] = (char)depth;

  if (depth % 10 == 0) {
    ESP_LOGI(TAG, "Recursion depth: %d", depth);
  }

  // Exit at a long depth (if we don't exist, IntelliSense may complain).
  // In practice, this will never be reached because the stack will overflow
  // long before then
  if (depth > 1000000) {
    ESP_LOGI(TAG, "Exiting recursion");
    return;
  }

  // Recursive call
  cause_stack_overflow(depth + 1);
}

void stack_overflow_main() {
  ESP_LOGI(TAG, "Stack overflow starting in:");

  // Count down
  for (int i = 0; i < WAIT_TIME_SEC; i++) {
    ESP_LOGI(TAG, "%d...", WAIT_TIME_SEC - i);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }

  cause_stack_overflow(1);
}