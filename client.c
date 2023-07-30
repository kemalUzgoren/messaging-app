#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORTNO	5061
#define HOSTNAME	"127.0.0.1"

void ExitSys(const char* errorMsg, int status);

int main(void)
{
    int result;
    struct sockaddr_in sinClient;
    int clientSock;
    char buf[1024];

    if ((clientSock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        ExitSys("socket", EXIT_FAILURE);

    memset(&sinClient, 0, sizeof(sinClient));
    sinClient.sin_family = AF_INET;
    sinClient.sin_port = htons(PORTNO);
    if (inet_pton(AF_INET, HOSTNAME, &sinClient.sin_addr) <= 0) {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    if (connect(clientSock, (struct sockaddr*)&sinClient, sizeof(sinClient)) == -1)
        ExitSys("connect", EXIT_FAILURE);

    printf("connected\n");

    for (;;) {
        // Send data to the server
        printf("Text:");
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\n")] = '\0';

        if (send(clientSock, buf, strlen(buf), 0) == -1)
            ExitSys("send", EXIT_FAILURE);
        if (strcmp(buf, "quit") == 0)
            break;

        // Receive response from the server
        if ((result = recv(clientSock, buf, sizeof(buf) - 1, 0)) == -1)
            ExitSys("recv", EXIT_FAILURE);
        buf[result] = '\0';

        puts(buf);
        if (strcmp(buf, "quit") == 0)
            break;
    }

    // Close the connection
    shutdown(clientSock, SHUT_RDWR);
    close(clientSock);

    return 0;
}

void ExitSys(const char* errorMsg, int status)
{
    perror(errorMsg);
    exit(status);
}
