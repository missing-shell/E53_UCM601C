/**
 * @file rfid_codec.h
 * @author ucchip
 * @brief  rfid编解码命令码及函数接口
 * @version 0.1
 * @date 2022-06-15
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef RFID_CODEC_H
#define RFID_CODEC_H

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

// #ifdef RFID_CODEC_EXPORTS
//   #define RFID_CODEC_API __declspec(dllexport)
// #else
//   #define RFID_CODEC_API __declspec(dllimport)
// #endif

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

/**
 * @brief rfid编解码命令码
 *
 */
typedef enum
{
    CMD_RAW_DATA,                    ///< 未定义数据流
    CMD_RESET = 0x70,                ///< 复位读写器
    CMD_SET_UART_BAUDRATE = 0x71,    ///< 设置串口通讯波特率
    CMD_GET_FIRMWARE_VERSION = 0x72, ///< 查询读写器固件版本
    CMD_SET_READER_ADDRESS = 0x73,   ///< 设置读写器地址
    CMD_SET_WORK_ANTENNA = 0x74,     ///< 设置读写器工作天线
    CMD_GET_WORK_ANTENNA = 0x75,     ///< 查询当前工作天线
    CMD_SET_OUTPUT_POWER = 0x76,     ///< 设置读写器射频输出功率
    CMD_GET_OUTPUT_POWER = 0x77,     ///< 查询读写器当前输出功率
    CMD_SET_FREQ_REGION = 0x78,      ///< 设置读写器工作频率范围
    CMD_GET_FREQ_REGION = 0x79,      ///< 查询读写器工作频率范围
    CMD_SET_BEEP_MODE = 0x7A,
    CMD_GET_READER_TEMP = 0x7B, ///< 查询当前设备的工作温度
    CMD_READ_GPIO_VALUE = 0x60,
    CMD_WRITE_GPIO_VALUE = 0x61,
    CMD_SET_ANT_CONECT_DETEC = 0x62,
    CMD_GET_ANT_CONECT_DETEC = 0x63,
    CMD_SET_TEMPORARY_OUT_POWER = 0x66, ///< 设置读写器临时射频输出功率
    CMD_SET_READER_IDENTIFIER = 0x67,
    CMD_GET_READER_IDENTIFIER = 0x68,
    CMD_SET_RF_LINK_PROFILE = 0x69, ///< 设置射频链路的通讯速率
    CMD_GET_RF_LINK_PROFILE = 0x6A, ///< 查询射频链路的通讯速率
    CMD_GET_RF_PORT_RETURN_LOSS = 0x7E,
    CMD_GET_FAST_EPC = 0x50,
    CMD_SET_FAST_EPC = 0x51,

    CMD_INVENTORY = 0x80,            ///< 盘存标签
    CMD_READ = 0x81,                 ///< 读标签
    CMD_WRITE = 0x82,                ///< 写标签
    CMD_LOCK = 0x83,                 ///< 锁定标签
    CMD_KILL = 0x84,                 ///< 灭活标签
    CMD_SET_ACCESS_EPC_MATCH = 0x85, ///< 匹配ACCESS操作的EPC号
    CMD_GET_ACCESS_EPC_MATCH = 0x86, ///< 查询匹配的EPC状态
    CMD_REAL_TIME_INVENTORY = 0x89,  ///< 盘存标签(实时上传标签数据)
    CMD_FAST_SWITCH_ANT_INVENTORY = 0x8A,
    CMD_CUS_SESSION_TARGET_INV = 0x8B, ///< 自定义session和target盘存
    CMD_STOP_INVENTORY = 0x8C,
    CMD_SELECT = 0x98, ///< 标签过滤

    CMD_ISO18000_6B_INVENTORY = 0xB0,
    CMD_ISO18000_6B_READ = 0xB1,
    CMD_ISO18000_6B_WRITE = 0xB2,
    CMD_ISO_18000_6B_LOCK = 0xB3,
    CMD_ISO_18000_6B_QUERY_LOCK = 0xB4,

    CMD_GET_INVENTORY_BUFFER = 0x90,
    CMD_GET_AND_RESET_INVENTORY_BUFFER = 0x91,
    CMD_GET_INVENTORY_BUFFER_TAG_COUNT = 0x92,
    CMD_RESET_INVENTORY_BUFFER = 0x93,
    CMD_BLOCK_WRITE = 0x94, ///< 按块写标签

} RFID_COMMAND;

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

/**
 * @brief 编码
 * @note  输出数据内存空间由调用者自行创建及释放
 * @param[in]  nAddr        读写器地址（供RS485串联使用）
 * @param[in]  nCmd         执行命令，对应 RFID_COMMAND
 * @param[in]  dataIn       输入数据
 * @param[in]  dataInSize   输入数据字节长度
 * @param[out] dataOut      已编码输出数据
 * @param[out] dataOutSize  输出数据字节长度
 * @return 0 正常，-1 参数错误
 */
int rfid_encode(unsigned char nAddr,
                int nCmd,
                unsigned char *dataIn,
                int dataInSize,
                unsigned char *dataOut,
                int *dataOutSize);

/**
 * @brief 解码
 * @note  输出数据内存空间由调用者自行创建及释放
 * @param[in]  dataIn       输入数据
 * @param[in]  dataInSize   输入数据字节长度
 * @param[out] nAddr        返回设备地址
 * @param[out] nCmd         返回当前命令
 * @param[out] dataOut      已解码输出数据
 * @param[out] dataOutSize  输出数据字节长度
 * @return  0 正常
 *         -1 参数错误
 *         -2 未找到数据头
 *         -3 数据大小错误
 */
int rfid_decode(unsigned char *dataIn,
                int dataInSize,
                unsigned char *nAddr,
                unsigned char *nCmd,
                unsigned char *dataOut,
                int *dataOutSize);

#endif // ~#ifndef RFID_CODEC_H
