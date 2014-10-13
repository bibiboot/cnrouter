#include "globals.h"

// Initializing the global structure with default values
struct globals globals = {
    .config = CONFIG_INIT,
    // This is null as per documentation of uthash
    .rentry = NULL,
    .rtable_size = 0,
    .total_packet_sniff = 0
};

