#pragma once

#include "globals.h"
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/if_arp.h>

void send_ethernet_packet();

void send_packet_on_line(char *if_name, void *packet,
                         int data_size);
