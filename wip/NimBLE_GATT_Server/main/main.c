/*
 * SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
/* Includes */
#include "common.h"
#include "gap.h"
#include "gatt_svc.h"
#include "heart_rate.h"
#include "led.h"

/* Library function declarations */
void ble_store_config_init(void);

/* Private function declarations */
static void on_stack_reset(int reason);
static void on_stack_sync(void);
static void nimble_host_config_init(void);
static void nimble_host_task(void *param);

/* Private functions */
/*
 *  Stack event callback functions
 *      - on_stack_reset is called when host resets BLE stack due to errors
 *      - on_stack_sync is called when host has synced with controller
 */
static void on_stack_reset(int reason) {
  /* On reset, print reset reason to console */
  ESP_LOGI(TAG, "nimble stack reset, reset reason: %d", reason);
}

static void on_stack_sync(void) {
  /* On stack sync, do advertising initialization */
  adv_init();
}

// This was called from the original file
static void nimble_host_config_init(void) {
  /* Set host callbacks */
  // logs the reason
  ble_hs_cfg.reset_cb = on_stack_reset;

  // calls adv_init();
  ble_hs_cfg.sync_cb = on_stack_sync;

  // from gatt_svc.h
  // just for logging, you don't have to worry about this function
  ble_hs_cfg.gatts_register_cb = gatt_svr_register_cb;

  // this is also probably for logging
  // (no need to worry as of now)
  ble_hs_cfg.store_status_cb = ble_store_util_status_rr;

  /* Store host configuration */
  ble_store_config_init();
}

/**
 * @brief runs nimble
 */
static void nimble_host_task(void *param) {
  /* Task entry log */
  ESP_LOGI(TAG, "nimble host task has been started!");

  /* This function won't return until nimble_port_stop() is executed */
  nimble_port_run();

  /* Clean up at exit */
  vTaskDelete(NULL);
}

/**
 * @brief updates heart rate and sends indication
 */
static void heart_rate_task(void *param) {
  /* Task entry log */
  ESP_LOGI(TAG, "heart rate task has been started!");

  /* Loop forever */
  while (1) {
    /* Update heart rate value every 1 second */
    update_heart_rate();
    ESP_LOGI(TAG, "heart rate updated to %d", get_heart_rate());

    /* Send heart rate indication if enabled */
    send_heart_rate_indication();

    /* Sleep */
    vTaskDelay(HEART_RATE_TASK_PERIOD);
  }

  /* Clean up at exit */
  vTaskDelete(NULL);
}

void app_main(void) {
  /* Local variables */
  BaseType_t rc = 0;
  esp_err_t ret;

  /* LED initialization */
  led_init();

  /*
   * NVS flash initialization (boilerplate)
   * Dependency of BLE stack to store configurations
   */
  ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "failed to initialize nvs flash, error code: %d ", ret);
    return;
  }

  /* NimBLE stack initialization (boilerplate) */
  ret = nimble_port_init();
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "failed to initialize nimble stack, error code: %d ", ret);
    return;
  }

#if CONFIG_BT_NIMBLE_GAP_SERVICE
  /* GAP service initialization */

  // +--------------------------------------+
  // | IMPORTANT: Also sets the device name |
  // +--------------------------------------+
  rc = gap_init();
  if (rc != 0) {
    ESP_LOGE(TAG, "failed to initialize GAP service, error code: %d", rc);
    return;
  }
#endif

  /* GATT server initialization */

  // ----------------------------------------
  // MOST IMPORTANT PART
  // *******************
  // Here is where we define our GATT
  // services and characteristics.
  //-----------------------------------------
  rc = gatt_svc_init();
  if (rc != 0) {
    ESP_LOGE(TAG, "failed to initialize GATT server, error code: %d", rc);
    return;
  }

  /* NimBLE host configuration initialization */
  // Defined in this file itself.
  // Logs stuff and initalizes ble_store.
  nimble_host_config_init();

  /* Start NimBLE host task thread and return */
  // This logs the task creation and calls nimble_port_run()
  // which won't return until nimble_port_stop() is executed.
  rc = xTaskCreate(nimble_host_task, "NimBLE Host", 4 * 1024, NULL, 5, NULL);
  if (rc != pdPASS) {
    ESP_LOGE(TAG, "failed to create NimBLE host task");
    return;
  }

  // --- --- --- CHECKPOINT --- --- --- //

  rc = xTaskCreate(heart_rate_task, "Heart Rate", 4 * 1024, NULL, 5, NULL);
  if (rc != pdPASS) {
    ESP_LOGE(TAG, "failed to create heart rate task");
    return;
  }
  return;
}
