#include "print_packet.h"

#include<netinet/in.h>
#include<errno.h>
#include<netdb.h>
#include<stdio.h> //For standard things
#include<stdlib.h>    //malloc
#include<string.h>    //strlen

#include<netinet/ip_icmp.h>   //Provides declarations for icmp header
#include<netinet/udp.h>   //Provides declarations for udp header
#include<netinet/tcp.h>   //Provides declarations for tcp header
#include<netinet/ip.h>    //Provides declarations for ip header
#include<netinet/if_ether.h>  //For ETH_P_ALL
#include<net/ethernet.h>  //For ether_header
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/ioctl.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>

char *ipaddr_string(u_int32_t *ip){
  struct in_addr sock_addr;
  memset(&sock_addr, 0, sizeof(struct in_addr));
  sock_addr.s_addr = *ip;

  return inet_ntoa(sock_addr);
}

void print_ip(uint32_t ip) {
    struct in_addr in_ip;
    memset(&in_ip, 0, sizeof(struct in_addr));
    in_ip.s_addr = ip;
    printf(" %s ", (unsigned char *)inet_ntoa(in_ip));
}

void print_routed_packet(struct sockaddr_in dest, char *result_if_name,
                         char *src_mac, char *dest_mac){
    printf("dest ip: %s | send interface : %s | dest mac : ",
           (unsigned char *)inet_ntoa(dest.sin_addr),
           result_if_name);
    print_mac(dest_mac);
    printf(" | src mac : ");
    print_mac(src_mac);
    printf("\n");
}

unsigned short csum(unsigned short *buf, int len)
{
        unsigned long sum;
        for(sum=0; len>0; len--)
                sum += *buf++;
        sum = (sum >> 16) + (sum &0xffff);
        sum += (sum >> 16);
        return (unsigned short)(~sum);
}

void print_data (unsigned char* data , int size)
{
    int i;
    for(i=0 ; i < size ; i++)
        fprintf(LOGFILE, "%02hhx ", data[i]);
    fprintf(LOGFILE, "\n");
}

void print_mac(const char *addr)
{
    int i;
    for (i = 0; i < ETH_ALEN - 1; i++)
        printf("%02hhx:", addr[i]);
    printf("%02hhx", addr[ETH_ALEN - 1]);
}

void print_ethernet_header(unsigned char* buffer, int size)
{
    struct ethhdr *eth = (struct ethhdr *)buffer;

    fprintf(LOGFILE , "\n");
    fprintf(LOGFILE , "Ethernet Header\n");
    fprintf(LOGFILE , "   |-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n",
            eth->h_dest[0] , eth->h_dest[1] , eth->h_dest[2] , eth->h_dest[3] ,
            eth->h_dest[4] , eth->h_dest[5] );
    fprintf(LOGFILE , "   |-Source Address      : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n",
            eth->h_source[0] , eth->h_source[1] , eth->h_source[2] ,
            eth->h_source[3] , eth->h_source[4] , eth->h_source[5] );
    fprintf(LOGFILE , "   |-Protocol            : %u \n",(unsigned short)eth->h_proto);
}

void print_data_detail (unsigned char* data , int size)
{
    int i , j;
    for(i=0 ; i < size ; i++)
    {
        if( i!=0 && i%16==0)   //if one line of hex printing is complete...
        {
            fprintf(LOGFILE , "         ");
            for(j=i-16 ; j<i ; j++)
            {
                if(data[j]>=32 && data[j]<=128)
                    fprintf(LOGFILE , "%c",(unsigned char)data[j]); //if its a number or alphabet

                else fprintf(LOGFILE , "."); //otherwise print a dot
            }
            fprintf(LOGFILE , "\n");
        }

        if(i%16==0) fprintf(LOGFILE , "   ");
            fprintf(LOGFILE , " %02X",(unsigned int)data[i]);

        if( i== size-1)  //print the last spaces
        {
            for(j=0;j<15-i%16;j++)
            {
              fprintf(LOGFILE , "   "); //extra spaces
            }

            fprintf(LOGFILE , "         ");

            for(j=i-i%16 ; j<=i ; j++)
            {
                if(data[j]>=32 && data[j]<=128)
                {
                  fprintf(LOGFILE , "%c",(unsigned char)data[j]);
                }
                else
                {
                  fprintf(LOGFILE , ".");
                }
            }

            fprintf(LOGFILE ,  "\n" );
        }
    }
}
