# ESP32 ReSpeaker Audio Capture (Test Branch)

*English | [简体中文](README.zh-CN.md)*

---

## Key Modified File (MOST IMPORTANT)

> **All GPIO remapping in this project is NOT done at application level.**  
> Instead, it is achieved by **directly modifying the ESP-ADF official board configuration**.

```
components/third_party/esp-adf/components/audio_board/
└── esp32_s3_korvo2_v3/
    └── board_pins_config.c   ← CORE modification of this project
```

This project **still uses the `esp32_s3_korvo2_v3` board** from ESP-ADF.

By **directly modifying `board_pins_config.c`**, the official board is adapted to:

> **Seeed Studio XIAO ESP32‑S3 + ReSpeaker 4‑Mic Array**

No new board is created.  
No ESP-ADF framework logic is changed.

---

## Project Overview

This is a **Bring-up / test branch** for validating that **ESP32 / ESP32‑S3** can work with:

- **I2C (codec control)**
- **I2S (audio data)**

together with:

- **TI AIC3104 audio codec**
- **ReSpeaker 4‑Mic Array**

### Hardware

- MCU: **Seeed Studio XIAO ESP32 / ESP32‑S3**
- Codec: **TI AIC3104**
- Mic Array: **ReSpeaker 4‑Mic Array**

### Goal

> **Build the minimum viable audio pipeline (MVP Bring-up)**

---

All tests are performed **after board-level pin remapping is verified**.

---

## Board-Level Pin Remapping (CORE)

### Design Principle

- Do NOT create a new board
- Do NOT modify ESP-ADF framework internals
- Directly modify **official board pin mapping**
- Let all upper layers automatically inherit new pins

This is also the **recommended ESP-ADF adaptation method**.

---

### I2C Pin Mapping

**File**
```
board_pins_config.c
```

```c
esp_err_t get_i2c_pins(i2c_port_t port, i2c_config_t *i2c_config)
{
    i2c_config->sda_io_num = GPIO_NUM_5;
    i2c_config->scl_io_num = GPIO_NUM_6;
    return ESP_OK;
}
```

| Signal | GPIO |
|------|------|
| SDA | GPIO5 |
| SCL | GPIO6 |

Used for **AIC3104 register control**.

---

### I2S Pin Mapping

```c
esp_err_t get_i2s_pins(int port, board_i2s_pin_t *i2s_config)
{
    i2s_config->bck_io_num   = GPIO_NUM_8;
    i2s_config->ws_io_num    = GPIO_NUM_7;
    i2s_config->data_out_num = GPIO_NUM_44;
    i2s_config->data_in_num  = GPIO_NUM_43;
    i2s_config->mck_io_num   = -1; // MCLK disabled for stability
    return ESP_OK;
}
```

| Signal | GPIO |
|------|------|
| BCLK | GPIO8 |
| WS / LRCK | GPIO7 |
| DOUT | GPIO44 |
| DIN | GPIO43 |
| MCLK | Disabled |

➡ All **codec / I2S / audio streams automatically use this mapping**

---

## Bring-up Execution Order (IMPORTANT)

The system is brought up strictly in the following order:

```
GPIO → I2C → Codec → I2S → Audio RX
```

Each stage has **independent test code**, making failures easy to locate.

---

## GPIO Output Test (Hardware Sanity Check)

### Test Files

```
main/gpio_test.c
main/gpio_test.h
```

### Tested Pins

| Logical | GPIO |
|-------|------|
| D0 | GPIO1 |
| D2 | GPIO3 |
| D3 | GPIO4 |

### Purpose

- Verify GPIO matrix mapping
- Verify pins are not used by boot / peripherals
- Verify board configuration is effective

### Result

> GPIO test PASS ⇒ Board mapping valid ⇒ Safe to proceed

---

## AIC3104 I2C Bring-up

### Test File

```
main/aic3104_ng.c
```

### What is Verified

- ESP-IDF v5.x **I2C NG API**
- Codec presence
- Page register read/write

### Successful Log

```text
AIC3104_NG: probe ok: page reg=0x00
AIC3104_NG: default setup applied
```

---

## I2S Full-Duplex Test (TX + RX)

### Test File

```
main/i2s_fd_test.c
```

### Test Method

- TX sends square wave
- RX samples simultaneously
- Count non-zero samples

### Successful Log

```text
I2S_FD: Valid samples count=64000
```

> Non-zero count well above threshold indicates RX path is valid

---

## app_main Bring-up Flow

The application entry strictly follows hardware bring-up order:

```c
setup_wifi();

gpio_test_init();
gpio_test_blink_all();

aic3104_ng_init(&aic, I2C_NUM_0, GPIO_NUM_5, GPIO_NUM_6, 100000);
aic3104_ng_probe(&aic, &page);
aic3104_ng_setup_default(&aic);

i2s_full_duplex_init();
i2s_fd_squarewave_test();
```

Each function corresponds to **one physical validation step**.

---

## Conclusion

✅ Board-level GPIO / I2C / I2S remapping is effective  
✅ AIC3104 control path works  
✅ I2S audio data path works  

---

## Summary

This branch provides a:

> **Minimal, reproducible ESP32 + AIC3104 + ReSpeaker audio bring-up template**

Suitable for:

- New hardware adaptation
- ESP-ADF board-level modification reference
- Audio pipeline debugging starting point
