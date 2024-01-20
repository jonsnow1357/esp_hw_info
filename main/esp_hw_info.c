// SPDX-License-Identifier: GPL-3.0-or-later
#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "esp_chip_info.h"
#include "esp_system.h"
#include "hal/efuse_hal.h"

#define TAG "info"
#define DESC_LEN 16

static void p_map_chip_model(char *desc, esp_chip_info_t *info) {
    memset(desc, 0, DESC_LEN);
    switch (info->model) {
        case CHIP_ESP32: {
            strncpy(desc, "ESP32", (DESC_LEN - 1));
            break;
        }
        case CHIP_ESP32S2: {
            strncpy(desc, "ESP32-S2", (DESC_LEN - 1));
            break;
        }
        case CHIP_ESP32S3: {
            strncpy(desc, "ESP32-S3", (DESC_LEN - 1));
            break;
        }
        case CHIP_ESP32C2: {
            strncpy(desc, "ESP32-C2", (DESC_LEN - 1));
            break;
        }
        case CHIP_ESP32C3: {
            strncpy(desc, "ESP32-C3", (DESC_LEN - 1));
            break;
        }
        case CHIP_ESP32C5: {
            strncpy(desc, "ESP32-C5", (DESC_LEN - 1));
            break;
        }
        case CHIP_ESP32C6: {
            strncpy(desc, "ESP32-C6", (DESC_LEN - 1));
            break;
        }
        case CHIP_ESP32H2: {
            strncpy(desc, "ESP32-H2", (DESC_LEN - 1));
            break;
        }
        case CHIP_ESP32P4: {
            strncpy(desc, "ESP32-P4", (DESC_LEN - 1));
            break;
        }
        case CHIP_POSIX_LINUX: {
            strncpy(desc, "ESP32 sim", (DESC_LEN - 1));
            break;
        }
        default: {
            strncpy(desc, "???", (DESC_LEN - 1));
            break;
        }
    }
}

static void p_map_reset_reason(char *desc, esp_reset_reason_t rr) {
    memset(desc, 0, DESC_LEN);
    switch (rr) {
        case ESP_RST_POWERON: {
            strncpy(desc, "power-on", (DESC_LEN - 1));
            break;
        }
        case ESP_RST_EXT: {
            strncpy(desc, "ext", (DESC_LEN - 1));
            break;
        }
        case ESP_RST_SW: {
            strncpy(desc, "SW", (DESC_LEN - 1));
            break;
        }
        case ESP_RST_PANIC: {
            strncpy(desc, "PANIC", (DESC_LEN - 1));
            break;
        }
        case ESP_RST_INT_WDT: {
            strncpy(desc, "WDT (int)", (DESC_LEN - 1));
            break;
        }
        case ESP_RST_TASK_WDT: {
            strncpy(desc, "WDT (task)", (DESC_LEN - 1));
            break;
        }
        case ESP_RST_WDT: {
            strncpy(desc, "WDT (other)", (DESC_LEN - 1));
            break;
        }
        case ESP_RST_DEEPSLEEP: {
            strncpy(desc, "deep sleep", (DESC_LEN - 1));
            break;
        }
        case ESP_RST_BROWNOUT: {
            strncpy(desc, "brown-out", (DESC_LEN - 1));
            break;
        }
        case ESP_RST_SDIO: {
            strncpy(desc, "SDIO", (DESC_LEN - 1));
            break;
        }
        case ESP_RST_USB: {
            strncpy(desc, "USB", (DESC_LEN - 1));
            break;
        }
        case ESP_RST_JTAG: {
            strncpy(desc, "JTAG", (DESC_LEN - 1));
            break;
        }
        default: {
            strncpy(desc, "???", (DESC_LEN - 1));
            break;
        }
    }
}

void app_main(void) {
    char desc[DESC_LEN] = "";
    esp_chip_info_t chip_info;
    uint8_t mac[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    esp_chip_info(&chip_info);
    p_map_chip_model(desc, &chip_info);
    ESP_LOGI(TAG, "chip %s (rev: %u, cores %u)", desc, chip_info.revision,
             chip_info.cores);
    if (chip_info.features & CHIP_FEATURE_EMB_FLASH) {
        ESP_LOGI(TAG, "  flash     [x]");
    } else {
        ESP_LOGI(TAG, "  flash     [ ]");
    }
    if (chip_info.features & CHIP_FEATURE_EMB_PSRAM) {
        ESP_LOGI(TAG, "  PSRAM     [x]");
    } else {
        ESP_LOGI(TAG, "  PSRAM     [ ]");
    }
    if (chip_info.features & CHIP_FEATURE_WIFI_BGN) {
        ESP_LOGI(TAG, "  Wi-Fi     [x]");
    } else {
        ESP_LOGI(TAG, "  Wi-Fi     [ ]");
    }
    if (chip_info.features & CHIP_FEATURE_BT) {
        ESP_LOGI(TAG, "  Bluetooth [x]");
    } else {
        ESP_LOGI(TAG, "  Bluetooth [ ]");
    }
    if (chip_info.features & CHIP_FEATURE_BLE) {
        ESP_LOGI(TAG, "  BLE       [x]");
    } else {
        ESP_LOGI(TAG, "  BLE       [ ]");
    }
    if (chip_info.features & CHIP_FEATURE_IEEE802154) {
        ESP_LOGI(TAG, "  LR-WPAN   [x]");
    } else {
        ESP_LOGI(TAG, "  LR-WPAN   [ ]");
    }

    efuse_hal_get_mac(mac);
    ESP_LOGI(TAG, "eFuse MAC (%x:%x:)%x:%x:%x:%x:%x:%x", mac[7], mac[6], mac[5],
             mac[4], mac[3], mac[2], mac[1], mac[0]);

    esp_reset_reason_t rst_reason = esp_reset_reason();
    p_map_reset_reason(desc, rst_reason);
    ESP_LOGI(TAG, "last reset - %s", desc);
}
