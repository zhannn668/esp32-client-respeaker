# ESP32 ReSpeaker Audio Capture (Test Branch)

*English | [简体中文](README.md)*

---

## Project Overview

This repository contains a **small bring-up / test branch** used to validate audio functionality on **ESP32 / ESP32-S3** platforms using **I2C + I2S**, together with the **TI AIC3104 audio codec** found on the **ReSpeaker 4-Mic Array**.

- MCU: **Seeed Studio XIAO ESP32 / ESP32-S3**
- Audio Codec: **TI AIC3104**
- Microphone Array: **ReSpeaker 4-Mic Array**
- Project Type: **Bring-up / Hardware Validation / Reference Implementation**

This branch focuses on **low-level hardware bring-up** rather than full application logic.

---

## Bring-up Flow

The project follows a standard embedded audio bring-up sequence:

```
GPIO → I2C → Codec → I2S → Audio RX/TX
```

Each stage has an **independent test module** to simplify debugging and fault isolation.

---

## GPIO Output Test (Basic Hardware Sanity Check)

### Purpose

Before enabling I2C or I2S, GPIO output is verified to ensure:

- Correct GPIO mapping
- Proper GPIO matrix routing
- Pins are not occupied by boot or peripheral functions

### Test Files

```
main/gpio_test.c
main/gpio_test.h
```

### Tested Pins (XIAO ESP32-S3)

| Logical Pin | GPIO |
|------------|------|
| D0 | GPIO1 |
| D2 | GPIO3 |
| D3 | GPIO4 |

### Test Method

- Configure all three GPIOs as outputs
- Toggle pins in a “chasing LED” pattern
- Observe behavior using a multimeter, LED, or logic analyzer

### Expected Log Output

```text
GPIO_TEST: GPIO test init done
GPIO_TEST: D0=1 D2=0 D3=0
GPIO_TEST: D0=0 D2=1 D3=0
GPIO_TEST: D0=0 D2=0 D3=1
```

### Result

If all three pins reliably toggle between 0V and 3.3V:

> ✅ GPIO configuration is correct  
> ✅ Board pin mapping is effective  
> ✅ Safe to proceed with I2C / I2S bring-up  

---

## AIC3104 I2C Initialization (NG Driver)

### Test File

```
main/aic3104_ng.c
```

### Purpose

- Validate ESP-IDF v5.x **I2C NG driver**
- Detect and probe the AIC3104 codec
- Verify basic register read/write access
- Apply default codec configuration

### Key Log Messages

```text
AIC3104_NG: init done
AIC3104_NG: probe ok: page reg=0x00
AIC3104_NG: default setup applied
```

Successful logs indicate stable I2C communication with the codec.

---

## I2S Full-Duplex Test (TX + RX)

### Test File

```
main/i2s_fd_test.c
```

### Purpose

- Verify I2S clocks (BCLK / WS / MCLK)
- Verify data lines (DIN / DOUT)
- Confirm end-to-end audio path between ESP32 and codec

### Test Method

- Generate a square-wave signal on I2S TX
- Simultaneously read data on I2S RX
- Count non-zero received samples to validate data integrity

### Key Log Messages

```text
I2S_FD: I2S FD init done (16k, stereo, 32bit)
I2S_FD: Valid samples count=64000 (threshold~16000)
```

A high number of valid samples confirms a working RX path.

---

## Board Configuration

### Key File

```
components/third_party/esp-adf/components/audio_board/
└── esp32_s3_korvo2_v3/board_pins_config.c
```

### Verified GPIO Mapping

| Function | GPIO |
|--------|------|
| I2C SDA | GPIO5 |
| I2C SCL | GPIO6 |
| I2S BCLK | GPIO8 |
| I2S WS / LRCK | GPIO7 |
| I2S DOUT | GPIO44 |
| I2S DIN | GPIO43 |

---

## Hardware Setup

- MCU: Seeed Studio XIAO ESP32 / ESP32-S3
- Codec: TI AIC3104
- Microphone Array: ReSpeaker 4-Mic Array

---

## Software Environment

- ESP-IDF v5.3.2
- ESP-ADF v2.7
- Toolchain: xtensa-esp32 / xtensa-esp32s3

---

## Build & Run

```bash
idf.py set-target esp32s3
idf.py build
idf.py flash monitor
```

---

## Notes

- This branch is intended for bring-up and testing only
- GPIO availability varies between XIAO models
- AIC3104 requires a valid MCLK signal to operate correctly

---

## Disclaimer

This repository is intended for **experimental and educational purposes** and serves as a **minimal, reusable reference** for ESP32 + AIC3104 audio bring-up.
