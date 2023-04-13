#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <pcap/pcap.h>
#include <mysql/mysql.h>

#define DB_HOST "localhost"
#define DB_USER "username"
#define DB_PASS "password"
#define DB_NAME "database_name"

void get_connected_ips(char *ips[]) {
    pcap_if_t *alldevs;
    pcap_if_t *d;
    char errbuf[PCAP_ERRBUF_SIZE];

    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
        return;
    }

    int i = 0;
    for (d = alldevs; d != NULL; d = d->next) {
        pcap_t *handle = pcap_open_live(d->name, BUFSIZ, 1, 1000, errbuf);
        if (handle == NULL) {
            continue;
        }

        struct pcap_pkthdr header;
        const u_char *packet;

        while (1) {
            packet = pcap_next(handle, &header);
            if (packet == NULL) {
                continue;
            }

            struct iphdr *ip_header = (struct iphdr *)(packet + sizeof(struct ether_header));
            char *ip = inet_ntoa(*(struct in_addr *)&ip_header->saddr);

            if (strcmp(ip, "0.0.0.0") == 0 || strcmp(ip, "127.0.0.1") == 0) {
                continue;
            }

            ips[i++] = strdup(ip);
        }

        pcap_close(handle);
    }

    pcap_freealldevs(alldevs);
}


void insert_ips_into_db(char *ips[]) {
    MYSQL *conn = mysql_init(NULL);

    if (!mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 0, NULL, 0)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        mysql_close(conn);
        return;
    }

    char query[1000];
    for (int i = 0; ips[i] != NULL; i++) {
        sprintf(query, "INSERT INTO connected_ips (ip_address) VALUES ('%s')", ips[i]);
        if (mysql_query(conn, query)) {
            fprintf(stderr, "%s\n", mysql_error(conn));
        }
    }

    mysql_close(conn);
}

int main() {
    char *ips[100];

    get_connected_ips(ips);
    insert_ips_into_db(ips);

    for (int i = 0; ips[i] != NULL; i++) {
        free(ips[i]);
    }

    return 0;
}



