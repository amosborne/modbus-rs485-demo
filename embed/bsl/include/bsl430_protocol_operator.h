#include <stdint.h>
#include <stdbool.h>

void rx_data_block( uint16_t addr, uint16_t length, uint8_t *data, bool crc_ok );

void erase_segment( uint16_t addr, bool crc_ok );

void load_pc( uint16_t addr, bool crc_ok );

void bad_command( bool crc_ok );
