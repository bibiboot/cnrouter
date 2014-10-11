#include "middleware.h"
#include "packet_send.h"
#include "print_packet.h"
#include "route_table.h"
#include "util.h"

void dummy_fill(unsigned char *packet) {
    struct custom_ethernet *eth_header = (struct custom_ethernet*)packet;
    eth_header->dest_mac = htons(2);
}

/**
 * Handle the incoming packet.
 * Either send or append to the queue.
 * Replace the pattern
 */
void incoming_packet_handler(unsigned char *packet, int packet_size)
{
    //char result_if_name[IFNAMSIZ] = "eth2";
    //uint64_t dest_pattern_num = 562954248519680;
    char result_if_name[IFNAMSIZ];
    uint64_t dest_pattern;

    /**
     * Get pattern and interface from table
     * result_if_name   = ?
     * dest_pattern_num = ?
     */
    //dummy_fill(packet);

    uint64_t given_pattern = get_pattern(packet);
    get_route_entry(given_pattern, &dest_pattern, result_if_name);

    set_pattern(packet, dest_pattern);

    send_packet_on_line(result_if_name, packet, packet_size);
}
