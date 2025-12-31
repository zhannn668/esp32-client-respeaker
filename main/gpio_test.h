#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化 GPIO 输出测试（D0 / D2 / D3）
 */
void gpio_test_init(void);

/**
 * @brief GPIO 全部同时闪烁测试
 */
void gpio_test_blink_all(void);

/**
 * @brief GPIO 走马灯测试（D0 -> D2 -> D3）
 */
void gpio_test_chase(void);

#ifdef __cplusplus
}
#endif
