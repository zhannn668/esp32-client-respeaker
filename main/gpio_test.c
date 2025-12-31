#include "gpio_test.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

/*
 * XIAO ESP32-S3 Pin Mapping
 * D0 -> GPIO1
 * D2 -> GPIO3
 * D3 -> GPIO4
 */
#define PIN_D0 GPIO_NUM_1
#define PIN_D2 GPIO_NUM_3
#define PIN_D3 GPIO_NUM_4

static const char *TAG = "GPIO_TEST";

void gpio_test_init(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask =
            (1ULL << PIN_D0) |
            (1ULL << PIN_D2) |
            (1ULL << PIN_D3),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    ESP_ERROR_CHECK(gpio_config(&io_conf));

    gpio_set_level(PIN_D0, 0);
    gpio_set_level(PIN_D2, 0);
    gpio_set_level(PIN_D3, 0);

    ESP_LOGW(TAG, "GPIO test init done: D0=GPIO1 D2=GPIO3 D3=GPIO4");
}

void gpio_test_blink_all(void)
{
    int level = 0;

    ESP_LOGW(TAG, "Start GPIO blink-all test");

    while (1) {
        level = !level;

        gpio_set_level(PIN_D0, level);
        gpio_set_level(PIN_D2, level);
        gpio_set_level(PIN_D3, level);

        ESP_LOGI(TAG, "ALL GPIO set to %d", level);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void gpio_test_chase(void)
{
    ESP_LOGW(TAG, "Start GPIO chase test");

    while (1) {
        gpio_set_level(PIN_D0, 1);
        gpio_set_level(PIN_D2, 0);
        gpio_set_level(PIN_D3, 0);
        ESP_LOGI(TAG, "D0=1 D2=0 D3=0");
        vTaskDelay(pdMS_TO_TICKS(400));

        gpio_set_level(PIN_D0, 0);
        gpio_set_level(PIN_D2, 1);
        gpio_set_level(PIN_D3, 0);
        ESP_LOGI(TAG, "D0=0 D2=1 D3=0");
        vTaskDelay(pdMS_TO_TICKS(400));

        gpio_set_level(PIN_D0, 0);
        gpio_set_level(PIN_D2, 0);
        gpio_set_level(PIN_D3, 1);
        ESP_LOGI(TAG, "D0=0 D2=0 D3=1");
        vTaskDelay(pdMS_TO_TICKS(400));
    }
}
