#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>

#include "bsl430_protocol_parser.h"
#include "bsl430_protocol_operator.h"

void __wrap_load_pc(uint16_t addr, bool crc_ok) {
    check_expected(addr);
}

void __wrap_erase_segment(uint16_t addr, bool crc_ok) {
    check_expected(addr);
}

void __wrap_rx_data_block(uint16_t addr, uint16_t length, uint8_t *data, bool crc_ok) {
    check_expected(addr);
    check_expected(length);
    check_expected_ptr(data);
}

void __wrap_bad_command(bool crc_ok) {
    check_expected(crc_ok);
}

static uint8_t LOAD_PC_CMD[10]       = {HDR, CMD_LOAD_PC,       0x04, 0x04, 0x00, 0xC0, 0xFF, 0xFF, 0x00, 0x00};
static uint8_t ERASE_SEGMENT_CMD[10] = {HDR, CMD_ERASE_SEGMENT, 0x04, 0x04, 0x00, 0xC0, 0x02, 0xA5, 0x00, 0x00};
static uint8_t RX_DATA_BLOCK_CMD[12] = {HDR, CMD_RX_DATA_BLOCK, 0x06, 0x06, 0x00, 0xC0, 0x02, 0x00, 0xAB, 0xCD, 0x00, 0x00};
static uint8_t TX_DATA_BLOCK_CMD[10] = {HDR, 0x14,              0x04, 0x04, 0x00, 0x0F, 0x0E, 0x00, 0x75, 0xE0};

static void test_bad_header(void **state) {
    uint8_t BAD_HEADER_LOAD_PC_CMD[10];
    memcpy(BAD_HEADER_LOAD_PC_CMD, LOAD_PC_CMD, 10);

    // set the header byte to something invalid
    BAD_HEADER_LOAD_PC_CMD[HDR_BYTE] = 0x00;
    expect_value(__wrap_bad_command, crc_ok, false);
    bsl430_protocol_parse(BAD_HEADER_LOAD_PC_CMD);
}

static void test_bad_length(void **state) {
    uint8_t BAD_LENGTH_LOAD_PC_CMD[10];
    memcpy(BAD_LENGTH_LOAD_PC_CMD, LOAD_PC_CMD, 10);

    // set the length bytes to something invalid
    BAD_LENGTH_LOAD_PC_CMD[L1_BYTE] = 0x00;
    expect_value(__wrap_bad_command, crc_ok, false);
    bsl430_protocol_parse(BAD_LENGTH_LOAD_PC_CMD);
}

static void test_good_crc16(void **state) {
    expect_value(__wrap_bad_command, crc_ok, true);
    bsl430_protocol_parse(TX_DATA_BLOCK_CMD);
}

static void test_load_pc(void **state) {
    uint8_t SET_ADDRESS_LOAD_PC_CMD[10];
    memcpy(SET_ADDRESS_LOAD_PC_CMD, LOAD_PC_CMD, 10);

    // use the default address
    expect_value(__wrap_load_pc, addr, 0xC000);
    bsl430_protocol_parse(SET_ADDRESS_LOAD_PC_CMD);

    // use a different address
    SET_ADDRESS_LOAD_PC_CMD[AH_BYTE] = 0xDE;
    SET_ADDRESS_LOAD_PC_CMD[AL_BYTE] = 0xAD;
    expect_value(__wrap_load_pc, addr, 0xDEAD);
    bsl430_protocol_parse(SET_ADDRESS_LOAD_PC_CMD);
}

static void test_erase_segment(void **state) {
    uint8_t SET_ADDRESS_ERASE_SEGMENT_CMD[10];
    memcpy(SET_ADDRESS_ERASE_SEGMENT_CMD, ERASE_SEGMENT_CMD, 10);

    // use the default address
    expect_value(__wrap_erase_segment, addr, 0xC000);
    bsl430_protocol_parse(SET_ADDRESS_ERASE_SEGMENT_CMD);

    // use a different address
    SET_ADDRESS_ERASE_SEGMENT_CMD[AH_BYTE] = 0xBE;
    SET_ADDRESS_ERASE_SEGMENT_CMD[AL_BYTE] = 0xEF;
    expect_value(__wrap_erase_segment, addr, 0xBEEF);
    bsl430_protocol_parse(SET_ADDRESS_ERASE_SEGMENT_CMD);
}

static void test_rx_data_block(void **state) {
    uint8_t SET_LENGTH_RX_DATA_BLOCK_CMD[12];
    memcpy(SET_LENGTH_RX_DATA_BLOCK_CMD, RX_DATA_BLOCK_CMD, 12);

    // use the default command
    uint8_t expected_data[2] = {0xAB, 0xCD};
    expect_value(__wrap_rx_data_block, addr, 0xC000);
    expect_value(__wrap_rx_data_block, length, 2);
    expect_memory(__wrap_rx_data_block, data, expected_data, 2);
    bsl430_protocol_parse(SET_LENGTH_RX_DATA_BLOCK_CMD);

    // set LL to be an incorrect length relative to L1/L2
    SET_LENGTH_RX_DATA_BLOCK_CMD[LL_BYTE] = 0x01;
    expect_value(__wrap_bad_command, crc_ok, false);
    bsl430_protocol_parse(SET_LENGTH_RX_DATA_BLOCK_CMD);
    SET_LENGTH_RX_DATA_BLOCK_CMD[LL_BYTE] = 0x02;

    // set LH to be nonzero
    SET_LENGTH_RX_DATA_BLOCK_CMD[LH_BYTE] = 0x01;
    expect_value(__wrap_bad_command, crc_ok, false);
    bsl430_protocol_parse(SET_LENGTH_RX_DATA_BLOCK_CMD);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_bad_header),
        cmocka_unit_test(test_bad_length),
        cmocka_unit_test(test_good_crc16),
        cmocka_unit_test(test_load_pc),
        cmocka_unit_test(test_erase_segment),
        cmocka_unit_test(test_rx_data_block),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
