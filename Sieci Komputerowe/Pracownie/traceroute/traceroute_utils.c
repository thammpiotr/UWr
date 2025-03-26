// Piotr Thamm 339919

#include "traceroute_utils.h"
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

void display_result(int ttl, struct in_addr router_addrs[], const long response_times[], int response_count)
{
    // Dodawanie spacji dla cyfr podczas numeracji wierszy
    if (ttl < 10) printf(" ");
    printf("%d. ", ttl);

    if (response_count == 0) {
        printf("*\n");
        return;
    }

    // Konwersja adresów IP na adresy tekstowe
    char ip_str[PACKETS_NUM][INET_ADDRSTRLEN];
    for (int i = 0; i < response_count; i++) {
        inet_ntop(AF_INET, &router_addrs[i], ip_str[i], sizeof(ip_str[0]));
    }
    // Wyswietlanie pierwszego adresu
    printf("%s", ip_str[0]);

    // Wyświetlanie pozostałych unikalnych adresów jeżeli dostalismy odpowiedz od wiecej niz jednego routera
    for (int i = 1; i < response_count; i++) {
        bool is_unique = true;

        for (int j = 0; j < i; j++) {
            if (strcmp(ip_str[i], ip_str[j]) == 0) {
                is_unique = false;
                break;
            }
        }
        if (is_unique) {
            printf(" %s", ip_str[i]);
        }
    }

    // Jezeli nie otrzymalismy odpowiedzi na wszystkie wyslane pakiety, nie liczymy sredniej, zapisujemy "???"
    if (response_count < PACKETS_NUM) {
        printf(" ???\n");
    }

    // W p.p. liczymy srednia czasow odpowiedzi dla kazdego wyslanego pakietu
    else {
        long total_time = 0;
        for (int i = 0; i < response_count; i++) {
            total_time += response_times[i];
        }

        long avg_time = total_time / response_count;
        printf(" %ldms\n", avg_time);
    }
}