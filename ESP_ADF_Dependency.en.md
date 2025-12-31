# ESP-ADF Dependency Notice

⚠️ **Important**: This project does **NOT** vendor ESP-ADF source code.

## Dependency Overview

This project depends on the **Espressif Audio Development Framework (ESP-ADF)** for:

- Audio codec drivers
- I2S interface abstraction
- Audio board support

Due to ESP-ADF's **deep directory hierarchy**, committing it directly—especially on Windows—often causes:

- `Filename too long` errors
- Git index failures
- Cross-platform build issues

Therefore, ESP-ADF is managed strictly as an **external dependency**.

---

## Required Versions

- **ESP-IDF**: v5.3.2  
- **ESP-ADF**: v2.7

⚠️ Version mismatch may lead to build or driver conflicts.

---

## Required Directory Layout

ESP-ADF must be cloned into:

```text
components/third_party/esp-adf
```

Expected structure:

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

## How to Fetch ESP-ADF

From the project root directory:

```bash
git clone https://github.com/espressif/esp-adf.git components/third_party/esp-adf
cd components/third_party/esp-adf
git checkout v2.7
```

---

## Pre-build Checklist

Before building, ensure:

- [ ] `components/third_party/esp-adf` exists  
- [ ] ESP-ADF version is `v2.7`  
- [ ] ESP-IDF version is `v5.3.2`  
- [ ] `IDF_PATH` is configured  

---

## Why ESP-ADF Is Not a Submodule

This repository intentionally avoids git submodules for ESP-ADF due to:

- Windows path length limitations
- Repository size and maintenance overhead
- Better reproducibility via documented dependencies

This mirrors common practice in Espressif official examples.

---
