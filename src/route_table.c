#include "route_table.h"
#include "cprotocol.h"
#include "util.h"

void get_route_entry_print(uint64_t given_pattern, uint64_t *dest_pattern,
                           char *interface)
{
    router_entry *rentry = (router_entry*)find_entry(given_pattern);
    if (rentry == NULL) {
        printf("ROUTER: This should never happen\n");
        exit(1);
    }

    *dest_pattern = rentry->dest_pattern;
    strcpy(interface, rentry->interface);
}

void get_route_entry_switch(uint64_t given_pattern, uint64_t *dest_pattern,
                            char *interface)
{
    // printf("Searcing for = %" PRId64 "\n", given_pattern);
    switch (given_pattern) {
        case R_0_1_2_0 :
            // Node1 to Node2 DATA
            *dest_pattern = R_2_1_2_0;
            strcpy(interface, INF2);
            break;
        case R_0_2_1_0 :
            // Node2 to Node1 DATA
            *dest_pattern = R_1_2_1_0;
            strcpy(interface, INF0);
            break;
        case R_0_1_3_0 :
            // Node1 to Node3 DATA
            *dest_pattern = R_3_1_3_0;
            strcpy(interface, INF1);
            break;
        case R_0_3_1_0 :
            // Node3 to Node1 DATA
            *dest_pattern = R_1_3_1_0;
            strcpy(interface, INF0);
            break;
        case R_0_2_3_0 :
            // Node2 to Node3 DATA
            *dest_pattern = R_3_2_3_0;
            strcpy(interface, INF1);
            break;
        case R_0_3_2_0 :
            // Node3 to Node2 DATA
            *dest_pattern = R_2_3_2_0;
            strcpy(interface, INF2);
            break;
        case R_0_1_2_1 :
            // Node1 to Node2 NACK
            *dest_pattern = R_2_1_2_1;
            strcpy(interface, INF2);
            break;
        case R_0_2_1_1 :
            // Node2 to Node1 NACK
            *dest_pattern = R_1_2_1_1;
            strcpy(interface, INF0);
            break;
        case R_0_1_3_1 :
            // Node1 to Node3 NACK
            *dest_pattern = R_3_1_3_1;
            strcpy(interface, INF1);
            break;
        case R_0_3_1_1 :
            // Node3 to Node1 NACK
            *dest_pattern = R_1_3_1_1;
            strcpy(interface, INF0);
            break;
        case R_0_2_3_1 :
            // Node2 to Node3 NACK
            *dest_pattern = R_3_2_3_1;
            strcpy(interface, INF1);
            break;
        case R_0_3_2_1 :
            // Node3 to Node2 NACK
            *dest_pattern = R_2_3_2_1;
            strcpy(interface, INF2);
            break;
        default :
            printf("Error: No route found\n");
            exit(1);
    }
}

void get_route_entry(uint64_t given_pattern, uint64_t *dest_pattern,
                     char *interface) {

    router_entry *rentry = (router_entry*)find_entry(given_pattern);
    if (rentry == NULL) {
        printf("ROUTER: This should never happen\n");
        exit(1);
    }

    *dest_pattern = rentry->dest_pattern;
    strcpy(interface, rentry->interface);
}

/**
 * Print the complete routing table
 *
 * ++++++++++++++++++++++++++++++++++++++++++++++
 * | Given pattern  | Dest pattern |  Interface |
 * ++++++++++++++++++++++++++++++++++++++++++++++
 * ++++++++++++++++++++++++++++++++++++++++++++++
 */
void print_route_table() {
    char res_interface[100];
    uint64_t dest_pattern;

    int i;
    printf(KRED "Route table");
    printf(KGRN"\n++++++++++++++++++++++++++++++++++++++++++++++\n");
    for (i = 0; i < globals.rtable_size; i++) {
        if (i%2 == 0)
            printf(KBLU);
        else
            printf(KGRN);
        memset(res_interface, 0, 100);
        uint64_t given_pattern = globals.rtable_keys[i];
        //get_route_entry(given_pattern, &dest_pattern, res_interface);
        get_route_entry_switch(given_pattern, &dest_pattern, res_interface);

        //printf("Pattern = %" PRId64 "\n", be64toh(pttn->value));
        //printf("| %15ld  | %15ld | %s |", given_pattern, dest_pattern, res_interface);
        printf("| %15ld  | %15ld | %s |", be64toh(given_pattern), be64toh(dest_pattern), res_interface);
        printf("\n++++++++++++++++++++++++++++++++++++++++++++++\n");
    }
    printf(RESET);
}

void print_route_table_human() {
    char res_interface[100];
    uint64_t dest_pattern;
    uint16_t dest_mac, src_ip, dest_ip, port;

    int i;
    printf(KRED "Human Route table");
    printf(KGRN "\n++++++++++++++++++++++++++++++++++++++++++++++\n");
    for (i = 0; i < globals.rtable_size; i++) {
        if (i%2 == 0)
            printf(KBLU);
        else
            printf(KGRN);
        memset(res_interface, 0, 100);
        uint64_t given_pattern = globals.rtable_keys[i];
        //get_route_entry(given_pattern, &dest_pattern, res_interface);
        get_route_entry_switch(given_pattern, &dest_pattern, res_interface);
        pattern_to_human(given_pattern, &dest_mac, &src_ip, &dest_ip, &port);
        //pattern_to_human2(given_pattern, &dest_mac, &src_ip, &dest_ip, &port);
        printf("| %d  | %d | %d | %d |||", dest_mac, src_ip, dest_ip, port);
        pattern_to_human(dest_pattern, &dest_mac, &src_ip, &dest_ip, &port);
        //pattern_to_human2(dest_pattern, &dest_mac, &src_ip, &dest_ip, &port);
        printf("| %d  | %d | %d | %d | %s", dest_mac, src_ip, dest_ip, port, res_interface);

        printf("\n++++++++++++++++++++++++++++++++++++++++++++++\n");
    }
    printf(RESET);
}

void add_route_entry(uint64_t given_pattern, uint64_t dest_pattern,
               char *interface) {

    /* Create entry */
    router_entry *r_node = malloc(sizeof(router_entry));
    r_node->given_pattern = given_pattern;
    r_node->dest_pattern = dest_pattern;
    strcpy(r_node->interface, interface);
    /* Add entry */
    add_entry(r_node);

    globals.rtable_keys[globals.rtable_size++] = given_pattern;
}

/**
 */
void init_build_route_table(){
    add_route_entry(htobe64(4295098368), htobe64(562954248519680), INF2);
    add_route_entry(htobe64(8590000128), htobe64(281483566710784), INF0);
}
