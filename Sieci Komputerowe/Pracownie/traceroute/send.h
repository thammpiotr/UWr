// Piotr Thamm 339919

#ifndef SEND_H
#define SEND_H

#include <netinet/in.h>

u_int16_t compute_icmp_checksum(const void *buff, int length);
int send_icmp_echo(int sockfd, struct sockaddr_in *recipient, int ttl, int i, int pid);
int send_n_icmp_echo(int sockfd, struct sockaddr_in *recipient, int ttl, int pid);

#endif