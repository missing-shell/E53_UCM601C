/**
 * @file
 * @brief ESP32 UCM601C
 *
 */
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct _RfidRWData
    {
        uint16_t tagCount;   // 成功操作的标签个数
        uint8_t dataLen;     // 所操作标签的有效数据长度  ？单位是字节
        uint8_t pc[2];       //
        uint8_t crc[2];      //
        uint8_t *epc;        //
        uint8_t *data;       // 数据
        uint16_t rwLen;      // 读或写操作的数据长度。单位是字节。
        uint8_t frequency;   // 频点参数
        uint8_t antenna;     // 天线号
        uint8_t rwCount;     // 该标签被成功操作的次数
        uint8_t *targetData; // 原始数据
    } RfidRWData;

    /**
     * RfidProtocolModel{status=0, address=0x1, cmd=0x81,
     * sourceData=[ A0 1C 01 81 00 01 12 30 00 E2 80 69 95 00 00 50 05 CF 26 54 63 F0 64 00 00 00 02 28 01 9F ],
     * targetData=[
     * 00 01 // tagCount
     * 12 // 所操作标签的有效数据长度。（PC+CRC+EPC+读取的标签数据）。 单位是字节。
     * 30 00 // 2位PC
     * E2 80 69 95 00 00 50 05 CF 26 54 63 // EPC
     * F0 64 // CRC
     * 00 00 // data
     * 00 02 // rw位数据
     * 28 // antId
     * 01 ]} // rwCount
     *
     * @param model
     */
    typedef struct _RfidProtocolModel
    {
        uint8_t status;      // 0 正常 -1 参数错误 -2 未找到数据头 -3 数据大小错误
        uint8_t address;     // 地址
        uint8_t cmd;         // 命令
        uint8_t *sourceData; // 原始数据
        uint8_t *targetData; // 解码或加协议头后的数据
    } RfidProtocolModel;

    typedef struct _RfidInventoryDataModel
    {
        uint8_t ant;     // 天线号 1字节
        uint8_t pc[2];   // 2字节
        uint8_t *epc;    // 标签规格
        uint8_t crc[2];  // 2字节
        uint8_t *data;   // 根据读写器选择的membank返回数据
        int32_t rssi;    // 信号强度 4字节
        uint8_t freq[3]; // 频率 3字节
    } RfidInventoryDataModel;

    // 定义一个用于存放RfidInventoryDataModel的队列类型
    typedef struct RfidInventoryDataModelQueue
    {
        size_t capacity;                  // 队列容量
        size_t count;                     // 当前元素数量
        RfidInventoryDataModel *elements; // 存放元素的数组
    } RfidInventoryDataModelQueue;

    typedef void (*rfid_inventory_callback_t)(const RfidInventoryDataModel *inventoryData);

    extern void parse_rfid_rw_data(const RfidProtocolModel *model, RfidRWData *rwd);
    extern void diver_write(uint8_t *data, uint8_t len);

#ifdef __cplusplus
}

#endif