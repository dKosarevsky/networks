#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#include "settings.h"

void perror_and_exit(char *s) {
    perror(s);
    exit(1);
}

int main(void) {
    struct sockaddr_in server_addr;
    int sock, slen = sizeof(server_addr);
    char buf[MSG_LEN];

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0)
        perror_and_exit("[-] socket");

    memset((char *) &server_addr, 0, sizeof(server_addr)); // заполняем нулями

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SOCK_PORT); // преобразует в сетевой порядок расположения байтов

    if (inet_aton(SRV_IP, &server_addr.sin_addr) == 0) {
        fprintf(stderr, "[-] inet_aton() failed\n");
        exit(1);
    }

    printf("[+] Input decimal number: ");
    fgets(buf, MSG_LEN, stdin);

    if (sendto(sock, buf, MSG_LEN, 0, (struct sockaddr *) &server_addr, slen) == -1)
        perror_and_exit("[-] sendto()");

    close(sock);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define SIZE 1024

void send_file(FILE *fp, int sockfd) {
    int n;
    char data[SIZE] = {0};

    while (fgets(data, SIZE, fp) != NULL) {
        if (send(sockfd, data, sizeof(data), 0) == -1) {
            perror("[-]Error in sending file.");
            exit(1);
        }
        bzero(data, SIZE);
    }
}

int main() {
    char *ip = "127.0.0.1";
    int port = 8080;
    int e;

    int sockfd;
    struct sockaddr_in server_addr;
    FILE *fp;
    char *filename = "send.txt";

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Server socket created successfully.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    e = connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (e == -1) {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Connected to Server.\n");

    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("[-]Error in reading file.");
        exit(1);
    }

    send_file(fp, sockfd);
    printf("[+]File data sent successfully.\n");

    printf("[+]Closing the connection.\n");
    close(sockfd);

    return 0;
}