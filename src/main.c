#include "packet_sniffer.h"
#include "arp.h"
#include "route_table.h"
#include "socket_util.h"
#include "util.h"
#include "globals.h"

void init_own_ip() {
    globals.eth0_ip = char_to_uint32(ETH0_IP);
    globals.eth1_ip = char_to_uint32(ETH1_IP);
    globals.eth2_ip = char_to_uint32(ETH2_IP);
}

/**
 * Router's interfaces and its mac addresss
 * Needed to sniff only certain interfaces
 */
void init_mac_addr(){

    interface_addr(globals.send_sock_fd, INF0, globals.eth1_mac);
    interface_addr(globals.send_sock_fd, INF1, globals.eth2_mac);
    interface_addr(globals.send_sock_fd, INF2, globals.eth3_mac);

}

void start(){
    void *val;

    pthread_create(&globals.sniff_th, 0, sniff, val);
}


int main(int argc, char *argv[]){

    /* Get own ip in uint32 format */
    init_own_ip();

    /* Create file descriptor to write the packet */
    create_log_file();

    /* Descriptor used for sending packets */
    globals.send_sock_fd = get_socket();

    /* Get MAC address of all the three interfaces */
    init_mac_addr();

    /* Build the arp table */
    init_build_arp_cache();

    uint32_t lan_node[] = { char_to_uint32(NODE1_IP),
                            char_to_uint32(NODE2_IP),
                            char_to_uint32(NODE3_IP),
                           };

    /* Building initial routing table */
    init_build_route_table();

    /* Print the arp table */
    print_arp_cache_table_list(lan_node, sizeof(lan_node)/sizeof(lan_node[0]));

    print_route_table();

    start();

    pthread_join(globals.sniff_th, NULL);

    return 0;
}
