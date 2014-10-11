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

    /* Building initial routing table */
    init_build_route_table();

    print_route_table();
    print_route_table_human();

    start();

    pthread_join(globals.sniff_th, NULL);

    return 0;
}
