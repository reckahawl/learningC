#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <sys/time.h>

#define PACKET_SIZE 64
#define MAX_WAIT_TIME 5

unsigned short checksum(unsigned short *ptr, int nbytes);

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <hostname or IP address>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *hostname = argv[1];
    struct hostent *host = gethostbyname(hostname);

    if (!host) {
        printf("Could not resolve hostname %s\n", hostname);
        exit(EXIT_FAILURE);
    }

    struct in_addr **addr_list = (struct in_addr **)host->h_addr_list;

    // Create a raw socket for sending ICMP packets
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set the TTL (Time To Live) field of the IP header to 64
    int ttl = 64;
    if (setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Fill in the ICMP echo request packet
    char packet[PACKET_SIZE];
    memset(packet, 0, PACKET_SIZE);

    struct icmp *icmp_hdr = (struct icmp *)packet;
    icmp_hdr->icmp_type = ICMP_ECHO;
    icmp_hdr->icmp_code = 0;
    icmp_hdr->icmp_id = getpid();
    icmp_hdr->icmp_seq = 1;
    icmp_hdr->icmp_cksum = 0;
    icmp_hdr->icmp_cksum = checksum((unsigned short *)icmp_hdr, PACKET_SIZE);

    // Send the ICMP echo request packet
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr = *addr_list[0];

    if (sendto(sockfd, packet, PACKET_SIZE, 0, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("sendto");
        exit(EXIT_FAILURE);
    }

    // Wait for the ICMP echo reply packet
    char reply[PACKET_SIZE];
    memset(reply, 0, PACKET_SIZE);

    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);

    struct timeval tv;
    tv.tv_sec = MAX_WAIT_TIME;
    tv.tv_usec = 0;

    int select_result = select(sockfd + 1, &readfds, NULL, NULL, &tv);

    if (select_result < 0) {
        perror("select");
        exit(EXIT_FAILURE);
    } else if (select_result == 0) {
        printf("Request timed out.\n");
        exit(EXIT_SUCCESS);
    } else {
        socklen_t len = sizeof(addr);
        if (recvfrom(sockfd, reply, PACKET_SIZE, 0, (struct sockaddr *)&addr, &len) < 0) {
            perror("recvfrom");
           
