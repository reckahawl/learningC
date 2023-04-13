#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <mysql/mysql.h>


#define DB_HOST "localhost"
#define DB_USER "username"
#define DB_PASS "password"
#define DB_NAME "database_name"


void connection_handler(int);

int socket_desc, client_sock, c;
struct sockaddr_in server, client;

socket_desc = socket(AF_INET, SOCK_STREAM, 0);

server.sin_family = AF_INET;
server.sin_addr.s_addr = INADDR_ANY;
server.sin_port = htons(8888);

bind(socket_desc, (struct sockaddr *)&server, sizeof(server));

listen(socket_desc, 3);

c = sizeof(struct sockaddr_in);

while ((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c))) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("could not create process");
        return 1;
    }

    if (pid == 0) {
        close(socket_desc);
        connection_handler(client_sock);
        exit(0);
    }

    close(client_sock);
}

void connection_handler(int sock) {
    char client_message[2000];

    // Receive data from client
    recv(sock, client_message, 2000, 0);

    // Extract IP address from client message
    char *ip_address = strtok(client_message, " ");

    // Connect to database
    MYSQL *conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 0, NULL, 0)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    // Insert IP address into database
    char query[1000];
    sprintf(query, "INSERT INTO ip_addresses (ip_address) VALUES ('%s')", ip_address);
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
    }

    // Close database connection and socket
    mysql_close(conn);
    close(sock);
}
