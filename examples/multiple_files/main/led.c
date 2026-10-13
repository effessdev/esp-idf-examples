#include "led.h"
#include "driver/gpio.h"
#include <stdbool.h>

#define LED_GPIO_NUM GPIO_NUM_22

// Keep hardware state internal to this file
static bool s_led_on = false;

void led_init(void) {
  gpio_config_t io_conf = {
      .pin_bit_mask = (1ULL << LED_GPIO_NUM),
      .mode = GPIO_MODE_OUTPUT,
      .pull_up_en = GPIO_PULLUP_DISABLE,
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_DISABLE,
  };
  gpio_config(&io_conf);

  s_led_on = false;
  gpio_set_level(LED_GPIO_NUM, 0);
}

void led_toggle(void) {
  s_led_on = !s_led_on;
  gpio_set_level(LED_GPIO_NUM, s_led_on ? 1 : 0);
}
