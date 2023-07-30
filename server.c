#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORTNO	5061

void ExitSys(const char* errorMsg, int status);

int main(void)
{
    int serverSock, clientSock;
    struct sockaddr_in sinServer, sinClient;
    socklen_t addrLenClient;
    char buf[1024];
    int result;

    if ((serverSock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        ExitSys("socket", EXIT_FAILURE);

    memset(&sinServer, 0, sizeof(sinServer));
    sinServer.sin_family = AF_INET;
    sinServer.sin_port = htons(PORTNO);
    sinServer.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSock, (struct sockaddr*)&sinServer, sizeof(sinServer)) == -1)
        ExitSys("bind", EXIT_FAILURE);

    if (listen(serverSock, 8) == -1)
        ExitSys("listen", EXIT_FAILURE);

    printf("waiting for connection...\n");

    addrLenClient = sizeof(sinClient);
    if ((clientSock = accept(serverSock, (struct sockaddr*)&sinClient, &addrLenClient)) == -1) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    printf("Client connected: %s:%d\n", inet_ntoa(sinClient.sin_addr), ntohs(sinClient.sin_port));

    for (;;) {
        // Receive data from the client
        if ((result = recv(clientSock, buf, sizeof(buf) - 1, 0)) == -1)
            ExitSys("recv", EXIT_FAILURE);
        buf[result] = '\0';

        puts(buf);
        if (strcmp(buf, "quit") == 0)
            break;

        // Send response back to the client
        printf("Text:");
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\n")] = '\0';


        // Send response back to the client (Echo the received data)
        if (send(clientSock, buf, strlen(buf), 0) == -1)
            ExitSys("send", EXIT_FAILURE);
        if (strcmp(buf, "quit") == 0)
            break;
    }

    // Close the connection
    shutdown(clientSock, SHUT_RDWR);
    close(clientSock);
    close(serverSock);

    return 0;
}

void ExitSys(const char* errorMsg, int status)
{
    perror(errorMsg);
    exit(status);
}
