#include "middleware.h"
#include "packet_send.h"
#include "print_packet.h"
#include "route_table.h"
#include "util.h"

void update_ethernet_packet(unsigned char *packet, unsigned char *src_mac,
                            unsigned char *dest_mac)
{
    /*pointer to ethenet header*/
    unsigned char* etherhead = packet;

    unsigned char* data = packet + ETH_HLEN;
    struct ethhdr *eh = (struct ethhdr *)etherhead;
    memcpy((void*)packet, (void*)dest_mac, ETH_ALEN);
    memcpy((void*)(packet+ETH_ALEN),(void*)src_mac, ETH_ALEN);
}

void incoming_osi_frame_packet_handler(unsigned char *packet, int packet_size)
{
    //Node2 00:04:23:c7:a4:b4
    //Router 00:04:23:c7:a6:46
    char result_if_name[IFNAMSIZ] = "eth2";
    unsigned char dest_mac[6] = {0x00, 0x04, 0x23, 0xc7, 0xa4, 0xb4};
    unsigned char src_mac[6] = {0x00, 0x04, 0x23, 0xc7, 0xa6, 0x46};
    update_ethernet_packet(packet, src_mac, dest_mac);
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

    //struct custom_ethernet *ether_head = (struct custom_ethernet*)packet;
    //ether_head->dest_mac = htons(1);
    //given_pattern = get_pattern(packet);

    get_route_entry(given_pattern, &dest_pattern, result_if_name);

    set_pattern(packet, dest_pattern);

    int src_index = get_interface_index(result_if_name);

    //send_packet_on_line(result_if_name, packet, packet_size);
    send_packet_on_line_fast(src_index, packet, packet_size);
}
