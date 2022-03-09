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

int convert_int(int a, int p, char *s) {
    // a - число в 10-й системе счисления
    // p - основание системы счисления [2, 30]
    // s - результат
    char letters[30] = {"0123456789ABCDEFGHIJKLMNOPQRST"};

    int num = (int) a;
    int rest = num % p;
    num /= p;

    if (num == 0) {
        s[0] = letters[rest];
        return 1;
    }

    int k = convert_int(num, p, s);
    s[k++] = letters[rest];
    return k;
}

void convert_and_print(char *b) {
    int n = atoi(b);
    char res2[RES_LEN] = {0};
    convert_int(n, 2, res2);

    char res7[RES_LEN] = {0};
    convert_int(n, 7, res7);

    printf("decimal: %d\n", n);
    printf("binary: %s\n", res2);
    printf("octal: %o\n", n);
    printf("hexadecimal: %x\n", n);
    printf("septimal: %s\n\n", res7); // с основанием 7 - номер студента Косаревский по списку группы
}


int main(void) {
    struct sockaddr_in server_addr, client_addr;
    int sock, slen = sizeof(client_addr);
    char buf[MSG_LEN];

    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        perror_and_exit("[-] socket");
    printf("[+] Server started\n");

    memset((char *) &server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SOCK_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1)
        perror_and_exit("[-] bind");

    while (1) {
        socklen_t addr_size;
        addr_size = sizeof(client_addr);
        if (recvfrom(sock, buf, MSG_LEN, 0, (struct sockaddr *) &client_addr, &addr_size) == -1)
            perror_and_exit("[-] recvfrom()");

        printf("[+] Received packet from %s:%d\n\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        convert_and_print(buf);

        printf("[+] Shutdown server? [y/n]: ");
        char buf_l[MSG_LEN];
        fgets(buf_l, MSG_LEN, stdin);
        if (*buf_l == 'y' || *buf_l == 'Y') {
            printf("[+] Server shutdown...\n");
            break;
        }
    }

    close(sock);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define SIZE 1024

void write_file(int sockfd) {
    int n;
    FILE *fp;
    char *filename = "recv.txt";
    char buffer[SIZE];

    fp = fopen(filename, "w");
    while (1) {
        n = recv(sockfd, buffer, SIZE, 0);
        if (n <= 0) {
            break;
            return;
        }
        fprintf(fp, "%s", buffer);
        bzero(buffer, SIZE);
    }
    return;
}

int main() {
    char *ip = "127.0.0.1";
    int port = 8080;
    int e;

    int sockfd, new_sock;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Server socket created successfully.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    e = bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (e < 0) {
        perror("[-]Error in bind");
        exit(1);
    }
    printf("[+]Binding successfull.\n");

    if (listen(sockfd, 10) == 0) {
        printf("[+]Listening....\n");
    } else {
        perror("[-]Error in listening");
        exit(1);
    }

    addr_size = sizeof(new_addr);
    new_sock = accept(sockfd, (struct sockaddr *) &new_addr, &addr_size);
    write_file(new_sock);
    printf("[+]Data written in the file successfully.\n");

    return 0;
}