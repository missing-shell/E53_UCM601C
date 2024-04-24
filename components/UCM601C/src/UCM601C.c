#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
// UCM601C驱动
#include "UCM601C.h"
#include "UCM601C_Cmd.h"
// 编解码
#include "rfid_codec.h"
#include "rfid_struct.h"
// 提高移植性
#include "transplant.h"

static const int *TAG = "UCM601C";

/**
 * 将指定的命令写入RFID驱动器。
 *
 * @param send_data 需要写入的命令数组(uint8_t)。
 */

/******************************************************************************/
// TODO: 错误处理，是否写入成功
static void write_command(uint8_t *send_data)
{
    // TODO：是否可以换成uint8_t?
    uint16_t len = sizeof(*send_data) / sizeof(send_data[0]);
    diver_write(send_data, len);
}
static void write_commad_one(uint8_t *send_data)
{
    diver_write(send_data, 1);
}
/******************************************************************************/

/**
 * 复位读写器命令
 *
 * @param address 地址
 * @return
 */
static void resetLowerMachine(uint8_t address)
{
    uint8_t *encode_data = NULL;
    uint8_t encode_data_len = 0;
    rifd_err_t err;

    err = rfid_encode(address, CMD_RESET, NULL, 0, encode_data, &encode_data_len);

    if (!err)
    {
        RIFD_LOGE(TAG, "rfid_encode err=%u at address %u", err, address);
        return; // 提前返回，防止进一步操作
    }

    if (encode_data != NULL)
    {
        write_commad_one(encode_data);
        free(encode_data);  // 确保释放分配的内存
        encode_data = NULL; // 重置指针为NULL，防止悬垂指针
    }
    else
    {
        RIFD_LOGE(TAG, "rfid_encode succeeded but no data was returned for address %u", address);
    }
}
/**
 * 获取固件地址
 *
 * @param[in]  address          地址
 * @param[out] Major            固件主版本号
 * @param[out] Minor            固件次版本号
 */
static void RfidFirmwareVersion(uint8_t address, uint8_t *Major, uint8_t *Minor)
{
    uint8_t *encode_data = NULL;
    uint8_t encode_data_len = 0;
    rfid_err_t err;

    err = rfid_encode(address, CMD_GET_FIRMWARE_VERSION, NULL, 0, encode_data, &encode_data_len);

    if (!err)
    {
        RIFD_LOGE(TAG, "rfid_encode err=%u at address %u", err, address);
        return;
    }

    if (encode_data != NULL)
    {
        write_commad_one(encode_data);
        free(encode_data);
        encode_data = NULL;
        // TODO: 读取数据
        // Major
        // Minor
    }
    else
    {
        RIFD_LOGE(TAG, "rfid_encode succeeded but no data was returned for address %u", address);
    }
}
static void real_time_inventory_callback(RfidInventoryDataModelQueue *queue)
{
    // TODO: 实现回调逻辑，处理队列中的RfidInventoryDataModel数据
}
/**
 * 实时上传标签数据的盘存标签功能。
 * 在实时模式（Auto）下，读写器收到此命令后会进行多标签识别操作，标签数据会实时上传，不会被存储在读写器的缓存区。这个命令适用于需要读取大量标签的场景，因为一轮盘存操作可能会持续较长时间。
 *
 * @param driver 读写器驱动，用于与读写器进行通信。
 * @param address 读写器的地址，用于识别特定的读写器。
 * @param workAntenna 天线编号，指定使用哪个天线进行标签读取。
 * @param callback 回调接口，用于接收实时上传的标签数据。
 * @return 返回错误码
 */
static void real_time_inventory(uint8_t address, uint8_t workAntenna, rfid_inventory_callback_t callback)
{
    uint8_t *encode_data = NULL;
    uint8_t encode_data_len = 0;
    rfid_err_t err;

    err = rfid_encode(address, CMD_REAL_TIME_INVENTORY, NULL, 0, encode_data, &encode_data_len);

    if (!err)
    {
        RIFD_LOGE(TAG, "rfid_encode err=%u at address %u", err, address);
        return;
    }

    if (encode_data != NULL)
    {
        write_commad(encode_data);
        free(encode_data);
        encode_data = NULL;
        // TODO: 调用回调函数,处理接收的数据
        callback();
    }
    else
    {
        RIFD_LOGE(TAG, "rfid_encode succeeded but no data was returned for address %u", address);
    }
}