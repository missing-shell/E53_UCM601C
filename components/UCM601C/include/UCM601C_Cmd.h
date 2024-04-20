/**
 * @file
 * @brief ESP32 UCM601C Registers Header File.
 *
 */
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

    // RFID Command Enumerations
    typedef enum RFID_COMMAND
    {
        CMD_SET_CW = 0x3E,                              // 打开 cw 波
        CMD_GET_CW = 0x3F,                              // 获取 cw 波状态
        CMD_GB_SEU_WRITE = 0x42,                        // 复旦微 GB 加密通信写
        CMD_GB_SEU_READ = 0x43,                         // 复旦微 GB 加密通信读
        CMD_GB_READ = 0x45,                             // 读标签 GB
        CMD_GB_WRITE = 0x46,                            // 写标签 GB
        CMD_GB_LOCK = 0x47,                             // 锁定标签 GB
        CMD_GB_KILL = 0x49,                             // 灭活标签 GB
        CMD_READER_PARA_SAVE = 0x4A,                    // 保存参数
        CMD_READER_PARA_RESET = 0x4B,                   // 恢复读写器默认配置
        CMD_READER_APP_UPGRADE = 0x4C,                  // 读写器应用软件升级
        CMD_BASEBAND_FIRMWARE_UPGRADE = 0x4D,           // 基带固件升级
        CMD_SET_KEEPALIVE = 0x4E,                       // 设置心跳包和上报时间
        CMD_GET_KEEPALIVE = 0x4F,                       // 获取心跳包和上报时间
        CMD_GET_SESSION_TARGET = 0x5A,                  // 获取 session 和 target
        CMD_SET_SESSION_TARGET = 0x5B,                  // 设置 session 和 target
        CMD_GET_TX_TIME = 0x5C,                         // 获取读写器 tx_on 时间和 tx_off 时间
        CMD_SET_TX_TIME = 0x5D,                         // 设置读写器 tx_on 时间和 tx_off 时间
        CMD_SET_8_ANT_POWER = 0x5E,                     // 设置的 8 天线读写器射频输出功率
        CMD_SET_4_ANT_POWER = 0x5F,                     // 设置的 4 天线读写器射频输出功率
        CMD_SET_TEMPORARY_OUTPUT_POWER = 0x66,          // 设置读写器临时射频输出功率
        CMD_SET_RF_LINK_PROFILE = 0x69,                 // 设置射频链路的通讯速率
        CMD_GET_RF_LINK_PROFILE = 0x6A,                 // 查询射频链路的通讯速率
        CMD_RESET = 0x70,                               // 复位读写器
        CMD_SET_UART_BAUDRATE = 0x71,                   // 设置串口通讯波特率
        CMD_GET_FIRMWARE_VERSION = 0x72,                // 查询读写器固件版本
        CMD_SET_READER_ADDRESS = 0x73,                  // 设置读写器地址
        CMD_SET_WORK_ANTENNA = 0x74,                    // 设置读写器工作天线
        CMD_GET_WORK_ANTENNA = 0x75,                    // 查询当前工作天线
        CMD_SET_OUTPUT_POWER = 0x76,                    // 设置的单天线读写器射频输出功率
        CMD_GET_OUTPUT_POWER = 0x77,                    // 查询读写器当前输出功率
        CMD_SET_FREQUENCY_REGION = 0x78,                // 设置读写器工作频率范围
        CMD_GET_FREQUENCY_REGION = 0x79,                // 查询读写器工作频率范围
        CMD_GET_READER_TEMPERATURE = 0x7B,              // 查询当前设备的工作温度
        CMD_INVENTORY = 0x80,                           // 盘存标签
        CMD_READ = 0x81,                                // 读标签
        CMD_WRITE = 0x82,                               // 写标签
        CMD_LOCK = 0x83,                                // 锁定标签
        CMD_KILL = 0x84,                                // 灭活标签
        CMD_SET_ACCESS_EPC_MATCH = 0x85,                // 匹配 ACCESS 操作的 EPC 号
        CMD_GET_ACCESS_EPC_MATCH = 0x86,                // 查询匹配的 EPC 状态
        CMD_FAST_SWITCH_ANT_INVENTORY = 0x87,           // 多天线轮询盘存
        CMD_REAL_TIME_INVENTORY = 0x89,                 // 盘存标签(实时上传标签数据)
        CMD_CUSTOM_INVENTORY = 0x8A,                    // 盘存标签(自定义读取数据)
        CMD_CUSTOMIZED_SESSION_TARGET_INVENTORY = 0x8B, // 自定义 session 和 target 盘存
        CMD_STOP_INVENTORY = 0x8C,                      // 停止盘存
        CMD_SET_SELECT = 0x8D,                          // 设置 select 参数
        CMD_GET_SELECT = 0x8E,                          // 获取 select 参数
        CMD_GET_INVENTORY_BUFFER = 0x90,                // 查询盘存缓存
        CMD_GET_AND_RESET_INVENTORY_BUFFER = 0x91,      // 查询并重置盘存缓存
        CMD_GET_INVENTORY_BUFFER_TAG_COUNT = 0x92,      // 查询盘存缓存标签数量
        CMD_RESET_INVENTORY_BUFFER = 0x93,              // 重置盘存缓存
        CMD_SM7_WRITE = 0x95,                           // SM7 加密写
        CMD_SM7_READ = 0x96,                            // SM7 解密读
        CMD_SM7_PK_UPDATE = 0x97,                       // SM7 PK 秘钥更新
        CMD_GB_MUL_SEU_AUTH = 0x98,                     // 复旦微 GB 双向认证
        CMD_CHECK_ANT = 0xE0,                           // 获取天线连接状态
        CMD_READER_OVERTEMPERATURE_ALARM = 0xE1         // 读写器温度过高告警
    } RFID_COMMAND;

#ifdef __cplusplus
}

#endif