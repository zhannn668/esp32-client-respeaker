# ESP32 ReSpeaker Audio Capture (Test Branch)

*English | [简体中文](README.md)*

## Overview

This is a **small test branch** for validating audio capture using **ESP32 / ESP32-S3** with a **ReSpeaker 4-Mic Array** over the **I2S interface**.

- MCU: **Seeed Studio XIAO (ESP32 series)**
- Microphone: **ReSpeaker 4-Mic Array**
- Project type: Experimental / Reference implementation

This branch is intended for testing and learning purposes only.

---

## Test Purpose

- Verify I2S audio capture on ESP32 with ReSpeaker 4-Mic Array
- Understand the ESP-IDF I2S audio workflow
- Provide a minimal reference for future audio-related projects

---

## Key Modifications

The main changes in this test branch focus on:

```
main/audio_proc.c
```

Including:

- I2S initialization parameters
- I2S GPIO mapping (BCLK / WS / DIN)
- Basic PCM audio capture validation

This file can be used as:
- A reference for ESP32 + ReSpeaker 4-Mic Array
- A starting point for further audio development

---

## Hardware Setup

- MCU: Seeed Studio XIAO (ESP32 / ESP32-S3)
- Microphone Array: ReSpeaker 4-Mic Array (I2S)
- Power and wiring depend on the specific test setup

---

## I2S Signals

| Signal | Description |
|----|----|
| BCLK | Bit Clock |
| WS / LRCK | Word Select |
| DIN | Audio Data Input |
| MCLK | Master Clock (required by ReSpeaker 4-Mic) |

I2S pins are configured via the ESP32 **GPIO Matrix** in software.

---

## Software Environment

- ESP-IDF v4.x / v5.x
- Toolchain: xtensa-esp32 / xtensa-esp32s3

---

## Build & Flash (Example)

```bash
idf.py set-target esp32s3
idf.py build
idf.py flash monitor
```

---

## Notes

- This is an experimental test branch
- GPIO assignments must be adapted to different XIAO boards
- Ensure proper MCLK configuration for ReSpeaker 4-Mic Array

---

## Disclaimer

This repository branch is provided for testing and reference purposes only.
