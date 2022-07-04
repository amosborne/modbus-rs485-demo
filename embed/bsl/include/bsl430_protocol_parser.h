#include <stdint.h>

#define HDR               0x80
#define CMD_RX_DATA_BLOCK 0x12
#define CMD_ERASE_SEGMENT 0x16
#define CMD_LOAD_PC       0x1A

#define HDR_BYTE 0
#define CMD_BYTE 1
#define L1_BYTE  2
#define L2_BYTE  3
#define AL_BYTE  4
#define AH_BYTE  5
#define LL_BYTE  6
#define LH_BYTE  7

void bsl430_protocol_parse( uint8_t *bytes );
