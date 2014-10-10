#include "packet_sniffer.h"
#include "route_table.h"
#include "socket_util.h"
#include "util.h"
#include "globals.h"

void start(){
    void *val;
    pthread_create(&globals.sniff_th, 0, sniff, val);
}


int main(int argc, char *argv[]){

    /* Create file descriptor to write the packet */
    create_log_file();

    /* Descriptor used for sending packets */
    globals.send_sock_fd = get_socket();

    /* Build the arp table */
    //init_build_arp_cache();

    /*
    uint32_t lan_node[] = { char_to_uint32(NODE1_IP),
                            char_to_uint32(NODE2_IP),
                            char_to_uint32(NODE3_IP),
                           };
    */

    /* Building initial routing table */
    //init_build_route_table();

    /* Print the arp table */
    //print_arp_cache_table_list(lan_node, sizeof(lan_node)/sizeof(lan_node[0]));

    //print_route_table();

    start();

    pthread_join(globals.sniff_th, NULL);

    return 0;
}
