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


int main() {
    int sock;
    char filename[MSG_LEN];
    char buf[MSG_LEN];
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        perror_and_exit("[-] Error in socket");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SOCK_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (connect(sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
        perror_and_exit("[-] Error in connect");
    printf("[+] Connected to Server.\n");

    printf("[+] Input filename: ");
    fgets(filename, MSG_LEN, stdin);
    filename[strcspn(filename, "\n")] = 0;  // crutch to remove `\n`

    if (send(sock, filename, sizeof(filename), 0) < 0)
        perror_and_exit("[-] Error in send");

    if (recv(sock, buf, sizeof(buf), 0) < 0)
        perror_and_exit("[-] Error in receive");

    printf("[+] Server return to client this file:\n");
    printf("%s\n", buf);

    printf("[+] Connection closed.\n");
    close(sock);

    return 0;
}
