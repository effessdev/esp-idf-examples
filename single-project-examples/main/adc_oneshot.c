#include "adc_oneshot.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

// ESP32 has ADC 1 and ADC 2.
// ADC2 should only be used when Wi-Fi and Bluetooth are completely disabled.
#define POT_ADC_UNIT ADC_UNIT_1

// ADC1 has 8 channels (Channels 0 through 7), which map to various GPIO pins
// like GPIO 32, 33, 34, 35, 36, and 39.
// ADC2 has 10 channels (Channels 0 through 9), which map to other pins like
// GPIO 4, 0, 2, 15, 13, 12, 14, 27, 25, and 26.
// Channel 3 maps to GPIO 39 (labelled as VN).
#define POT_ADC_CHANNEL ADC_CHANNEL_3

// Atten stands for attenuation and db stands for decibel.
// -------------------
// VALUE | MAX VOLTAGE
// -------------------
// 0     | 1.1V
// 2_5   | 1.5V
// 6     | 2.2V
// 12    | 3.3V
#define POT_ADC_ATTEN ADC_ATTEN_DB_12

static const char *TAG = "POT_POLL";

void app_main(void) {
  adc_oneshot_unit_handle_t adc_handle;
  adc_oneshot_unit_init_cfg_t init_config = {
      .unit_id = POT_ADC_UNIT,
      .ulp_mode = ADC_ULP_MODE_DISABLE,
  };
  ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc_handle));

  adc_oneshot_chan_cfg_t chan_config = {
      .bitwidth = ADC_BITWIDTH_DEFAULT, // Typically 12-bit (0 to 4095)
      .atten = POT_ADC_ATTEN,
  };
  ESP_ERROR_CHECK(
      adc_oneshot_config_channel(adc_handle, POT_ADC_CHANNEL, &chan_config));

  int raw_value = 0;

  while (1) {
    ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, POT_ADC_CHANNEL, &raw_value));

    ESP_LOGI(TAG, "Potentiometer Raw Value: %d", raw_value);

    vTaskDelay(pdMS_TO_TICKS(250));
  }
}