#include "middleware.h"
#include "packet_send.h"
#include "print_packet.h"
#include "route_table.h"
#include "util.h"
#include "socket_util.h"

void get_route_entry_deprecated(unsigned char *packet, char *result_if_name)
{
    struct custom_ethernet *ether_header = (struct custom_ethernet*)packet;
    struct custom_ip *ip_header = (struct custom_ip*)(packet + C_ETHLEN);
    switch(ntohs(ip_header->dest_ip)) {
        case 1:
            ether_header->dest_mac = htons(1);
            strcpy(result_if_name, INF0);
            break;
        case 2:
            ether_header->dest_mac = htons(2);
            strcpy(result_if_name, INF2);
            break;
        case 3:
            ether_header->dest_mac = htons(3);
            strcpy(result_if_name, INF1);
            break;
        default:
            printf("Route: This should never happen\n");
            exit(1);
    }
}

void incoming_packet_handler_deprecated(unsigned char *packet,
                                        int packet_size)
{
    char result_if_name[IFNAMSIZ];

    /**
     * Get pattern and interface from table
     * result_if_name   = ?
     * dest_pattern_num = ?
     */

    get_route_entry_deprecated(packet, result_if_name);

    int src_index = get_interface_index(result_if_name);

    send_packet_on_line_fast(src_index, packet, packet_size);
}

/**
 * Handle the incoming packet.
 * Either send or append to the queue.
 * Replace the pattern
 */
void incoming_packet_handler(unsigned char *packet, int packet_size)
{
    char result_if_name[IFNAMSIZ];
    uint64_t dest_pattern;

    /**
     * Get pattern and interface from table
     * result_if_name   = ?
     * dest_pattern_num = ?
     */

    uint64_t given_pattern = get_pattern(packet);

    get_route_entry_switch(given_pattern, &dest_pattern, result_if_name);

    set_pattern(packet, dest_pattern);

    int src_index = get_interface_index(result_if_name);

    send_packet_on_line_fast(src_index, packet, packet_size);
}
