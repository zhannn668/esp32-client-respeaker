# ESP32 ReSpeaker Audio Capture (Test Branch)

*简体中文 | [English](README.en.md)*

## 项目简介

这是一个**小型测试分支**，用于验证 **ESP32 / ESP32-S3** 平台通过 **I2S 接口**采集 **ReSpeaker 4-Mic Array** 音频数据的可行性。

- 实际主控：**Seeed Studio XIAO（ESP32 系列）**
- 麦克风阵列：**ReSpeaker 4-Mic Array**
- 项目性质：功能验证 / 参考实现 / 非完整产品

该分支主要用于实验和参考，不作为完整解决方案。

---

## 测试目标

- 验证 ESP32 I2S 接口与 ReSpeaker 4-Mic Array 的兼容性  
- 熟悉 ESP-IDF 下 I2S 音频采集流程  
- 为后续音频处理或通信模块提供基础参考  

---

## 关键修改说明

本测试分支主要作为参考实现，**核心修改文件为**：

```
main/audio_proc.c
```

修改内容主要集中在：

- I2S 初始化参数配置
- I2S GPIO（BCLK / WS / DIN / MCLK）映射
- 基本 PCM 音频数据采集验证

该文件可作为：
- ESP32 + ReSpeaker 4-Mic Array 的 I2S 参考实现
- 后续音频项目的基础模板

---

## 硬件环境

- MCU：Seeed Studio XIAO（ESP32 / ESP32-S3）
- 麦克风：ReSpeaker 4-Mic Array（I2S）
- 实际供电与引脚连接以测试环境为准

---

## I2S 信号说明

| 信号 | 说明 |
|----|----|
| BCLK | 位时钟 |
| WS / LRCK | 声道选择 |
| DIN | 音频数据输入 |
| MCLK | 主时钟（ReSpeaker 4-Mic 需要） |

I2S 引脚通过 ESP32 的 **GPIO Matrix** 在软件中配置。

---

## 软件环境

- ESP-IDF v5.3.2
- 编译工具链：xtensa-esp32 / xtensa-esp32s3

---

## 编译与运行（示例）

```bash
idf.py set-target esp32s3
idf.py build
idf.py flash monitor
```

---

## 注意事项

- 本分支为测试用途，代码结构可能随时调整
- 不同 XIAO 型号 GPIO 资源不同，请根据实际硬件修改 I2S 引脚
- 请确保 ReSpeaker 4-Mic Array 的 MCLK 配置正确

---

## 说明

本仓库分支仅用于实验和学习参考。
