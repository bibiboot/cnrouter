#include "globals.h"

void init_build_route_table();

void print_route_table_human();

void print_route_table();

void print_switch_route_table();

void print_switch_route_table_human();

void get_route_entry(uint64_t given_pattern,
                     uint64_t *dest_pattern,
                     char *interface);

void get_route_entry_switch(uint64_t given_pattern,
                            uint64_t *dest_pattern,
                            char *interface);
