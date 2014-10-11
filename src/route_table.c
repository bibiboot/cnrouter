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
    printf(KGRN "Route table");
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++\n");
    for (i = 0; i < globals.rtable_size; i++) {
        memset(res_interface, 0, 100);
        uint64_t given_pattern = globals.rtable_keys[i];
        get_route_entry(given_pattern, &dest_pattern, res_interface);

        //printf("Pattern = %" PRId64 "\n", be64toh(pttn->value));
        printf("| %15ld  | %15ld | %s |", given_pattern, dest_pattern, res_interface);
        printf("\n++++++++++++++++++++++++++++++++++++++++++++++\n");
    }
    printf(RESET);
}

void print_route_table_human() {
    char res_interface[100];
    uint64_t dest_pattern;
    uint16_t dest_mac, src_ip, dest_ip, port;

    int i;
    printf(KGRN "Human Route table");
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++\n");
    for (i = 0; i < globals.rtable_size; i++) {
        memset(res_interface, 0, 100);
        uint64_t given_pattern = globals.rtable_keys[i];
        get_route_entry(given_pattern, &dest_pattern, res_interface);
        pattern_to_human(given_pattern, &dest_mac, &src_ip, &dest_ip, &port);
        printf("| %d  | %d | %d | %d |||", dest_mac, src_ip, dest_ip, port);
        pattern_to_human(dest_pattern, &dest_mac, &src_ip, &dest_ip, &port);
        printf("| %d  | %d | %d | %d | %s", dest_mac, src_ip, dest_ip, port, res_interface);

        //printf("| %" PRId64 "  | %" PRId64 " | %s |", given_pattern, dest_pattern, res_interface);
        //printf("| %15ld  | %15ld | %s |", given_pattern, dest_pattern, res_interface);
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
}
