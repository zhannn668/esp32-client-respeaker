
# ESP32 ReSpeaker Audio Capture (Test Branch)

*简体中文 | [English](README.en.md)*

---

## 关键修改文件（最重要，一定先看）

> **本项目的所有引脚适配，完全不是在应用层“手写 GPIO/I2C/I2S 初始化”，而是通过修改 ESP‑ADF 官方 Board 配置实现的。**

```
components/third_party/esp-adf/components/audio_board/
└── esp32_s3_korvo2_v3/
    └── board_pins_config.c   ← ★ 本项目最核心、唯一的硬件适配入口
```

**结论先给出：**

- 本工程 **仍然使用 ESP‑ADF 官方的 `esp32_s3_korvo2_v3` Board**
- 没有新建 Board
- 没有魔改 ESP‑ADF 框架
- 只是 **重写了该 Board 的引脚映射**
- 所有上层模块（`audio_board / codec / i2s_stream`）**自动使用新引脚**

如果你只想知道：

> 👉「ESP32‑S3 + XIAO + ReSpeaker 4‑Mic 到底怎么接、怎么改？」

**只需要看这一份 `board_pins_config.c`。**

---

## 项目简介

这是一个 **Bring‑up / 测试分支**，目标非常单一：

> **验证 ESP32 / ESP32‑S3 能否稳定驱动 TI AIC3104，并通过 I2C + I2S 打通 ReSpeaker 4‑Mic Array 的完整音频链路。**

### 硬件组成

- 主控：**Seeed Studio XIAO ESP32 / ESP32‑S3**
- Codec：**TI AIC3104**
- 麦克风阵列：**ReSpeaker 4‑Mic Array**
- 接口：**I2C + I2S**

### 本分支不做什么

- ❌ 不做语音识别
- ❌ 不做音频算法
- ❌ 不做网络业务

本分支只做一件事：

> **把底层硬件链路“一次性打通”。**

---

## Bring‑up 总体思路（非常重要）

整个工程严格遵循 **硬件 Bring‑up 的黄金顺序**：

```
GPIO
  ↓
I2C（Codec 控制）
  ↓
Codec 寄存器可读写
  ↓
I2S 时钟（BCLK / WS / MCLK）
  ↓
I2S 数据（RX / TX）
```

⚠️ **顺序不能乱**  
⚠️ **每一步都必须有独立测试**

后面的所有代码和文档，都是围绕这个顺序展开的。

---

## Board 层引脚重配置（核心原理）

### 为什么一定要在 Board 层改？

ESP‑ADF 的设计理念是：

> **Board 层负责“这块板子怎么连线”，应用层只关心“我在用音频”。**

如果你在应用层：

- 自己 init I2C
- 自己 init I2S
- 自己指定 GPIO

那你基本等于 **绕过了 ESP‑ADF 的整套抽象**，后期会非常痛苦。

**正确做法：**  
👉 修改 `board_pins_config.c`

---

### I2C 引脚配置（Codec 控制通道）

**文件：**

```
components/third_party/esp-adf/components/audio_board/
└── esp32_s3_korvo2_v3/board_pins_config.c
```

```c
esp_err_t get_i2c_pins(i2c_port_t port, i2c_config_t *i2c_config)
{
    i2c_config->sda_io_num = GPIO_NUM_5;
    i2c_config->scl_io_num = GPIO_NUM_6;
    return ESP_OK;
}
```

| 信号 | GPIO |
|----|----|
| SDA | GPIO5 |
| SCL | GPIO6 |

📌 **作用说明**

- 所有 Codec（AIC3104）的寄存器访问
- 所有 `audio_board_init()` 内部 I2C 操作
- 都会自动走这两个引脚

---

### I2S 引脚配置（音频数据通道）

```c
esp_err_t get_i2s_pins(int port, board_i2s_pin_t *i2s_config)
{
    i2s_config->bck_io_num   = GPIO_NUM_8;
    i2s_config->ws_io_num    = GPIO_NUM_7;
    i2s_config->data_out_num = GPIO_NUM_44;
    i2s_config->data_in_num  = GPIO_NUM_43;
    i2s_config->mck_io_num   = -1; // 先禁用 MCLK，优先保证稳定
    return ESP_OK;
}
```

| 信号 | GPIO |
|----|----|
| BCLK | GPIO8 |
| WS / LRCK | GPIO7 |
| DOUT | GPIO44 |
| DIN | GPIO43 |
| MCLK | 禁用 |

📌 **重要结论**

> 一旦这里修改完成，ESP‑ADF 内所有 I2S 相关模块都会自动吃到这组引脚。

---

## GPIO 输出测试（最小硬件自检）

### 为什么一定要先测 GPIO？

因为这是：

- 成本最低
- 信息最直接
- 能最快排除 **“板子根本没按你想的那样连”** 的问题

### 测试文件

```
main/gpio_test.c
main/gpio_test.h
```

### 测试引脚（XIAO ESP32‑S3）

| 逻辑名 | GPIO |
|----|----|
| D0 | GPIO1 |
| D2 | GPIO3 |
| D3 | GPIO4 |

### 测试方式

- 三个 GPIO 配置为输出
- 轮流拉高（走马灯）
- 用万用表 / LED / 示波器观察

### 结论

> GPIO 正常 ⇒ Board 层映射生效 ⇒ 才有资格继续 I2C / I2S

---

## app_main Bring‑up 执行流程（强烈建议仔细看）

下面这段代码 **不是业务逻辑**，而是：

> **一条“硬件 Bring‑up 执行脚本”**

### app_main 中的真实调用顺序

```c
// 1. NVS 初始化（WiFi / ADF 必需）
nvs_flash_init();

// 2. 启动 WiFi（本项目依赖网络）
setup_wifi();

// 3. GPIO 最小硬件验证
gpio_test_init();
gpio_test_blink_all();

// 4. AIC3104 I2C Bring-up
aic3104_ng_t aic = {0};
aic3104_ng_init(&aic, I2C_NUM_0, GPIO_NUM_5, GPIO_NUM_6, 100000);

uint8_t page = 0xFF;
aic3104_ng_probe(&aic, &page);
aic3104_ng_setup_default(&aic);
```

---

### 每一步在“硬件层面”到底验证了什么？

#### ① GPIO Test

- GPIO Matrix 是否生效
- Board 层引脚是否真的被用上

❌ 失败 ⇒ **别看 I2C / I2S，先检查接线**

---

#### ② I2C Init

- I2C 控制器工作
- SDA / SCL 真正连到 Codec

❌ 失败 ⇒ **引脚 / 上拉 / 电源问题**

---

#### ③ Codec Probe

- AIC3104 是否存在
- 寄存器是否可读写

❌ 失败 ⇒ **Codec 没供电 / 地址错误 / I2C 时序问题**

---

#### ④ Codec Default Setup

- 时钟树
- ADC / DAC 通道
- 数字音频路径

⚠️ **这一步成功 ≠ 有声音**
👉 只是说明 Codec 进入“可用状态”

---

## I2S 全双工验证

### 测试文件

```
main/i2s_fd_test.c
```

### 测试方式

- TX：输出方波
- RX：同步采样
- 统计非零样本数量

### 成功日志

```text
I2S_FD: Valid samples count=64000 (threshold≈16000)
```

📌 **判断标准**

- 非零样本 ≫ 阈值
- RX 通道数据稳定

---

## 最终结论

✅ Board 层引脚修改 **100% 生效**  
✅ I2C → Codec → I2S 链路完整打通  
✅ AIC3104 + ReSpeaker 可被 ESP32‑S3 稳定驱动  

---

## 项目定位总结

> **这是一个“可以被照抄的 ESP32 音频 Bring‑up 模板”。**

非常适合用于：

- 新板子音频硬件调试
- ESP‑ADF Board 层适配学习
- AIC3104 / I2S 系统最小验证

