#include "globals.h"
#include <linux/if_ether.h>

uint64_t get_pattern(unsigned char *packet);

void set_pattern(unsigned char *packet, uint64_t pttn_num);

void create_log_file();

void pattern_to_human(uint64_t pattern, uint16_t *dest_mac,
                      uint16_t *src_ip, uint16_t *dest_ip,
                      uint16_t *port);
