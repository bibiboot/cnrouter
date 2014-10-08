#include "middleware.h"
#include "packet_send.h"
#include "packet_update.h"
#include "print_packet.h"
#include "route.h"
#include "socket_util.h"
#include "route_table.h"
#include "util.h"
#include<netinet/udp.h>   //Provides declarations for udp header

/**
 * Based on the source ip,
 * it can tell us where it was recieved,
 * that is which interface
 */
void get_recived_interface(uint32_t dest_ip, char *res_interface) {
    // Ip of the sending router with RIP packet
    if ( dest_ip == char_to_uint32(NODE1_IP) ) {
       strcpy(res_interface, INF1);
    } else if ( dest_ip == char_to_uint32(NODE2_IP) ) {
       strcpy(res_interface, INF2);
    } else {
       printf("RIP: This should never happen, No interface found for : ");
       print_ip(dest_ip);
       printf("\n");
       exit(1);
    }
}

void incoming_packet_handler_ttl_zero(unsigned char *packet, int size) {

    unsigned char dest_mac[6], src_mac[6];
    char result_if_name[IFNAMSIZ];
    struct sockaddr_in dest;
    struct iphdr *iph = (struct iphdr*)(packet + sizeof(struct ethhdr));

    if ( iph->protocol != 1 ) {
        printf("WARN : NOT HANDLED TTL FOR NON ICMP\n");
    }

    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;

    // Routing
    get_new_route(iph->saddr, result_if_name, dest_mac, src_mac);

    if (PRINT_ROUTE_DEBUG) {
        print_routed_packet(dest, result_if_name, src_mac, dest_mac);
    }

    int new_packet_size = get_resize_icmp_packet_time_exc_reply(packet,size);
    new_packet_size = 70;
    printf("Old size = %d and New size = %d\n", size, new_packet_size);

    unsigned char *new_packet = malloc(new_packet_size);
    memset(new_packet, 0, new_packet_size);

    //Copy the whole old packet to the new packet
    memcpy(new_packet, packet, new_packet_size);

    // Reply back to the source mac address
    update_ethernet_packet_reply(new_packet);

    update_icmp_packet_time_exc_reply(new_packet, new_packet_size);

    update_ip_packet_time_exc_reply(new_packet);

    send_packet_on_line(result_if_name, dest_mac, new_packet, new_packet_size);

    free(new_packet);
}

/**
 * Handle the incoming packet whose ttl = 0.
 * Update the ethernet header with corrrect mac address
 *     source mac address
 *     destination mac address
 * Update the ip header with correct ip address
 *     source ip address
 *     destination ip address
 *     ttl = 64
 *     checksum
 * Update the icmp header
 *     change type to 18
 *     checksum
 */
void incoming_packet_handler_self_icmp(unsigned char *packet, int size)
{
    unsigned char dest_mac[6], src_mac[6];
    char result_if_name[IFNAMSIZ];
    struct sockaddr_in dest;
    struct iphdr *iph = (struct iphdr*)(packet + sizeof(struct ethhdr));

    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;

    // Routing
    get_new_route(iph->saddr, result_if_name, dest_mac, src_mac);

    if (PRINT_ROUTE_DEBUG) {
        print_routed_packet(dest, result_if_name, src_mac, dest_mac);
    }

    // Reply back to the source mac address
    update_ethernet_packet_reply(packet);

    update_ip_packet_reply(packet);

    update_icmp_packet_reply(packet, size);

    send_packet_on_line(result_if_name, dest_mac, packet, size);
}

/**
 * Handle the incoming packet.
 * Either send or append to the queue.
 */
void incoming_packet_handler(unsigned char *packet, int size){

    unsigned char dest_mac[6], src_mac[6];
    char result_if_name[IFNAMSIZ];
    struct sockaddr_in dest;
    struct iphdr *iph = (struct iphdr*)(packet + sizeof(struct ethhdr));

    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;

    // Routing
    get_new_route(iph->daddr, result_if_name, dest_mac, src_mac);

    if (PRINT_ROUTE_DEBUG) {
        print_routed_packet(dest, result_if_name, src_mac, dest_mac);
    }

    update_ip_packet(packet);

    update_ethernet_packet(packet, src_mac, dest_mac);

    send_packet_on_line(result_if_name, dest_mac, packet, size);
}

/**
 * Prepare packet
 * Change ip and ethernet header
 * Send packet on line
 * Deprecated ********************************* Deprecated
 */
void prepare_n_send_packet(unsigned char *packet, int size,
                           unsigned char *dest_mac, char *if_name){

    update_ip_packet(packet);

    update_ethernet_packet(packet, globals.eth1_mac, dest_mac);

    send_packet_on_line(if_name, dest_mac, packet, size);
}
