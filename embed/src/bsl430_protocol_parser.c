#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "bsl430_protocol_parser.h"
#include "bsl430_protocol_operator.h"

static bool crc16(uint8_t *bytes) {
    uint8_t CKL = bytes[4 + bytes[L1_BYTE]];
    uint8_t CKH = bytes[5 + bytes[L1_BYTE]];
    uint16_t expected = (CKH << 8) | CKL;

    uint16_t actual = 0;
    unsigned int i;
    for (i = 0; i < (4 + bytes[L1_BYTE]); i = i + 2) {
        uint16_t word = (bytes[i+1] << 8) | bytes[i];
        actual = (actual ^ word);
    }
    actual = ~actual;

    return (actual == expected);
}

void bsl430_protocol_parse(uint8_t *bytes) {
    bool crc_ok = crc16(bytes);

    if (bytes[HDR_BYTE] != HDR) {
        return bad_command(crc_ok);
    } else if (bytes[L1_BYTE] != bytes[L2_BYTE]) {
        return bad_command(crc_ok);
    }

    uint16_t addr = (bytes[AH_BYTE] << 8) | bytes[AL_BYTE];

    switch (bytes[CMD_BYTE]) {
    case CMD_LOAD_PC:
        return load_pc(addr, crc_ok);
    case CMD_ERASE_SEGMENT:
        return erase_segment(addr, crc_ok);
    case CMD_RX_DATA_BLOCK: {
        if ((bytes[L1_BYTE]-4) != bytes[LL_BYTE]) {
            return bad_command(crc_ok);
        } else if (bytes[LH_BYTE] != 0) {
            return bad_command(crc_ok);
        } else {
            return rx_data_block(addr, bytes[LL_BYTE], &bytes[LH_BYTE + 1], crc_ok);
        }
    }
    }

    return bad_command(crc_ok);
}
