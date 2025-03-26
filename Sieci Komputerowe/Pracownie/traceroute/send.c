// Piotr Thamm 339919

#include "send.h"
#include "traceroute_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <sys/time.h>

// =================== Funkcja ze strony wykladu (suma kontrolna nagłówka ICMP) ===================
u_int16_t compute_icmp_checksum(const void *buff, int length)
{
    const u_int16_t* ptr = buff;
    u_int32_t sum = 0;
    assert (length % 2 == 0);
    for (; length > 0; length -= 2)
        sum += *ptr++;
    sum = (sum >> 16U) + (sum & 0xffffU);
    return ~(sum + (sum >> 16U));
}

int send_icmp_echo(int sockfd, struct sockaddr_in *recipient, int ttl, int i, int pid)
{
    // Jezeli ustawienie ttl sie nie powiedzie, konczymy program
    if (setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0) {
        fprintf(stderr, "Setsockopt failed: %s\n", strerror(errno));
        return -1;
    }
    // Konstrukcja komunikatu ICMP do wysłania
    struct icmp header;
    memset(&header, 0, sizeof(header));
    header.icmp_type = ICMP_ECHO;
    header.icmp_code = 0;
    header.icmp_hun.ih_idseq.icd_id = htons(pid);
    // Dla kazdego wysylanego pakietu ustawiamy indywidualny seq, zalezny od ttl, PACKET_NUM i indexu pakietu wyslanego podczas danego ttl
    header.icmp_hun.ih_idseq.icd_seq = htons(ttl * PACKETS_NUM + i);
    header.icmp_cksum = 0;
    header.icmp_cksum = compute_icmp_checksum(&header, sizeof(header));

    ssize_t bytes_sent = sendto(
        sockfd,
        &header,
        sizeof(header),
        0,
        (struct sockaddr*)recipient,
        sizeof(*recipient)
    );

    if (bytes_sent < 0) {
        fprintf(stderr, "Sending packets failed: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

int send_n_icmp_echo(int sockfd, struct sockaddr_in *recipient, int ttl, int pid)
{
    for (int i = 0; i < PACKETS_NUM; i++) {
        if (send_icmp_echo(sockfd, recipient, ttl, i, pid) < 0) {
            return -1;
        }
    }
    return 0;
}