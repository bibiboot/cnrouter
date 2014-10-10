#include "packet_sniffer.h"

#include <net/if.h>

#define PACKET_LEN 65536

void print_header(unsigned char *packet, int size)
{
    printf(KCYN "SNIFF: " RESET);
    struct data *dhead = (struct data*) packet;
    printf("Dest Mac : %d, Src Mac : %d\n",
            ntohs(dhead->dest_mac),
            ntohs(dhead->src_mac));

}

void print_non_human_read_payload(unsigned char *packet, int size)
{
    printf(KCYN "SNIFF: " RESET);
    int header_size = sizeof(struct data);

    printf("Total [%d]: Header size [%d]\n",
            size, header_size,size - header_size);
}

void print_human_read_payload(unsigned char *packet, int size)
{
    printf(KCYN "SNIFF: " RESET);
    int header_size = sizeof(struct data);
    unsigned char *payload = packet + header_size;

    printf("Total [%d]: Header size [%d]: Payload [%d]: %s\n",
            size, header_size,size - header_size, payload);
}

int process_custom_packet(unsigned char* buffer, int size)
{
    struct data *dhead = (struct data*) buffer;

    if (ntohs(dhead->dest_mac) == 1) {
        print_data_detail(buffer, size);
        print_header(buffer, size);
        print_human_read_payload(buffer, size);
        fflush(LOGFILE);
        return 0;
    }
    return 1;
}

int set_promisc(char *interface, int sock ) {
    struct ifreq ifr;
    strncpy(ifr.ifr_name, interface,strlen(interface)+1);
    if((ioctl(sock, SIOCGIFFLAGS, &ifr) == -1)) {
        /*Could not retrieve flags for the
        * interface*/
        perror("Could not retrive flags for the interface");
        exit(0);
    }
    printf("DEBUG: The interface is ::: %s\n", interface);
    perror("DEBUG: Retrieved flags from interface successfully");

    /*now that the flags have been
    * retrieved*/
    /* set the flags to PROMISC */
    ifr.ifr_flags |= IFF_PROMISC;
    if (ioctl (sock, SIOCSIFFLAGS, &ifr) == -1 ) {
        perror("Could not set the PROMISC flag:");
        exit(0);
    }
    printf("DEBUG: Setting interface ::: %s ::: to promisc\n", interface);
    return(0);
}


void* sniff(void *val)
{
    int saddr_size , data_size;
    struct sockaddr saddr;
    unsigned char *buffer = (unsigned char *) malloc(PACKET_LEN);
    memset(buffer, '\0', PACKET_LEN);

    printf("Starting...\n");

    int sock_raw = socket( AF_PACKET , SOCK_RAW , htons(ETH_P_ALL)) ;

    if (sock_raw < 0) {
        perror("Socket Error");
        return;
    }

    /**
     * Set it on promiscous mode,
     * Otherwise it won't sniff packet
     * Which do not belong to him
     */
    set_promisc(INF0, sock_raw);
    set_promisc(INF1, sock_raw);
    set_promisc(INF2, sock_raw);

    while(1) {
        saddr_size = sizeof saddr;
        // Receive a packet
        data_size = recvfrom(sock_raw , buffer , PACKET_LEN , 0 , &saddr , (socklen_t*)&saddr_size);
        if(data_size <0 )
        {
            printf("Error: Recvfrom error , failed to get packets\n");
            return ;
        }

        /* Track count of the packet type */
        int status = process_custom_packet(buffer , data_size);

        if (status != 0)
            continue;

        incoming_packet_handler(buffer, data_size);

        memset(buffer, '\0', PACKET_LEN);

        fflush(LOGFILE);
        fflush(stdout);
        exit(1);
    }

    close(sock_raw);
}
