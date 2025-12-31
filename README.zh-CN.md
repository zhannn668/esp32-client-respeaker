# ESP32 ReSpeaker Audio Capture (Test Branch)

*简体中文 | [English](README.en.md)*

---

## 项目简介

这是一个**小型测试分支**，用于验证 **ESP32 / ESP32-S3** 平台通过 **I2C + I2S 接口**与 **TI AIC3104 音频 Codec（ReSpeaker 4-Mic Array）** 协同工作的可行性。

- 实际主控：**Seeed Studio XIAO ESP32 / ESP32-S3**
- 音频 Codec：**TI AIC3104**
- 麦克风阵列：**ReSpeaker 4-Mic Array**
- 项目性质：**Bring-up / 功能验证 / 参考实现**

该分支专注于**底层硬件链路打通**，不包含完整业务逻辑。

---

## Bring-up 总体流程

本工程遵循标准嵌入式音频系统 Bring-up 顺序：

```
GPIO → I2C → Codec → I2S → Audio RX/TX
```

每一步均有独立测试代码，确保问题可快速定位。

---

## GPIO 输出测试（基础硬件自检）

### 测试目的

在进行 I2C / I2S 通信前，首先验证：

- GPIO 复用是否正确
- GPIO Matrix 映射是否生效
- 引脚未被 Boot / 外设占用

### 测试文件

```
main/gpio_test.c
main/gpio_test.h
```

### 测试引脚（XIAO ESP32-S3）

| 逻辑引脚 | GPIO |
|--------|------|
| D0 | GPIO1 |
| D2 | GPIO3 |
| D3 | GPIO4 |

### 测试方式

- 三路 GPIO 配置为输出
- 以“走马灯”方式轮流输出高电平
- 通过万用表 / LED / 逻辑分析仪观察电平变化

### 预期日志

```text
GPIO_TEST: GPIO test init done
GPIO_TEST: D0=1 D2=0 D3=0
GPIO_TEST: D0=0 D2=1 D3=0
GPIO_TEST: D0=0 D2=0 D3=1
```

### 结论

当三路 GPIO 能稳定输出 0V / 3.3V：

> ✅ GPIO 配置与 Board 映射生效  
> ✅ 可进入 I2C / I2S Bring-up  

---

## AIC3104 I2C 初始化（NG 驱动）

### 测试文件

```
main/aic3104_ng.c
```

### 测试目标

- 验证 ESP-IDF v5.x **I2C NG API**
- 验证 AIC3104 芯片存在
- 验证寄存器可正常读写

### 关键日志

```text
AIC3104_NG: init done
AIC3104_NG: probe ok: page reg=0x00
AIC3104_NG: default setup applied
```

---

## I2S 全双工测试（TX + RX）

### 测试文件

```
main/i2s_fd_test.c
```

### 测试目标

- 验证 I2S 时钟（BCLK / WS / MCLK）
- 验证数据线（DIN / DOUT）
- 验证 Codec ↔ ESP32 音频通路

### 测试方式

- TX 端输出方波信号
- RX 端同步采样
- 统计非零样本数量

### 关键日志

```text
I2S_FD: I2S FD init done (16k, stereo, 32bit)
I2S_FD: Valid samples count=64000 (threshold~16000)
```

> 非零样本显著大于阈值即表示 RX 正常

---

## Board 配置说明

### 关键文件

```
components/third_party/esp-adf/components/audio_board/
└── esp32_s3_korvo2_v3/board_pins_config.c
```

### 已验证 GPIO 映射

| 功能 | GPIO |
|----|----|
| I2C SDA | GPIO5 |
| I2C SCL | GPIO6 |
| I2S BCLK | GPIO8 |
| I2S WS | GPIO7 |
| I2S DOUT | GPIO44 |
| I2S DIN | GPIO43 |

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

- 本分支为 Bring-up / 测试用途
- 不同 XIAO 型号 GPIO 资源不同
- AIC3104 依赖 MCLK，需确认硬件连接

---

## 说明

本仓库分支仅用于实验和学习参考，  
可作为 **ESP32 + AIC3104 / ReSpeaker 音频系统 Bring-up 的最小可行示例**。
