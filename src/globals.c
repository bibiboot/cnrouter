#include "globals.h"

// Initializing the global structure with default values
struct globals globals = {
    .config = CONFIG_INIT,
    // This is null as per documentation of uthash
    .rentry = NULL,
    .rtable_size = 0,
    .total_packet_sniff = 0,
    .rtable_size = 12,
    .rtable_keys = {
                     R_0_1_2_0,
                     R_0_2_1_0,
                     R_0_1_2_1,
                     R_0_2_1_1,
                     R_0_1_3_0,
                     R_0_3_1_0,
                     R_0_1_3_1,
                     R_0_3_1_1,
                     R_0_2_3_0,
                     R_0_3_2_0,
                     R_0_2_3_1,
                     R_0_3_2_1
                   }

};

