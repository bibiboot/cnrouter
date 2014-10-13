#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <string.h>
#include <linux/if_ether.h>
#include<netinet/ip.h>    //Provides declarations for ip header
#include <pthread.h>

#include "uthash.h"
#include "config.h"
#include "color.h"
#include "cprotocol.h"

#define DEBUG 0

#define PRINT_ROUTE_DEBUG 0

#define PACKET_SIZE 65536
// Print function name, filename and line number in print
#define DBG(...) do{fprintf(stderr, "%s (%s, line %d): ", __func__, __FILE__, __LINE__); \
                         fprintf(stderr, __VA_ARGS__);           \
                         fprintf(stderr, "\n");} while(0)
#define INF0 "eth0"
#define INF1 "eth1"
#define INF2 "eth2"
#define ETH0_IP "10.10.1.1"
#define ETH1_IP "10.10.3.1"
#define ETH2_IP "10.10.2.1"

#define MULTICAST_IP "224.0.0.9"

#define EXTRACT_16BITS(p) \
	((u_int16_t)((u_int16_t)*((const u_int8_t *)(p) + 0) << 8 | \
		     (u_int16_t)*((const u_int8_t *)(p) + 1)))

#define EXTRACT_32BITS(p) \
	((u_int32_t)((u_int32_t)*((const u_int8_t *)(p) + 0) << 24 | \
		     (u_int32_t)*((const u_int8_t *)(p) + 1) << 16 | \
		     (u_int32_t)*((const u_int8_t *)(p) + 2) << 8 | \
		     (u_int32_t)*((const u_int8_t *)(p) + 3)))


FILE *LOGFILE;

// Hashmap data structure
typedef struct hashl {
    uint64_t given_pattern;
    uint64_t dest_pattern;
    char interface[100];
    UT_hash_handle hh;
} router_entry;

struct globals {
    struct config config;
    // Hashmap
    router_entry *rentry;
    int send_sock_fd;
    pthread_t sniff_th;
    int rtable_size;
    uint64_t rtable_keys[100];
    int total_packet_sniff;
};

extern struct globals globals;
