/**
 * @file RFID_Transplant.h
 *
 * @brief This header file provides definitions and type aliases
 *        for the RFID transplant module, which utilizes FreeRTOS,
 *        UART driver, and error handling in the ESP environment.
 */

#ifndef _RFID_TRANSPLANT_H_
#define _RFID_TRANSPLANT_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "esp_log.h"

/**
 * @file RFID_Native_Logging.h
 *
 * @brief This header file defines custom logging macros for the RFID Native module,
 *        using the ESP-IDF logging infrastructure.
 */
#define TAG "RFID Native"
#define RFID_LOGD(format, ...) ESP_LOGD(TAG, format, ##__VA_ARGS__)
#define RFID_LOGI(format, ...) ESP_LOGI(TAG, format, ##__VA_ARGS__)
#define RFID_LOGW(format, ...) ESP_LOGW(TAG, format, ##__VA_ARGS__)
#define RFID_LOGE(format, ...) ESP_LOGE(TAG, format, ##__VA_ARGS__)
#define RFID_LOGF(format, ...) ESP_LOGE(TAG, format, ##__VA_ARGS__)

/**
 * @brief Alias for esp_err_t, representing an error code specific to the RFID transplant module.
 */
typedef esp_err_t rfid_err_t;

#ifdef __cplusplus
extern "C"
{
#endif

    // 假设此处有需要与 C 兼容的函数声明或全局变量定义
    // 例如：
    // void some_c_compatible_function(int arg);
    // extern int some_global_variable;

#ifdef __cplusplus
}
#endif

#endif