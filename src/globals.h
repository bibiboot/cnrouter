#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <string.h>
#include <linux/if_ether.h>
#include<netinet/ip.h>
#include <pthread.h>

#include "uthash.h"
#include "config.h"
#include "color.h"
#include "cprotocol.h"

#define DEBUG 0

#define PACKET_SIZE 65536
#define PACKET_LEN 65536

#define INF0 "eth0"
#define INF1 "eth1"
#define INF2 "eth2"

// Node1 to Node2 DATA
//#define R_0_1_2_0 4295098368
//#define R_2_1_2_0 562954248519680
#define R_0_1_2_0 2199040032768
#define R_2_1_2_0 2199040033280

// Node2 to Node1 DATA
//#define R_0_2_1_0 8590000128
//#define R_1_2_1_0 281483566710784
#define R_0_2_1_0 1099545182208
#define R_1_2_1_0 1099545182464

// Node1 to Node3 DATA
//#define R_0_1_3_0 4295163904
//#define R_3_1_3_0 844429225295872
#define R_0_1_3_0 3298551660544
#define R_3_1_3_0 3298551661312

// Node3 to Node1 DATA
//#define R_0_3_1_0 12884967424
//#define R_1_3_1_0 281487861678080
#define R_0_3_1_0 1099561959424
#define R_1_3_1_0 1099561959680

// Node2 to Node3 DATA
//#define R_0_2_3_0 8590131200
//#define R_3_2_3_0 844433520263168
#define R_0_2_3_0 3298568437760
#define R_3_2_3_0 3298568438528

// Node3 to Node2 DATA
//#define R_0_3_2_0 12885032960
//#define R_2_3_2_0 562962838454272
#define R_0_3_2_0 2199073587200
#define R_2_3_2_0 2199073587712

// Node1 to Node2 NACK
//#define R_0_1_2_1 4295098369
//#define R_2_1_2_1 562954248519681
#define R_0_1_2_1 72059793077960704
#define R_2_1_2_1 72059793077961216

// Node2 to Node1 NACK
//#define R_0_2_1_1 8590000129
//#define R_1_2_1_1 281483566710785
#define R_0_2_1_1 72058693583110144
#define R_1_2_1_1 72058693583110400

// Node1 to Node3 NACK
//#define R_0_1_3_1 4295163905
//#define R_3_1_3_1 844429225295873
#define R_0_1_3_1 72060892589588480
#define R_3_1_3_1 72060892589589248

// Node3 to Node1 NACK
//#define R_0_3_1_1 12884967425
//#define R_1_3_1_1 281487861678081
#define R_0_3_1_1 72058693599887360
#define R_1_3_1_1 72058693599887616

// Node2 to Node3 NACK
//#define R_0_2_3_1 8590131201
//#define R_3_2_3_1 844433520263169
#define R_0_2_3_1 72060892606365696
#define R_3_2_3_1 72060892606366464

// Node3 to Node2 NACK
//#define R_0_3_2_1 12885032961
//#define R_2_3_2_1 562962838454273
#define R_0_3_2_1 72059793111515136
#define R_2_3_2_1 72059793111515648

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
