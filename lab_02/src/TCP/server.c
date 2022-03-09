#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "settings.h"

void perror_and_exit(char *s) {
    perror(s);
    exit(1);
}

void print_log(char *filename) {
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    printf("[+] Server log: %s [%d:%d:%d %d.%d.%d]\n", filename,
           timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec,
           timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);
}

void send_file(FILE *fp, int sock) {
    char data[SIZE] = {0};

    while (fgets(data, SIZE, fp) != NULL) {
        if (send(sock, data, sizeof(data), 0) == -1) {
            perror_and_exit("[-] Error in sending file.");
        }
        bzero(data, SIZE);
    }
}

int main() {
    int sock, listener;
    struct sockaddr_in server_addr, client_addr;
    char filename[MSG_LEN];
    int bytes_read;

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0)
        perror_and_exit("[-] Error in socket");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SOCK_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(listener, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
        perror_and_exit("[-] Error in bind");

    listen(listener, 1);
    printf("[+] Server is listening...\n");

    while (1) {
        socklen_t cli_addr_size = sizeof(client_addr);
        sock = accept(listener, (struct sockaddr *) &client_addr, &cli_addr_size);
        if (sock < 0)
            perror_and_exit("[-] Error in accept");

        printf("[+] Received packet from %s:%d\n\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        while (1) {
            bytes_read = recv(sock, filename, MSG_LEN, 0);
            if (bytes_read <= 0)
                break;

            printf("[+] Requested file: %s \n", filename);
            FILE *file = fopen(filename, "r");
            if (file == NULL) {
                send(sock, "[-] Error in reading file.", bytes_read, 0);
                perror_and_exit("[-] Error in reading file.");
            }

            send_file(file, sock);
            printf("[+] File data sent successfully.\n");

            print_log(filename);
        }

        close(sock);

        printf("[+] Shutdown server? [y/n]: ");
        char buf_l[MSG_LEN];
        fgets(buf_l, MSG_LEN, stdin);
        if (*buf_l == 'y' || *buf_l == 'Y') {
            printf("[+] Server shutdown...\n");
            break;
        }
    }

    return 0;
}
