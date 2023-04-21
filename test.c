#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ifaddrs.h>

int main() {
    FILE *fp;
    char filename[] = "file.txt";
    fp = fopen(filename, "w");
    if(fp == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    struct ifaddrs *ifap, *ifa;
    struct sockaddr_in *sa;
    char *addr;

    getifaddrs(&ifap);
    for(ifa = ifap; ifa != NULL; ifa = ifa->ifa_next) {
        if(ifa->ifa_addr->sa_family == AF_INET) {
            sa = (struct sockaddr_in*) ifa->ifa_addr;
            addr = inet_ntoa(sa->sin_addr);
            if(strcmp(addr, "127.0.0.1") != 0) { // Exclude localhost
                fprintf(fp, "%s\n", addr);
            }
        }
    }
    fclose(fp);
    printf("Client IPs written to file.txt\n");
    return 0;
}
