#pragma once

#include "globals.h"
#include <linux/if_arp.h>

struct sockaddr_ll socket_address_eth0;
struct sockaddr_ll socket_address_eth2;

void send_packet_on_line(char *if_name, void *packet,
                         int data_size);
