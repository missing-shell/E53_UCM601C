#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "UCM601C.h"
#include "UCM601C_Cmd.h"

char *rfid_protocol_model_to_string(const RfidProtocolModel *model, char *buffer, size_t buffer_len)
{
    snprintf(buffer, buffer_len,
             "RfidProtocolModel{status=%d, address=0x%02X, cmd=0x%04X, sourceData=%s, targetData=%s}",
             model->status, model->address, model->cmd,
             model->sourceData, model->targetData);

    return buffer;
}