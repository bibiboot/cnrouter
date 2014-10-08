#include "packet_sniffer.h"

#define PACKET_LEN 65536

int tcp=0,udp=0,icmp=0,others=0,total=0,i,j;

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

    while(1) {
        saddr_size = sizeof saddr;
        // Receive a packet
        data_size = recvfrom(sock_raw , buffer , PACKET_LEN , 0 , &saddr , (socklen_t*)&saddr_size);
        if(data_size <0 )
        {
            printf("Error: Recvfrom error , failed to get packets\n");
            return ;
        }

        // Filter. Currently allowing all.
        if (!is_allowed(buffer, data_size)) {
            continue;
        }

        if( is_rip_packet(buffer, data_size) ) {
            // RIP Handler
            incoming_packet_handler_rip(buffer, data_size);
            continue;
        }

        /**
         * Handle Time Exceeded ICMP case
         * Handle ping to the user router
         * Handle packet forwarding
         */
        if ( is_ttl_zero(buffer, data_size) ) {
            incoming_packet_handler_ttl_zero(buffer, data_size);
        } else if ( is_packet_reply(buffer, data_size) ){
            incoming_packet_handler_self_icmp(buffer, data_size);
        } else {
            incoming_packet_handler(buffer, data_size);
        }

        /* Track count of the packet type */
        int status = process_packet(buffer , data_size);

        memset(buffer, '\0', PACKET_LEN);
        //fflush(LOGFILE);
        fflush(stdout);
    }

    close(sock_raw);
}

int process_packet(unsigned char* buffer, int size)
{
    struct iphdr *iph = (struct iphdr*)(buffer + sizeof(struct ethhdr));
    ++total;
    switch (iph->protocol)
    {
        case 1:  //ICMP Protocol
            ++icmp;
            break;
        case 6:  //TCP Protocol
            ++tcp;
            break;

        case 17: //UDP Protocol
            ++udp;
            break;

        default: //Some Other Protocol like ARP etc.
            ++others;
            break;
    }
    //printf("TCP : %d   UDP : %d   ICMP : %d   Others : %d   Total : %d\r",
    printf("TCP : %d   UDP : %d   ICMP : %d   Others : %d   Total : %d\n",
            tcp , udp , icmp , others , total);

    return 1;
}
