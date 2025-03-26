// Piotr Thamm 339919

#include "send.h"
#include "receive.h"
#include "traceroute_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Invalid Format. Proper form: %s <destination_ip>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    // Sprawdzenie czy gniazdo zostało poprawnie utworzone
    if (sockfd < 0) {
        fprintf(stderr, "Socket creation failed: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    // Adres odbiorcy w strukturze adresowej
    struct sockaddr_in recipient;
    memset(&recipient, 0, sizeof(recipient));
    recipient.sin_family = AF_INET;

    // Sprawdzenie czy podany adres IP jest poprawny
    if (inet_pton(AF_INET, argv[1], &recipient.sin_addr) <= 0) {
        fprintf(stderr, "Invalid IP address: %s\n", strerror(errno));
        close(sockfd);
        return EXIT_FAILURE;
    }

    int pid = getpid();

    // ========================== GLOWNA PETLA ==========================
    for (int ttl = 1; ttl <= TTL_MAX; ttl++) {
        if (send_n_icmp_echo(sockfd, &recipient, ttl, pid) < 0) {
            close(sockfd);
            return EXIT_FAILURE;
        }

        struct in_addr router_addrs[PACKETS_NUM];
        long response_times[PACKETS_NUM];
        bool destination_reached = false;
        int response_count = receive_responses(sockfd, ttl, pid, router_addrs, response_times, &destination_reached);

        display_result(ttl, router_addrs, response_times, response_count);

        // Jeżeli dotarliśmy do adresu docelowego - koniec
        if (destination_reached) {
            break;
        }
    }

    close(sockfd);
    return 0;
}