#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "UCM601C.h"
#include "UCM601C_Cmd.h"

#define UCM601C_UART_PORT_NUM (1)
#define UCM601C_BAND (115200)
#define UCM601C_TX (5)
#define UCM601C1_RX (4)
#define UCM601C_UART_BUF_SIZE (2048)
/**
 *
 *串口驱动配置
 *串口读写任务函数
 *写数据如何传递--传入指针
 *读数据如何传递--消息队列--依次解析相关数据
 *何时写--调用uart_write_bytes() <-- diver_write <-- wirteCommand() <-- 具体命令 <-- 应用层 (发送命令)
 *何时读--调用uart_read_bytes(调用方式：？中断 ？事件循环)                (读取命令)
 *@param
 */
static void ucm601c_uart_init(void) // Define the echo task function
{
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        // Define and initialize the UART configuration
        .baud_rate = UCM601C_BAND,       // Set the baud rate
        .data_bits = UART_DATA_8_BITS,   // Set the data bits
        .parity = UART_PARITY_DISABLE,   // Disable parity
        .stop_bits = UART_STOP_BITS_1,   // Set the stop bits
        .source_clk = UART_SCLK_DEFAULT, // Set the source clock
    };
    int intr_alloc_flags = 0; // Define the interrupt allocation flags

    ESP_ERROR_CHECK(uart_driver_install(UCM601C_UART_PORT_NUM, UCM601C_UART_BUF_SIZE * 2, UCM601C_UART_BUF_SIZE * 2, 0, NULL, intr_alloc_flags)); // Install the UART driver
    ESP_ERROR_CHECK(uart_param_config(UCM601C_UART_PORT_NUM, &uart_config));                                                                      // Configure the UART parameters
    ESP_ERROR_CHECK(uart_set_pin(UCM601C_UART_PORT_NUM, UCM601C_TX, UCM601C1_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));                        // Set the UART pins
}
// TODO:错误判断，是否写入成功
void diver_write(uint8_t *data, uint8_t len)
{
    uart_write_bytes(UCM601C_UART_PORT_NUM, data, len);
}