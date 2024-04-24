/**
 * @file rfid_struct.h
 * @author ucchip
 * @brief 编解码相关数据结构定义
 * @version 0.1
 * @date 2022-06-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef RFID_STRUCT_H
#define RFID_STRUCT_H

/**
 * @section Encode 编码数据类型定义
 *
 */

/**
 * @brief 串口波特率配置 对应 CMD_SET_UART_BAUDRATE
 *
 */
typedef enum
{
    BAUDRATE_4800,
    BAUDRATE_9600,
    BAUDRATE_14400,
    BAUDRATE_19200,
    BAUDRATE_38400,
    BAUDRATE_56000,
    BAUDRATE_57600,
    BAUDRATE_115200,
    BAUDRATE_128000,
    BAUDRATE_256000,
    BAUDRATE_460800

} SERIAL_PORT_BAUDRATE;

/**
 * @brief 视频链路属性，对应命令 cmd_set_rf_link_profile/cmd_get_rf_link_profile
 *
 */
typedef enum
{
    RF_LINK_PROFILE_ID0, /// Profile 0：FM0 40KHz
    RF_LINK_PROFILE_ID1, /// Profile 1：FM0 200KHz（默认）
    RF_LINK_PROFILE_ID2, /// Profile 2：Miller_4 250KHz
    RF_LINK_PROFILE_ID3  /// Profile 3：FM0 640KHz
} RF_LINK_PROFILE_ID;

/**
 * @brief 射频规范
 *
 */
typedef enum
{
    FREQ_CCE = 1,
    FREQ_ETSI,
    FREQ_CHN,
    FREQ_CUSTOM, ///< 用户自定义
} FREQUENCY_STANDARD;

/**
 * @brief 射频频谱默认结构
 * @note
 * 对应命令 CMD_SET_FREQ_REGION/CMD_GET_FREQ_REGION
 * 可以在射频规范的频率范围内再设置跳频的范围。
 * 参数所对应的频率请参见频率参数对应表。
 * 参数的设置规则为：
 * 1. 起始频率与结束频率不能超过射频规范的范围。
 * 2. 起始频率必须低于结束频率。
 * 3. 起始频率等于结束频率则定频发射。
 *
 */
typedef struct _frequency_region_default
{
    uint8_t region;    ///< 射频规范 FREQ_CCE/FREQ_ETSI/FREQ_CHN
    uint8_t startFreq; ///< 频率起始点
    uint8_t endFreq;   ///< 频率结束点
    uint8_t nReserve;  ///< 未定义

} frequency_region_default;

/**
 * @brief 射频频谱自定义结构，对应命令 CMD_SET_FREQ_REGION/CMD_GET_FREQ_REGION
 *
 */
typedef struct _frequency_region_custom
{
    uint8_t region;       ///< 射频规范 FREQ_CUSTOM
    uint8_t freqSpace;    ///< 频点间隔
    uint8_t freqQuantity; ///< 频点数量
    uint8_t nReserve;     ///< 未定义
    uint32_t startFreq;   ///< 起始频率

} frequency_region_custom;

/**
 * @brief 指定盘存的session
 *
 */
typedef enum
{
    SESSION_S0,
    SESSION_S1,
    SESSION_S2,
    SESSION_S3
} CUSTOMIZED_SESSION;
/**
 * @brief 指定盘存的Inventory Flag
 *
 */
typedef enum
{
    INVENTORY_A,
    INVENTORY_B
} INVENTORY_FLAG;
/**
 * @brief 对应命令 cmd_customized_session_target_inventory
 *
 */
typedef struct _customized_session_target_inventory
{
    uint8_t session; ///< 指定盘存的session，对应CUSTOMIZED_SESSION
    uint8_t target;  ///< 指定盘存的Inventory Flag，对应INVENTORY_FLAG
    uint8_t ant;     ///< 天线号
} custom_inventory;

typedef enum
{
    EPC_MODE_STILL, ///< EPC匹配一直有效，直到下一次刷新
    EPC_MODE_CLEAR, ///< 清除EPC匹配
} EPC_MATCH_MODE;
/**
 * @brief 对应命令 CMD_SET_ACCESS_EPC_MATCH
 *
 */
typedef struct _epc_match
{
    uint8_t mode;    ///< 对应EPC_MATCH_MODE
    uint8_t len;     ///< EPC数据长度
    uint8_t data[0]; ///< EPC数据内容
} epc_match;

/**
 * @brief 标签存储区域
 *
 */
typedef enum
{
    MEM_BANK_RESERVED, ///< 保留
    MEM_BANK_EPC,
    MEM_BANK_TID,
    MEM_BANK_USER
} MEM_BANK_RFID_RW;
/**
 * @brief 对应(编码)命令 CMD_READ/CMD_WRITE/CMD_BLOCK_WRITE
 * @note  这是一个变长的结构体
 */
typedef struct _rfid_rw_data
{
    uint8_t memBank;   ///< 对应 MEM_BANK_RFID_RW
    uint8_t nReserve;  ///< 保留
    uint16_t wordCnt;  ///< 读/写数据长度
    uint32_t wordAddr; ///< 读/写数据首地址
    uint32_t password; ///< 标签访问密码
    uint8_t data[0];   ///< 写命令对应数据域
} rfid_rw_data;

/**
 * @brief 操作的数据区域
 *
 */
typedef enum
{
    LOCK_USER = 1,
    LOCK_TID,
    LOCK_EPC,
    LOCK_ACCESS,
    LOCK_KILL
} MEM_BANK_LOCK;

typedef enum
{
    LOCK_TYPE_OPEN,        ///< 开放
    LOCK_TYPE_LOCK,        ///< 锁定
    LOCK_TYPE_OPEN_ALWAYS, ///< 永久开放
    LOCK_TYPE_LOCK_ALWAYS  ///< 永久锁定
} LOCK_TYPE;

/**
 * @brief 对应命令 CMD_LOCK
 *
 */
typedef struct _rfid_lock
{
    uint8_t memBank;   ///< 对应 MEM_BANK_LOCK
    uint8_t lockType;  ///< 对应 LOCK_TYPE
    uint16_t nReserve; ///< 保留
    uint32_t password; ///< 标签访问密码
} rfid_lock;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/**
 * @section decode 解码数据类型定义
 *
 */

/// 复位成功，对应命令 CMD_RESET
#define CMD_RESET_SUCCESS (0x10)
/// 缓存为空错误，对应命令 CMD_GET_INVENTORY_BUFFER
#define CMD_GET_INVENTORY_BUFFER_EMPTY (0x38)

/**
 * @brief 错误码，对应命令 CMD_READ
 *
 */
typedef enum
{
    CMD_RX_ERR_TAG = 0x32,          ///< 读标签错误
    CMD_RX_ERR_NO_OP_TAG = 0x36,    ///< 无可操作标签错误
    CMD_RX_ERR_ACCESS,              ///< 成功盘存但访问失败
    CMD_RX_ERR_PASSWORD = 0x40,     ///< 访问标签错误或访问密码错误
    CMD_RX_ERR_INVALID_PARAMS,      ///< 无效的参数
    CMD_RX_ERR_WORDCNT_LENGTH,      ///< wordCnt参数超过规定长度
    CMD_RX_ERR_MEMBANK_OUT_OF_RANGE ///< MemBank参数超出范围
} CMD_READ_ERROR;

/**
 * @brief 版本号，对应命令 CMD_GET_FIRMWARE_VERSION
 *
 */
typedef struct _firmware_version
{
    uint8_t major; ///< 主版本号
    uint8_t minor; ///< 次版本号
} firmware_version;

typedef enum
{
    TEMP_MINUS, ///< 负数
    TEMP_PLUS   ///< 正数
} TEMP_PLUS_MINUS;

/**
 * @brief 温度，对应命令 CMD_GET_READER_TEMP
 *
 */
typedef struct _reader_temp
{
    uint8_t plusminus; ///< 正负号，对应 TEMP_PLUS_MINUS
    uint8_t temp;      ///< 温度（绝对）值
} reader_temp;

/**
 * @brief 天线号，对应命令 CMD_GET_WORK_ANTENNA
 *
 */
typedef enum
{
    ANTENNA_NO_1, ///< 天线1
    ANTENNA_NO_2, ///< 天线2
    ANTENNA_NO_3, ///< 天线3
    ANTENNA_NO_4  ///< 天线4
} WORK_ANTENNA_SEQ;

/**
 * @brief 天线输出功率，对应命令 CMD_GET_OUTPUT_POWER
 *
 */
typedef struct _output_power
{
    uint8_t nPower1; ///< 天线1输出功率，取值0-33, 单位dBm
    uint8_t nPower2; ///< 天线2输出功率
    uint8_t nPower3; ///< 天线3输出功率
    uint8_t nPower4; ///< 天线4输出功率
} output_power;

#endif // ~#ifndef RFID_STRUCT_H