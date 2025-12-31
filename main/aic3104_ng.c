#include "aic3104_ng.h"
#include "esp_log.h"

static const char *TAG = "AIC3104_NG";

esp_err_t aic3104_ng_init(aic3104_ng_t *ctx, int i2c_port, int sda_gpio, int scl_gpio, uint32_t speed_hz)
{
    if (!ctx) return ESP_ERR_INVALID_ARG;

    ctx->bus = NULL;
    ctx->dev = NULL;
    ctx->i2c_port = i2c_port;
    ctx->sda_gpio = sda_gpio;
    ctx->scl_gpio = scl_gpio;
    ctx->speed_hz = speed_hz ? speed_hz : 100000;

    i2c_master_bus_config_t bus_cfg = {
        .i2c_port = i2c_port,
        .sda_io_num = sda_gpio,
        .scl_io_num = scl_gpio,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .intr_priority = 0,
        .trans_queue_depth = 0,
        .flags.enable_internal_pullup = 1,
    };

    esp_err_t ret = i2c_new_master_bus(&bus_cfg, &ctx->bus);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "i2c_new_master_bus failed: %s", esp_err_to_name(ret));
        return ret;
    }

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = AIC3104_ADDR,
        .scl_speed_hz = ctx->speed_hz,
    };

    ret = i2c_master_bus_add_device(ctx->bus, &dev_cfg, &ctx->dev);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "i2c_master_bus_add_device(0x%02X) failed: %s", AIC3104_ADDR, esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGW(TAG, "init done: port=%d SDA=%d SCL=%d speed=%lu", i2c_port, sda_gpio, scl_gpio, (unsigned long)ctx->speed_hz);
    return ESP_OK;
}

esp_err_t aic3104_ng_write(aic3104_ng_t *ctx, uint8_t reg, uint8_t val)
{
    if (!ctx || !ctx->dev) return ESP_ERR_INVALID_STATE;
    uint8_t buf[2] = { reg, val };
    return i2c_master_transmit(ctx->dev, buf, sizeof(buf), 50);
}

esp_err_t aic3104_ng_read(aic3104_ng_t *ctx, uint8_t reg, uint8_t *val)
{
    if (!ctx || !ctx->dev || !val) return ESP_ERR_INVALID_ARG;

    esp_err_t ret = i2c_master_transmit(ctx->dev, &reg, 1, 50);
    if (ret != ESP_OK) return ret;

    return i2c_master_receive(ctx->dev, val, 1, 50);
}

esp_err_t aic3104_ng_probe(aic3104_ng_t *ctx, uint8_t *page_val_out)
{
    if (!ctx) return ESP_ERR_INVALID_ARG;

    ESP_LOGW(TAG, "probe: write page 0");
    esp_err_t ret = aic3104_ng_write(ctx, AIC3104_PAGE_CTRL, 0x00);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "write page failed: %s", esp_err_to_name(ret));
        return ret;
    }

    uint8_t v = 0xFF;
    ret = aic3104_ng_read(ctx, AIC3104_PAGE_CTRL, &v);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "read page failed: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGW(TAG, "probe ok: page reg=0x%02X", v);
    if (page_val_out) *page_val_out = v;
    return ESP_OK;
}

esp_err_t aic3104_ng_setup_default(aic3104_ng_t *ctx)
{
    if (!ctx) return ESP_ERR_INVALID_ARG;

    esp_err_t ret;

    // page 0
    ret = aic3104_ng_write(ctx, AIC3104_PAGE_CTRL, 0x00);
    if (ret != ESP_OK) return ret;

    // 0dB DAC
    ret = aic3104_ng_write(ctx, AIC3104_LEFT_DAC_VOLUME, 0x00);
    if (ret != ESP_OK) return ret;
    ret = aic3104_ng_write(ctx, AIC3104_RIGHT_DAC_VOLUME, 0x00);
    if (ret != ESP_OK) return ret;

    // outputs to 0dB, unmuted, powered up (same values as your Arduino sketch)
    ret = aic3104_ng_write(ctx, AIC3104_HPLOUT_LEVEL, 0x0D);
    if (ret != ESP_OK) return ret;
    ret = aic3104_ng_write(ctx, AIC3104_HPROUT_LEVEL, 0x0D);
    if (ret != ESP_OK) return ret;

    ret = aic3104_ng_write(ctx, AIC3104_LEFT_LOP_LEVEL, 0x0B);
    if (ret != ESP_OK) return ret;
    ret = aic3104_ng_write(ctx, AIC3104_RIGHT_LOP_LEVEL, 0x0B);
    if (ret != ESP_OK) return ret;

    ESP_LOGW(TAG, "default setup applied");
    return ESP_OK;
}
