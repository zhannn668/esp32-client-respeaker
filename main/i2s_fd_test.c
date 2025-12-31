
#include "driver/i2s_std.h"
#include "esp_log.h"
#include "freertos/portmacro.h"
#include "i2s_fd_test.h"

static const char *TAG = "I2S_FD";

static i2s_chan_handle_t tx_chan = NULL;
static i2s_chan_handle_t rx_chan = NULL;

void i2s_full_duplex_init(void)
{   
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
    ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, &tx_chan, &rx_chan));

    i2s_std_config_t std_cfg = {
        .clk_cfg  = I2S_STD_CLK_DEFAULT_CONFIG(16000),
        .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_32BIT, I2S_SLOT_MODE_STEREO),
        .gpio_cfg = {
            .mclk = I2S_GPIO_UNUSED,     // 先不管 MCLK，按你 Arduino 例程的行为
            .bclk = GPIO_NUM_8,
            .ws   = GPIO_NUM_7,
            .dout = GPIO_NUM_44,
            .din  = GPIO_NUM_43,
            .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv   = false,
            },
        },
    };

    ESP_ERROR_CHECK(i2s_channel_init_std_mode(tx_chan, &std_cfg));
    ESP_ERROR_CHECK(i2s_channel_init_std_mode(rx_chan, &std_cfg));

    ESP_ERROR_CHECK(i2s_channel_enable(tx_chan));
    ESP_ERROR_CHECK(i2s_channel_enable(rx_chan));

    ESP_LOGW(TAG, "I2S FD init done (16k, stereo, 32bit)");
}

void i2s_fd_squarewave_test(void)
{
    const int sampleRate = 16000;
    const int freq = 440;
    const int halfWavelength = sampleRate / freq;
    const int32_t amplitude = 500;

    int32_t sample = amplitude;
    int count = 0;

    // 写 32000 个 sample（注意：这里是“一个 32bit 值”，实际 stereo 时你可能要写 L/R）
    for (int i = 0; i < 32000; i++) {
        if (count % halfWavelength == 0) sample = -sample;
        // stereo：写成 L/R 两个 32bit
        int32_t frame[2] = { sample, sample };
        size_t w = 0;
        ESP_ERROR_CHECK(i2s_channel_write(tx_chan, frame, sizeof(frame), &w, portMAX_DELAY));
        count++;
    }

    // 读 32000 个 frame，统计非全0/非全1
    int nonZero = 0;
    bool truncated = false;
    for (int i = 0; i < 32000; i++) {
        int32_t frame[2] = {0};
        size_t r = 0;
        ESP_ERROR_CHECK(i2s_channel_read(rx_chan, frame, sizeof(frame), &r, portMAX_DELAY));
        // 简单判定：任意声道有“正常值”
        for (int ch = 0; ch < 2; ch++) {
            int32_t v = frame[ch];
            if (v != 0 && v != (int32_t)0xFFFFFFFF) nonZero++;
            if (i < 50 && !truncated) {
                // 打印前 50 帧
                // ESP_LOGI(TAG, "%ld", (long)v);
            }
        }
        if (i == 50 && !truncated) truncated = true;
    }

    ESP_LOGW(TAG, "Valid samples count=%d (threshold~16000)", nonZero);
}
