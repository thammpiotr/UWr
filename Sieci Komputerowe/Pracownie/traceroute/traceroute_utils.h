// Piotr Thamm 339919

#ifndef TRACEROUTE_UTILS_H
#define TRACEROUTE_UTILS_H
#include <stdbool.h>
#include <netinet/in.h>

// ================ STAŁE ================
#define PACKETS_NUM 3
#define TIMEOUT_MS 1000 // 1000ms
#define TTL_MAX 30
// ================ FUNKCJE POMOCNICZE ================
void display_result(int ttl, struct in_addr router_addrs[], const long response_times[], int response_count);

#endif