#include "packet_send.h"

/**
 * Send packet actually
 * Creates the socket address
 */
void send_packet_on_line(char *if_name, void *packet, int data_size){
    struct ifreq ifr;
    /*target address*/
    struct sockaddr_ll socket_address;

    //Find src interface index
    int src_index = interface_index(globals.send_sock_fd, if_name);
    printf("Index = %d\n", src_index);

    /*prepare sockaddr_ll*/
    unsigned char dest_mac[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x05};
    create_custom_socket_address(&socket_address, src_index, dest_mac);

    /*send the packet*/
    int send_result = 0;
    send_result = sendto(globals.send_sock_fd, packet,
                         data_size , 0, (struct sockaddr*)&socket_address,
                         sizeof(socket_address));
    if (send_result < 0){
	printf("ERROR: sendto\n");
	perror("sendto");
	exit(1);
    }
}

void send_packet_on_line_fast(int index, void *packet, int data_size){
    int send_result = 0;
    if ( index == 2 ) {
        send_result = sendto(globals.send_sock_fd, packet,
                             data_size , 0, (struct sockaddr*)&socket_address_eth0,
                             sizeof(socket_address_eth0));
    } else if ( index == 4 ) {
        send_result = sendto(globals.send_sock_fd, packet,
                             data_size , 0, (struct sockaddr*)&socket_address_eth2,
                             sizeof(socket_address_eth2));
    }
    else if ( index == 3 ) {
        send_result = sendto(globals.send_sock_fd, packet,
                             data_size , 0, (struct sockaddr*)&socket_address_eth1,
                             sizeof(socket_address_eth1));
    }
    /*send the packet*/
    if (send_result < 0){
	printf("ERROR: sendto\n");
	perror("sendto");
	exit(1);
    }
}
