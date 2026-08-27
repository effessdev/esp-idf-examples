/* Includes */
/* STD APIs */
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* ESP APIs */
#include "driver/gpio.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "sdkconfig.h"

/* FreeRTOS APIs */
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

/* NimBLE stack APIs */
#include "host/ble_hs.h"
#include "host/ble_uuid.h"
#include "host/util/util.h"
#include "nimble/ble.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"

/* Defines */
#define TAG "NimBLE_GATT_Server"
#define DEVICE_NAME "NimBLE_GATT"

static const ble_uuid16_t auto_io_svc_uuid = BLE_UUID16_INIT(0x1815);
static const ble_uuid16_t led_chr_uuid = BLE_UUID16_INIT(0x2A56);

static uint16_t led_chr_val_handle;
static int led_chr_access(uint16_t conn_handle, uint16_t attr_handle,
                          struct ble_gatt_access_ctxt *ctxt, void *arg);

/* GATT services table */
static const struct ble_gatt_svc_def gatt_svr_svcs[] = {
    /* Automation IO service */
    {.type = BLE_GATT_SVC_TYPE_PRIMARY,
     .uuid = &auto_io_svc_uuid.u,
     .characteristics =
         (struct ble_gatt_chr_def[]){
             {/* LED characteristic */
              .uuid = &led_chr_uuid.u,
              .access_cb = led_chr_access,
              .flags = BLE_GATT_CHR_F_WRITE,
              .val_handle = &led_chr_val_handle},
             {
                 0, /* No more characteristics in this service. */
             }}},

    {
        0, /* No more services. */
    },
};

static int led_chr_access(uint16_t conn_handle, uint16_t attr_handle,
                          struct ble_gatt_access_ctxt *ctxt, void *arg) {
  if (ctxt->op == BLE_GATT_ACCESS_OP_WRITE_CHR) {
    // Extract the incoming byte payload
    uint8_t val = ctxt->om->om_data[0];

    if (val == 1) {
      gpio_set_level(GPIO_NUM_2, 1);
      ESP_LOGI(TAG, "LED turned ON");
    } else if (val == 0) {
      gpio_set_level(GPIO_NUM_2, 0);
      ESP_LOGI(TAG, "LED turned OFF");
    } else {
      ESP_LOGW(TAG, "Invalid payload value received: %d", val);
    }
    return 0;
  }

  return BLE_ATT_ERR_UNLIKELY;
}

void app_main(void) {
  esp_err_t ret; // To store the return code

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

  ret = nimble_port_init();
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "failed to initialize nimble stack, error code: %d ", ret);
    return;
  }

  BaseType_t rc = 0; // Still just an int; "rc" stands for "return code"

  ble_svc_gap_init();

  rc = ble_svc_gap_device_name_set(DEVICE_NAME);
  if (rc != 0) {
    ESP_LOGE(TAG, "failed to set device name to %s, error code: %d",
             DEVICE_NAME, rc);
    return;
  }

  ble_svc_gatt_init();

  rc = ble_gatts_count_cfg(gatt_svr_svcs);
  if (rc != 0) {
    ESP_LOGE(TAG, "failed to count gatt services, error code: %d", rc);
    return;
  }

  rc = ble_gatts_add_svcs(gatt_svr_svcs);
  if (rc != 0) {
    ESP_LOGE(TAG, "failed to add gatt services, error code: %d", rc);
    return;
  }
}