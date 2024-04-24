#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "UCM601C.h"
#include "UCM601C_Cmd.h"
void parse_rfid_rw_data(const RfidProtocolModel *model, RfidRWData *rwd)
{
    const uint8_t *targetData = model->targetData;
    size_t targetDataLen = strlen(targetDataLen);

    // Copy the entire targetData array
    memcpy(rwd->targetData, targetData, targetDataLen);

    rwd->tagCount = (targetData[0] << 8) | targetData[1];
    rwd->dataLen = targetData[2];

    // Copy PC
    memcpy(rwd->pc, targetData + 3, sizeof(rwd->pc));

    size_t epcLen = rwd->dataLen - 2 - 2 - rwd->rwLen;
    if (rwd->rwLen == 0)
    {
        epcLen = 12;
    }

    // Copy EPC
    memcpy(rwd->epc, targetData + 5, epcLen);

    // Copy CRC
    memcpy(rwd->crc, targetData + 5 + epcLen, sizeof(rwd->crc));

    // Copy Data
    memcpy(rwd->data, targetData + 5 + epcLen + 2, rwd->dataLen - 2 - epcLen - 2);

    rwd->frequency = targetData[targetDataLen - 2] & 0xFC;
    rwd->antenna = targetData[targetDataLen - 2] & 0x03;
    rwd->rwCount = targetData[targetDataLen - 1];
}

char *rfid_rw_data_to_string(const RfidRWData *rwd, char *buffer, size_t buffer_len)
{
    snprintf(buffer, buffer_len,
             "RfidRWData{tagCount=%u, dataLen=%u, pc=%02X%02X, epc=%s, crc=%02X%02X, data=%s, rwLen=%u, frequency=%u, antenna=%u, rwCount=%u, targetData=%s}",
             rwd->tagCount, rwd->dataLen, rwd->pc[0], rwd->pc[1],
             rwd->epc, rwd->crc[0], rwd->crc[1], rwd->data,
             rwd->rwLen, rwd->frequency, rwd->antenna, rwd->rwCount,
             rwd->targetData);

    return buffer;
}
