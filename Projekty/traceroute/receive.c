// Piotr Thamm 339919

#include "receive.h"
#include "traceroute_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <poll.h>
#include <time.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <stdbool.h>

int receive_responses(int sockfd, int ttl, int pid, struct in_addr router_addrs[], long response_times[], bool *destination_reached)
{
    bool received[PACKETS_NUM] = {false};
    int response_count = 0;

    // Inicjalizacja struktury dla poll (z wykladu)
    struct pollfd ps;
    ps.fd = sockfd;
    ps.events = POLLIN;
    ps.revents = 0;

    // Inicjalizacja rozpoczęcia pomiarów czasu
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    long elapsed_ms = 0;

    // Oczekiwanie na odpowiedzi az otrzymamy wszystkie
    while (response_count < PACKETS_NUM) {
        // Obliczenie uplynietego czasu
        clock_gettime(CLOCK_MONOTONIC, &end_time);
        elapsed_ms = (end_time.tv_sec - start_time.tv_sec)*1000 + (end_time.tv_nsec - start_time.tv_nsec)/1000000;

        // Jezeli uplynelo wiecej czasu niz TIMEOUT_MS, to przerywamy
        if (elapsed_ms >= TIMEOUT_MS) {
            break;
        }

        int timeout_ms = TIMEOUT_MS - elapsed_ms;

        // Czekanie maksymalnie timeout_ms milisekund na pakiet w gnieździe sockfd
        int poll_result = poll(&ps, 1, timeout_ms);

        if (poll_result == 0) { // Nastapil timeout
            break;
        } if (poll_result < 0) {
            fprintf(stderr, "Poll error: %s\n", strerror(errno));
            break;
        }

        // Jezeli sa dane do odczytania
        if (ps.revents == POLLIN) {
            struct sockaddr_in sender;
            socklen_t sender_len = sizeof(sender);
            u_int8_t buffer[IP_MAXPACKET];

            // Odbior pakietu
            ssize_t packet_len = recvfrom(
                sockfd,
                buffer,
                IP_MAXPACKET,
                MSG_DONTWAIT,
                (struct sockaddr*)&sender, &sender_len
            );

            if (packet_len < 0) {
                // EWOULDBLOCK wystepuje gdy gniazdo nie ma danych do odczytu
                if (errno != EWOULDBLOCK) {
                    fprintf(stderr, "Recvfrom error: %s\n", strerror(errno));
                }
                continue;
            }

            struct ip *ip_header = (struct ip *)buffer;
            ssize_t ip_header_len = 4 * ip_header->ip_hl;
            struct icmp *icmp_header = (struct icmp *)(buffer + ip_header_len);

            int packet_idx = -1;

            // Obsługa odpowiedzi od adresu docelowego
            if (icmp_header->icmp_type == ICMP_ECHOREPLY) {
                // Sprawdzenie czy to odpowiedź na wyslany pakiet
                if (ntohs(icmp_header->icmp_hun.ih_idseq.icd_id) == pid) {
                    int seq = ntohs(icmp_header->icmp_hun.ih_idseq.icd_seq);
                    // Sprawdzamy czy seq jest pomiedzy gorna i dolna granica przedzialu numerow sekwencyjnych dla danego TTL
                    if (seq >= ttl * PACKETS_NUM && seq < (ttl + 1) * PACKETS_NUM) {
                        packet_idx = seq - ttl * PACKETS_NUM;
                        // Jeżeli typ to ICMP_ECHOREPLY to oznacza, że dostaliśmy odpowiedź od adresu docelowego
                        *destination_reached = true;
                    }
                }
            }
            // Obsługa odpowidzi od routerów pośrednich
            else if (icmp_header->icmp_type == ICMP_TIME_EXCEEDED) {
                /*
                * Wyodrębnianie oryginalnego pakietu ICMP z odpowiedzi TIME_EXCEEDED:
                * 1. orig_ip - przejście do nagłówka IP oryginalnego pakietu
                *    (pomijamy nagłówek IP odpowiedzi i 8-bajtowy nagłówek ICMP TIME EXCEEDED)
                * 2. orig_ip_len - długośc oryginalnego nagłówka IP
                * 3. orig_icmp - przejście do oryginalnego nagłówka ICMP w pakiecie Echo Request
                */
                struct ip *orig_ip = (struct ip *)(buffer + ip_header_len + 8);
                ssize_t orig_ip_len = 4 * orig_ip->ip_hl;
                struct icmp *orig_icmp = (struct icmp *)((u_int8_t *)orig_ip + orig_ip_len);

                // Sprawdzenie czy to odpowiedź na wyslany pakiet
                if (ntohs(orig_icmp->icmp_hun.ih_idseq.icd_id) == pid) {
                    int seq = ntohs(orig_icmp->icmp_hun.ih_idseq.icd_seq);
                    // Sprawdzamy czy seq jest pomiedzy gorna i dolna granica przedzialu numerow sekwencyjnych dla danego TTL
                    if (seq >= ttl * PACKETS_NUM && seq < (ttl + 1) * PACKETS_NUM) {
                        packet_idx = seq - ttl * PACKETS_NUM;
                    }
                }
            }
            // Jezeli znalezliono odpowiedni pakiet, zapisuje informacje
            if (packet_idx >= 0 && packet_idx < PACKETS_NUM && !received[packet_idx]) {
                clock_gettime(CLOCK_MONOTONIC, &end_time);
                long response_time = (end_time.tv_sec - start_time.tv_sec)*1000 + (end_time.tv_nsec - start_time.tv_nsec)/1000000;
                received[packet_idx] = true;
                router_addrs[packet_idx] = sender.sin_addr;
                response_times[packet_idx] = response_time;
                response_count++;
            }
        }
    }

    return response_count;
}