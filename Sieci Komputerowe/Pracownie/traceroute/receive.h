// Piotr Thamm 339919

#ifndef RECEIVE_H
#define RECEIVE_H

#include <stdbool.h>
#include <netinet/in.h>

int receive_responses(int sockfd, int ttl, int pid, struct in_addr router_addrs[], long response_times[], bool *destination_reached);

#endif