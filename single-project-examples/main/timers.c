#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"

#define LED_GPIO GPIO_NUM_22

static const char *TAG = "MAIN";
static bool led_state = false;

// The function to be called every time the timer fires
static void toggle_led(void *arg) {
  led_state = !led_state;
  gpio_set_level(LED_GPIO, led_state);
  ESP_LOGI(TAG, "LED toggled to: %d", led_state);
}

void timers_main(void) {
  gpio_reset_pin(LED_GPIO);
  gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);

  // Set up the timer creation arguments
  const esp_timer_create_args_t timer_args = {
      .callback = &toggle_led,   // Called when the timer expires
      .name = "toggle_led_timer" // For debugging purposes only
  };

  // Handle to store the timer instance
  esp_timer_handle_t led_timer;

  // Create the timer instance using timer_args
  ESP_ERROR_CHECK(esp_timer_create(&timer_args, &led_timer));

  // Start the timer in periodic mode with a 1-second interval (1,000,000
  // microseconds)
  ESP_ERROR_CHECK(esp_timer_start_periodic(led_timer, 1000000));
}
