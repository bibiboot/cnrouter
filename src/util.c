#include "util.h"
#include "print_packet.h"

int get_interface_index(char *result_if_name)
{
    int src_index = 0;
    if (strcmp(result_if_name, "eth0") == 0) {
        src_index = 2;
    } else if (strcmp(result_if_name, "eth2") == 0) {
        src_index = 4;
    } else if (strcmp(result_if_name, "eth1") == 0) {
        src_index = 3;
    } else {
        printf("Index: This should never happen\n");
        exit(1);
    }
    return src_index;
}

void pattern_to_human2(uint64_t pattern, uint16_t *dest_mac,
                      uint16_t *src_ip, uint16_t *dest_ip,
                      uint16_t *port)
{
    unsigned char *packet = (unsigned char*)&pattern;
    struct custom_ethernet *ether_header = (struct custom_ethernet*)packet;
    struct custom_ip *ip_header = (struct custom_ip*)(packet + C_ETHLEN);
    struct custom_udp *udp_header = (struct custom_udp*)(packet + C_ETHLEN + C_IPLEN);
    *dest_mac = ether_header->dest_mac;
    *src_ip = ip_header->src_ip;
    *dest_ip = ip_header->dest_ip;
    *port = udp_header->port;
}

void pattern_to_human(uint64_t pattern, uint16_t *dest_mac,
                      uint16_t *src_ip, uint16_t *dest_ip,
                      uint16_t *port)
{
    unsigned char *packet = (unsigned char*)&pattern;
    struct custom_ethernet *ether_header = (struct custom_ethernet*)packet;
    struct custom_ip *ip_header = (struct custom_ip*)(packet + C_ETHLEN);
    struct custom_udp *udp_header = (struct custom_udp*)(packet + C_ETHLEN + C_IPLEN);
    *dest_mac = ntohs(ether_header->dest_mac);
    *src_ip = ntohs(ip_header->src_ip);
    *dest_ip = ntohs(ip_header->dest_ip);
    *port = ntohs(udp_header->port);
}

void set_pattern(unsigned char *packet, uint64_t pttn_num)
{
    struct pattern *pttn = (struct pattern*)packet;
    pttn->value = pttn_num;
}

uint64_t get_pattern(unsigned char *packet)
{
    struct pattern *pttn = (struct pattern*)packet;
    //printf("Pattern = %" PRId64 "\n", be64toh(pttn->value));
    //return be64toh(pttn->value);
    return pttn->value;
}

void create_log_file(){
    LOGFILE = fopen("log.txt","w");
    if (LOGFILE == NULL)
    {
        printf("Unable to create log.txt file.");
    }
}

/**
 * Calculates difference between two timeval.
 */
unsigned int time_diff_micro(struct timeval end, struct timeval start){
  return ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec);
}
