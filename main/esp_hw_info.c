// SPDX-License-Identifier: GPL-3.0-or-later
#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "esp_chip_info.h"
#include "esp_system.h"
#include "hal/efuse_hal.h"
#include "esp_console.h"

#define TAG "info"
#define BUFF_LEN 16
#define CONSOLE_CMD_MAX_LEN 128

static void p_map_chip_model(char *buff, esp_chip_info_t *info) {
    memset(buff, 0, BUFF_LEN);
    switch (info->model) {
        case CHIP_ESP32: {
            strncpy(buff, "ESP32", (BUFF_LEN - 1));
            break;
        }
        case CHIP_ESP32S2: {
            strncpy(buff, "ESP32-S2", (BUFF_LEN - 1));
            break;
        }
        case CHIP_ESP32S3: {
            strncpy(buff, "ESP32-S3", (BUFF_LEN - 1));
            break;
        }
        case CHIP_ESP32C2: {
            strncpy(buff, "ESP32-C2", (BUFF_LEN - 1));
            break;
        }
        case CHIP_ESP32C3: {
            strncpy(buff, "ESP32-C3", (BUFF_LEN - 1));
            break;
        }
        case CHIP_ESP32C5: {
            strncpy(buff, "ESP32-C5", (BUFF_LEN - 1));
            break;
        }
        case CHIP_ESP32C6: {
            strncpy(buff, "ESP32-C6", (BUFF_LEN - 1));
            break;
        }
        case CHIP_ESP32H2: {
            strncpy(buff, "ESP32-H2", (BUFF_LEN - 1));
            break;
        }
        case CHIP_ESP32P4: {
            strncpy(buff, "ESP32-P4", (BUFF_LEN - 1));
            break;
        }
        case CHIP_POSIX_LINUX: {
            strncpy(buff, "ESP32 sim", (BUFF_LEN - 1));
            break;
        }
        default: {
            strncpy(buff, "???", (BUFF_LEN - 1));
            break;
        }
    }
}

static void p_map_reset_reason(char *buff, esp_reset_reason_t rr) {
    memset(buff, 0, BUFF_LEN);
    switch (rr) {
        case ESP_RST_POWERON: {
            strncpy(buff, "power-on", (BUFF_LEN - 1));
            break;
        }
        case ESP_RST_EXT: {
            strncpy(buff, "ext", (BUFF_LEN - 1));
            break;
        }
        case ESP_RST_SW: {
            strncpy(buff, "SW", (BUFF_LEN - 1));
            break;
        }
        case ESP_RST_PANIC: {
            strncpy(buff, "PANIC", (BUFF_LEN - 1));
            break;
        }
        case ESP_RST_INT_WDT: {
            strncpy(buff, "WDT (int)", (BUFF_LEN - 1));
            break;
        }
        case ESP_RST_TASK_WDT: {
            strncpy(buff, "WDT (task)", (BUFF_LEN - 1));
            break;
        }
        case ESP_RST_WDT: {
            strncpy(buff, "WDT (other)", (BUFF_LEN - 1));
            break;
        }
        case ESP_RST_DEEPSLEEP: {
            strncpy(buff, "deep sleep", (BUFF_LEN - 1));
            break;
        }
        case ESP_RST_BROWNOUT: {
            strncpy(buff, "brown-out", (BUFF_LEN - 1));
            break;
        }
        case ESP_RST_SDIO: {
            strncpy(buff, "SDIO", (BUFF_LEN - 1));
            break;
        }
        case ESP_RST_USB: {
            strncpy(buff, "USB", (BUFF_LEN - 1));
            break;
        }
        case ESP_RST_JTAG: {
            strncpy(buff, "JTAG", (BUFF_LEN - 1));
            break;
        }
        default: {
            strncpy(buff, "???", (BUFF_LEN - 1));
            break;
        }
    }
}

static char p_buff[BUFF_LEN] = "";

static int do_lscpu_cmd(int argc, char **argv) {
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);

    p_map_chip_model(p_buff, &chip_info);
    printf("chip: %s\n", p_buff);
    printf("  rev:        %u\n", chip_info.revision);
    printf("  cores:      %u\n", chip_info.cores);

    if (chip_info.features & CHIP_FEATURE_EMB_FLASH) {
        printf("  flash:      yes\n");
    } else {
        printf("  flash:      no\n");
    }
    if (chip_info.features & CHIP_FEATURE_EMB_PSRAM) {
        printf("  PSRAM:      yes\n");
    } else {
        printf("  PSRAM:      no\n");
    }
    if (chip_info.features & CHIP_FEATURE_WIFI_BGN) {
        printf("  Wi-Fi:      yes\n");
    } else {
        printf("  Wi-Fi:      no\n");
    }
    if (chip_info.features & CHIP_FEATURE_BT) {
        printf("  BT:         yes\n");
    } else {
        printf("  BT:         no\n");
    }
    if (chip_info.features & CHIP_FEATURE_BLE) {
        printf("  BLE:        yes\n");
    } else {
        printf("  BLE:        no\n");
    }
    if (chip_info.features & CHIP_FEATURE_IEEE802154) {
        printf("  LR-WPAN:    yes\n");
    } else {
        printf("  LR-WPAN:    no\n");
    }

    esp_reset_reason_t rst_reason = esp_reset_reason();

    p_map_reset_reason(p_buff, rst_reason);
    printf("last reset: %s\n", p_buff);
    return 0;
}

static int do_lssoc_cmd(int argc, char **argv) {
    return 0;
}

static int do_lsid_cmd(int argc, char **argv) {
    uint8_t mac[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    efuse_hal_get_mac(mac);
    printf("eFuse MAC (%x:%x:)%x:%x:%x:%x:%x:%x\n",
           mac[7], mac[6], mac[5], mac[4], mac[3], mac[2], mac[1], mac[0]);
    return 0;
}

static void p_register_console_cmd() {
    const esp_console_cmd_t lscpu_cmd = {
        .command = "lscpu",
        .help = "Show CPU information",
        .hint = NULL,
        .func = &do_lscpu_cmd,
        .argtable = NULL
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&lscpu_cmd));

    const esp_console_cmd_t lssoc_cmd = {
        .command = "lssoc",
        .help = "Show SoC information",
        .hint = NULL,
        .func = &do_lssoc_cmd,
        .argtable = NULL
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&lssoc_cmd));

    const esp_console_cmd_t lsid_cmd = {
        .command = "lsid",
        .help = "Show ID (MAC)",
        .hint = NULL,
        .func = &do_lsid_cmd,
        .argtable = NULL
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&lsid_cmd));
}

void app_main(void) {
    esp_console_repl_t *repl = NULL;
    esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
    repl_config.prompt = CONFIG_IDF_TARGET ">";
    repl_config.max_cmdline_length = CONSOLE_CMD_MAX_LEN;

#if defined(CONFIG_ESP_CONSOLE_UART_DEFAULT) || defined(CONFIG_ESP_CONSOLE_UART_CUSTOM)
    esp_console_dev_uart_config_t hw_config = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_console_new_repl_uart(&hw_config, &repl_config, &repl));
#elif defined(CONFIG_ESP_CONSOLE_USB_CDC)
    esp_console_dev_usb_cdc_config_t hw_config = ESP_CONSOLE_DEV_CDC_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_console_new_repl_usb_cdc(&hw_config, &repl_config, &repl));
#elif defined(CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG)
    esp_console_dev_usb_serial_jtag_config_t hw_config = ESP_CONSOLE_DEV_USB_SERIAL_JTAG_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_console_new_repl_usb_serial_jtag(&hw_config, &repl_config, &repl));
#else
#error Unsupported console type
#endif
    p_register_console_cmd();
    ESP_ERROR_CHECK(esp_console_start_repl(repl));
}
