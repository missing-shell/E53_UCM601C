
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "rfid_codec.h"
#include "rfid_struct.h"

#define READER_HEAD (0xA0)
#define READER_ADDR (0x01)
#define MAX_RAW_DATA_LEN 50

typedef struct _data_header
{
    uint8_t head;
    uint8_t len;
    uint8_t addr;
    uint8_t cmd;
    uint8_t data[0];
} data_header;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

static char s_dataProtocol[MAX_RAW_DATA_LEN] = {0};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

static int CheckSum(unsigned char uBuff[], int uBuffLen)
{
    int i, uSum = 0;
    for (i = 0; i < uBuffLen; i++)
    {
        uSum += uBuff[i];
    }
    uSum = (~uSum) + 1;
    return uSum;
}

// static char * u8toa(unsigned char nVal,char *dstBuf,int nRadix)
// {
//     if ((NULL == dstBuf) || ((nRadix != 10) && (nRadix != 16)))
//     {
//         return NULL;
//     }
//     memset(dstBuf, 0, 3);
//     itoa(nVal, dstBuf, nRadix);
//     if (16 == nRadix)
//     {
//         for (int i = 0; i < 2; i++)
//         {
//             if ((dstBuf[i] >= 'a') && (dstBuf[i] <= 'z'))
//             {
//                 dstBuf[i] -= 0x20;
//             }
//         }
//     }

//     if ((nVal <= 0xF) && (16 == nRadix))
//     {
//         dstBuf[1] = dstBuf[0];
//         dstBuf[0] = '0';
//     }
//     return dstBuf;
// }

int rfid_encode(unsigned char nAddr,
                int nCmd,
                unsigned char *dataIn,
                int dataInSize,
                unsigned char *dataOut,
                int *dataOutSize)
{
    if ((NULL == dataOut) || (NULL == dataOutSize))
    {
        return -1;
    }
    int nRet = 0;
    data_header *pHdr = (data_header *)dataOut;
    pHdr->head = READER_HEAD;
    pHdr->addr = nAddr;
    pHdr->cmd = (uint8_t)nCmd;

    switch (nCmd)
    {
    case CMD_RESET:
    case CMD_GET_FIRMWARE_VERSION:
    case CMD_GET_READER_TEMP:
    case CMD_READ_GPIO_VALUE:
    case CMD_GET_WORK_ANTENNA:
    case CMD_GET_FAST_EPC:
    case CMD_GET_OUTPUT_POWER:
    case CMD_GET_FREQ_REGION:
    case CMD_GET_RF_LINK_PROFILE:
    case CMD_STOP_INVENTORY:
    case CMD_RESET_INVENTORY_BUFFER:
    case CMD_GET_INVENTORY_BUFFER:
    case CMD_GET_AND_RESET_INVENTORY_BUFFER:
    case CMD_GET_INVENTORY_BUFFER_TAG_COUNT:
    {
        pHdr->len = 3;
        break;
    }
    case CMD_WRITE_GPIO_VALUE:
    {
        pHdr->len = 5;
        if ((NULL == dataIn) || (dataInSize < 1))
        {
            nRet = -1;
            break;
        }
        s_dataProtocol[0] = 0x03;
        s_dataProtocol[1] = dataIn[0];
        break;
    }
    case CMD_SET_BEEP_MODE:
    {
        pHdr->len = 4;
        break;
    }
    case CMD_SET_UART_BAUDRATE:
    case CMD_SET_WORK_ANTENNA:
    case CMD_SET_FAST_EPC:
    case CMD_SET_OUTPUT_POWER:
    case CMD_SET_RF_LINK_PROFILE:
    case CMD_REAL_TIME_INVENTORY:
    case CMD_INVENTORY:
    {
        pHdr->len = 4;
        if ((NULL == dataIn) || (dataInSize < 1))
        {
            nRet = -1;
            break;
        }
        s_dataProtocol[0] = dataIn[0];
        break;
    }
    case CMD_SET_FREQ_REGION:
    {
        if ((NULL == dataIn) || (dataInSize < sizeof(frequency_region_default)))
        {
            nRet = -1;
            break;
        }
        frequency_region_default *freqDefault = (frequency_region_default *)dataIn;
        if (FREQ_CUSTOM == freqDefault->region)
        {
            union
            {
                uint32_t u32;
                uint8_t u8[4];
            } freq;
            frequency_region_custom *freqCustom = (frequency_region_custom *)dataIn;
            pHdr->len = 9;
            s_dataProtocol[0] = (uint8_t)freqCustom->region;
            s_dataProtocol[1] = freqCustom->freqSpace;
            s_dataProtocol[2] = freqCustom->freqQuantity;
            freq.u32 = freqCustom->startFreq;
            s_dataProtocol[3] = freq.u8[2];
            s_dataProtocol[4] = freq.u8[1];
            s_dataProtocol[5] = freq.u8[0];
        }
        else
        {
            pHdr->len = 6;
            s_dataProtocol[0] = (uint8_t)freqDefault->region;
            s_dataProtocol[1] = freqDefault->startFreq;
            s_dataProtocol[2] = freqDefault->endFreq;
        }

        break;
    }
    case CMD_CUS_SESSION_TARGET_INV:
    {
        pHdr->len = 6;
        if ((NULL == dataIn) || (dataInSize < sizeof(custom_inventory)))
        {
            nRet = -1;
            break;
        }
        custom_inventory *ci = (custom_inventory *)dataIn;
        s_dataProtocol[0] = ci->session;
        s_dataProtocol[0] = ci->target;
        s_dataProtocol[0] = ci->ant;
        break;
    }
    case CMD_SET_ACCESS_EPC_MATCH:
    {
        if (NULL == dataIn)
        {
            nRet = -1;
            break;
        }
        epc_match *epc = (epc_match *)dataIn;
        if (EPC_MODE_STILL == epc->mode)
        {
            pHdr->len = 17;
            s_dataProtocol[0] = epc->mode;
            s_dataProtocol[1] = epc->len;
            for (int nCnt = 0; nCnt < 12; nCnt++)
            {
                s_dataProtocol[2 + nCnt] = epc->data[nCnt];
            }
        }
        else
        {
            pHdr->len = 4;
            s_dataProtocol[0] = EPC_MODE_CLEAR;
        }
        break;
    }
    case CMD_READ:
    {
        union
        {
            uint32_t u32;
            uint8_t u8[4];
        } data_u32;
        union
        {
            uint16_t u16;
            uint8_t u8[2];
        } data_u16;
        if ((NULL == dataIn) || (dataInSize < sizeof(rfid_rw_data)))
        {
            nRet = -1;
            break;
        }
        pHdr->len = 14;
        rfid_rw_data *rfidRead = (rfid_rw_data *)dataIn;
        s_dataProtocol[0] = rfidRead->memBank;
        data_u32.u32 = rfidRead->wordAddr;
        s_dataProtocol[1] = data_u32.u8[3];
        s_dataProtocol[2] = data_u32.u8[2];
        s_dataProtocol[3] = data_u32.u8[1];
        s_dataProtocol[4] = data_u32.u8[0];
        data_u16.u16 = rfidRead->wordCnt;
        s_dataProtocol[5] = data_u16.u8[1];
        s_dataProtocol[6] = data_u16.u8[0];
        data_u32.u32 = rfidRead->password;
        s_dataProtocol[7] = data_u32.u8[3];
        s_dataProtocol[8] = data_u32.u8[2];
        s_dataProtocol[9] = data_u32.u8[1];
        s_dataProtocol[10] = data_u32.u8[0];
        break;
    }
    case CMD_WRITE:
    case CMD_BLOCK_WRITE:
    {
        union
        {
            uint32_t u32;
            uint8_t u8[4];
        } data_u32;
        union
        {
            uint16_t u16;
            uint8_t u8[2];
        } data_u16;
        if ((NULL == dataIn) || (dataInSize < sizeof(rfid_rw_data)))
        {
            nRet = -1;
            break;
        }
        rfid_rw_data *rfidWr = (rfid_rw_data *)dataIn;
        data_u32.u32 = rfidWr->password;
        s_dataProtocol[0] = data_u32.u8[3];
        s_dataProtocol[1] = data_u32.u8[2];
        s_dataProtocol[2] = data_u32.u8[1];
        s_dataProtocol[3] = data_u32.u8[0];
        s_dataProtocol[4] = rfidWr->memBank;
        data_u32.u32 = rfidWr->wordAddr;
        s_dataProtocol[5] = data_u32.u8[3];
        s_dataProtocol[6] = data_u32.u8[2];
        s_dataProtocol[7] = data_u32.u8[1];
        s_dataProtocol[8] = data_u32.u8[0];
        data_u16.u16 = rfidWr->wordCnt;
        s_dataProtocol[9] = data_u16.u8[1];
        s_dataProtocol[10] = data_u16.u8[0];
        for (int nCnt = 0; nCnt < rfidWr->wordCnt * 2; nCnt++)
        {
            s_dataProtocol[11 + nCnt] = rfidWr->data[nCnt];
        }
        pHdr->len = 14 + rfidWr->wordCnt * 2;
        break;
    }
    case CMD_LOCK:
    {
        union
        {
            uint32_t u32;
            uint8_t u8[4];
        } data_u32;
        if ((NULL == dataIn) || (dataInSize < sizeof(rfid_lock)))
        {
            nRet = -1;
            break;
        }
        rfid_lock *lock = (rfid_lock *)dataIn;
        data_u32.u32 = lock->password;
        s_dataProtocol[0] = data_u32.u8[3];
        s_dataProtocol[1] = data_u32.u8[2];
        s_dataProtocol[2] = data_u32.u8[1];
        s_dataProtocol[3] = data_u32.u8[0];
        s_dataProtocol[4] = lock->memBank;
        s_dataProtocol[5] = lock->lockType;
        pHdr->len = 9;
        break;
    }
    case CMD_KILL:
    {
        union
        {
            uint32_t u32;
            uint8_t u8[4];
        } data_u32;
        data_u32.u32 = *(uint32_t *)dataIn;
        s_dataProtocol[0] = data_u32.u8[3];
        s_dataProtocol[1] = data_u32.u8[2];
        s_dataProtocol[2] = data_u32.u8[1];
        s_dataProtocol[3] = data_u32.u8[0];
        pHdr->len = 7;
        break;
    }
    case CMD_RAW_DATA:
    {
        if ((NULL == dataIn) || (dataInSize > MAX_RAW_DATA_LEN))
        {
            nRet = -1;
            break;
        }
        pHdr->cmd = dataIn[0];
        pHdr->len = 3 + dataInSize;
        for (int nCnt = 0; nCnt < (dataInSize - 1); nCnt++)
        {
            s_dataProtocol[nCnt] = dataIn[nCnt + 1];
        }
        break;
    }
    default:
        break;
    }

    if (nRet)
    {
        memset(s_dataProtocol, 0, MAX_RAW_DATA_LEN);
        return nRet;
    }

    for (int i = 0; i < (pHdr->len - 3); i++)
    {
        pHdr->data[i] = s_dataProtocol[i];
    }
    dataOut[pHdr->len + 1] = CheckSum(dataOut, pHdr->len + 4);

    *dataOutSize = pHdr->len + 2;
    memset(s_dataProtocol, 0, MAX_RAW_DATA_LEN);

    return nRet;
}

int rfid_decode(unsigned char *dataIn,
                int dataInSize,
                unsigned char *nAddr,
                unsigned char *nCmd,
                unsigned char *dataOut,
                int *dataOutSize)
{
    if ((NULL == dataIn) || (dataInSize < sizeof(data_header)) ||
        (NULL == nAddr) || (NULL == nCmd) ||
        (NULL == dataOut) || (NULL == dataOutSize))
    {
        return -1;
    }
    int nRet = 0;
    data_header *pHdr = (data_header *)dataIn;
    if (READER_HEAD != pHdr->head)
    {
        return -2;
    }
    if ((pHdr->len < 3) || (pHdr->len > MAX_RAW_DATA_LEN))
    {
        return -3;
    }
    *nAddr = pHdr->addr;
    *nCmd = pHdr->cmd;
    int nCheckSum = CheckSum(dataIn, pHdr->len + 1);
    //    if (dataIn[pHdr->len + 1] != (uint8_t)nCheckSum)
    //    {
    //        return -4;
    //    }
    if (CMD_GET_FREQ_REGION == pHdr->cmd)
    {
        frequency_region_custom *pFreqCustom = (frequency_region_custom *)pHdr->data;
        if (FREQ_CUSTOM == pFreqCustom->region)
        {
            frequency_region_custom custom;
            memset(&custom, 0, sizeof(frequency_region_custom));
            memcpy(&custom, pHdr->data, 3);
            memcpy(&custom.startFreq, &pHdr->data[3], 3);
            custom.startFreq = custom.startFreq << 8;
            *dataOutSize = sizeof(frequency_region_custom);
            return nRet;
        }
    }
    *dataOutSize = pHdr->len - 3;
    if (*dataOutSize > 0)
    {
        memcpy(dataOut, pHdr->data, *dataOutSize);
    }
    return nRet;
}
