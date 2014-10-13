#include "packet_sniffer.h"
#include "route_table.h"
#include "socket_util.h"
#include "util.h"
#include "globals.h"
#include "packet_send.h"

void start(){
    void *val;
    pthread_create(&globals.sniff_th, 0, sniff, val);
}


int main(int argc, char *argv[]){

    /* Create file descriptor to write the packet */
    create_log_file();

    /* Descriptor used for sending packets */
    globals.send_sock_fd = get_socket();

    unsigned char dest_mac[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x05};
    create_custom_socket_address(&socket_address_eth0,
                                 2, dest_mac);
    create_custom_socket_address(&socket_address_eth2,
                                 4, dest_mac);

    /* Building initial routing table */
    init_build_route_table();

    print_route_table();
    print_route_table_human();

    start();

    pthread_join(globals.sniff_th, NULL);

    return 0;
}
