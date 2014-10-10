#include "middleware.h"
#include "packet_send.h"
#include "packet_update.h"
#include "print_packet.h"
#include "route.h"
#include "socket_util.h"
#include "route_table.h"
#include "util.h"
#include <netinet/udp.h>   //Provides declarations for udp header

/**
 * Handle the incoming packet.
 * Either send or append to the queue.
 */
void incoming_packet_handler(unsigned char *packet, int size){

    char result_if_name[IFNAMSIZ] = "eth2";
    send_packet_on_line(result_if_name, packet, size);
}
