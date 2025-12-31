# ESP32 ReSpeaker Audio Capture (Test Branch)

*简体中文 | [English](README.en.md)*

## 项目简介

这是一个**小型测试分支**，用于验证 **ESP32 / ESP32-S3** 平台通过 **I2S 接口**与 **TI AIC3104 音频 Codec（ReSpeaker 4-Mic Array）** 协同工作的可行性。

- 实际主控：**Seeed Studio XIAO ESP32 / ESP32-S3**
- 音频 Codec：**TI AIC3104**
- 麦克风阵列：**ReSpeaker 4-Mic Array**
- 项目性质：**功能验证 / 参考实现 / 非完整产品**

该分支重点关注 **底层音频链路打通（I2C + I2S）+ GPIO测试**，不包含完整业务逻辑。

---

## 测试目标

- ✅ 验证 **ESP32 I2C NG 驱动** 与 AIC3104 的稳定通信  
- ✅ 完成 **AIC3104 Codec 初始化与基础音频路径配置**  
- ✅ 验证 **I2S 全双工（TX/RX）数据通路** 是否正常  
- ✅ 确认 Board 配置文件中的 **GPIO 映射真实生效**

---

## 核心文件与功能说明

### 1️⃣ `main/aic3104_ng.c`

**作用：AIC3104 Codec 的 I2C（NG 驱动）初始化与寄存器配置**

该文件完成了 **音频 Codec 侧“上电 + 可通信 + 可工作”** 的全部基础动作，是音频系统的**起点**。

主要功能包括：

- 使用 **ESP-IDF v5.x 新 I2C NG API**（`i2c_master_bus_*`）
- 绑定 I2C 引脚（SDA / SCL）
- 对 AIC3104 进行：
  - Page 选择与探测（probe）
  - 寄存器可读写验证
  - 默认音频路径配置（DAC / HPLOUT / LOP）

**验证方式（串口日志）：**
```text
AIC3104_NG: init done
AIC3104_NG: probe ok: page reg=0x00
AIC3104_NG: default setup applied
```

---

### 2️⃣ `main/i2s_fd_test.c`

**作用：I2S 全双工（TX + RX）链路功能验证**

该文件是一个**纯 I2S 层测试模块**，用于确认 **ESP32 ⇄ AIC3104** 音频数据链路是否真实可用。

主要功能包括：

- 初始化 **I2S Full-Duplex 模式**
  - 采样率：16 kHz
  - 声道：Stereo
  - 位宽：32-bit
- 通过 I2S **TX 端**输出方波测试信号
- 通过 I2S **RX 端**同步采集数据
- 对采集数据进行非零样本统计与阈值判断

**验证方式（串口日志）：**
```text
I2S_FD: I2S FD init done (16k, stereo, 32bit)
I2S_FD: Valid samples count=64000 (threshold~16000)
```

---

## Board 配置与 GPIO 映射

### 关键 Board 文件

```
components/third_party/esp-adf/components/audio_board/
└── esp32_s3_korvo2_v3/board_pins_config.c
```

### 已验证生效的 GPIO 映射

| 功能 | GPIO |
|----|----|
| I2C SDA | GPIO5 |
| I2C SCL | GPIO6 |
| I2S BCLK | GPIO8 |
| I2S WS / LRCK | GPIO7 |
| I2S DOUT | GPIO44 |
| I2S DIN | GPIO43 |
| I2S MCLK | Board 内部配置 |

---

## 硬件环境

- MCU：Seeed Studio XIAO ESP32 / ESP32-S3
- Codec：TI AIC3104
- 麦克风阵列：ReSpeaker 4-Mic Array

---

## 软件环境

- ESP-IDF v5.3.2
- ESP-ADF v2.7
- xtensa-esp32 / xtensa-esp32s3 toolchain

---

## 编译与运行

```bash
idf.py set-target esp32s3
idf.py build
idf.py flash monitor
```

---

## 注意事项

- 本分支为测试用途
- GPIO 映射需与实际硬件一致
- AIC3104 依赖 MCLK，需确认硬件连接

---

## 说明

本仓库分支仅用于实验和学习参考。
