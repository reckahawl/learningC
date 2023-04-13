#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <mysql/mysql.h>

#define DB_HOST "localhost"
#define DB_USER "username"
#define DB_PASS "password"
#define DB_NAME "database_name"

void *connection_handler(void *);

int sock_fd, cli_sock, c, *new_sock;
struct sockaddr_in server, client;

sock_fd = socket(AF_INET, SOCK_STREAM, 0);

server.sin_family = AF_INET;
server.sin_addr.s_addr = INADDR_ANY;
server.sin_port = htons(8888);

bind(sock_fd, (struct sockaddr *)&server, sizeof(server));

listen(sock_fd, 3);

c = sizeof(struct sockaddr_in);

while((cli_sock = accept(sock_fd, (struct sockaddr *)&client, (socklen_t *)&c))){
    pthread_t sniffer_thread;
    new_sock = malloc(1);
    *new_sock = client_sock;

    if(pthread_create(&sniffer_thread, NULL, connection_handler, (void *)new_sock)<0){
        perror("thread creat");
        return 1;
    }
}

void *connection_handler(void *sock_fd){
    int sock = *(int *)sock_fd;
    char client_message[2000];

    //recieve data
    recv(sock, client_message, 2000, 0);

    //extract ip
    char *ip_addr = strtok(client_message, " ");

    //connect to db
    MYSQL *conn = mysql_init(NULL);
    if(!mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME,0, NULL, 0)){
        fprintf(stderr, "%s\n", mysql_error(conn));
        return NULL;
    }

    //insert IP address into database
    char querry[1000];
    sprintf(querry, "INSERT INTO ip_addresses(ip_address) VALUES('%s')", ip_address);
    if(mysql_querry(conn, querry)){
        fprintf(stderr, "%s\n", mysql_error(conn));
    }

    free(sock_fd);
    mysql_close(conn);
    return NULL;

}
