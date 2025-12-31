# ESP-ADF 依赖说明

⚠️ **重要说明**：本项目 **不会** 将 ESP-ADF 源码直接提交到仓库中。

## 依赖概述

本项目依赖 **Espressif Audio Development Framework（ESP-ADF）**，用于：

- 音频 Codec 驱动
- I2S 接口抽象
- Audio Board 支持

由于 ESP-ADF 目录层级 **非常深**，在 Windows 环境下直接提交源码极易导致：

- `Filename too long` 错误
- Git 索引失败
- 跨平台构建问题

因此本项目采用 **外部依赖** 方式管理 ESP-ADF，而不是 submodule 或 vendor 源码。

---

## 版本要求

- **ESP-IDF**：v5.3.2  
- **ESP-ADF**：v2.7

⚠️ 版本不匹配可能导致编译失败或驱动冲突。

---

## 目录结构要求

ESP-ADF 需要放置在：

```text
components/third_party/esp-adf
```

期望目录结构：

```text
components
└── third_party
    └── esp-adf
        ├── components
        ├── esp-idf
        ├── examples
        └── tools
```

---

## 获取 ESP-ADF

在工程根目录执行：

```bash
git clone https://github.com/espressif/esp-adf.git components/third_party/esp-adf
cd components/third_party/esp-adf
git checkout v2.7
```

---

## 编译前检查清单

在执行 `idf.py build` 前，请确认：

- [ ] `components/third_party/esp-adf` 存在  
- [ ] ESP-ADF 版本为 `v2.7`  
- [ ] ESP-IDF 版本为 `v5.3.2`  
- [ ] `IDF_PATH` 已正确设置  

---

## 为什么不使用 Git Submodule

本仓库**刻意避免**使用 submodule 管理 ESP-ADF，原因包括：

- Windows 路径长度限制
- 仓库体积巨大，维护成本高
- 外部依赖文档化更清晰、可控

该做法符合 Espressif 官方示例工程的常见实践。

---
