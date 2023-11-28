#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "actionManager.h"
#include "client2.h"

static void init(void) {
#ifdef WIN32
    WSADATA wsa;
    int err = WSAStartup(MAKEWORD(2, 2), &wsa);
    if(err < 0)
    {
       puts("WSAStartup failed !");
       exit(EXIT_FAILURE);
    }
#endif
}

static void end(void) {
#ifdef WIN32
    WSACleanup();
#endif
}

static void app(const char *address) {
    SOCKET sock = init_connection(address);
    char buffer[BUF_SIZE];

    fd_set rdfs;

    int connected = FALSE;
    /* send our name */
    write_server(sock, "unknown");

    while (1) {
        FD_ZERO(&rdfs);

        /* add STDIN_FILENO */
        FD_SET(STDIN_FILENO, &rdfs);

        /* add the socket */
        FD_SET(sock, &rdfs);

        if (select(sock + 1, &rdfs, NULL, NULL, NULL) == -1) {
            perror("select()");
            exit(errno);
        }

        /* something from standard input : i.e keyboard */
        if (FD_ISSET(STDIN_FILENO, &rdfs)) {
            fgets(buffer, BUF_SIZE - 1, stdin);
            {
                char *p = NULL;
                p = strstr(buffer, "\n");
                if (p != NULL) {
                    *p = 0;
                } else {
                    /* fclean */
                    buffer[BUF_SIZE - 1] = 0;
                }
            }

            jsonString request = parseRequest(buffer);
            cJSON *requestJson = cJSON_Parse(request);

            cJSON *command = cJSON_GetObjectItemCaseSensitive(requestJson, "command");

            char *commandString = cJSON_GetStringValue(command);

            if (strncmp(request, "error", 5) == 0) {
                printf("Invalid command\n");
            } else if (connected == FALSE &&
                       (strncmp(commandString, "login", 5) != 0 && strncmp(commandString, "register", 8) != 0)) {
                printf("You must be connected to do this action\n");
                printf("Please login first : login <username> <password>\n");
            } else {
                write_server(sock, request);
            }

        } else if (FD_ISSET(sock, &rdfs)) {
            int n = read_server(sock, buffer);
            /* server down */
            if (n == 0) {
                printf("Server disconnected !\n");
                break;
            }

            cJSON *response = cJSON_Parse(buffer);

            processResponse(response, &connected);

            puts(buffer);
        }
    }

    end_connection(sock);
}

static int init_connection(const char *address) {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN sin = {0};
    struct hostent *hostinfo;

    if (sock == INVALID_SOCKET) {
        perror("socket()");
        exit(errno);
    }

    hostinfo = gethostbyname(address);
    if (hostinfo == NULL) {
        fprintf(stderr, "Unknown host %s.\n", address);
        exit(EXIT_FAILURE);
    }

    sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr;
    sin.sin_port = htons(PORT);
    sin.sin_family = AF_INET;

    if (connect(sock, (SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR) {
        perror("connect()");
        exit(errno);
    }

    return sock;
}

static void end_connection(int sock) {
    closesocket(sock);
}

static int read_server(SOCKET sock, char *buffer) {
    int n = 0;

    if ((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0) {
        perror("recv()");
        exit(errno);
    }

    buffer[n] = 0;

    return n;
}

static void write_server(SOCKET sock, const char *buffer) {
    if (send(sock, buffer, strlen(buffer), 0) < 0) {
        perror("send()");
        exit(errno);
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage : %s [address] [pseudo]\n", argv[0]);
        return EXIT_FAILURE;
    }

    init();

    app(argv[1]);

    end();

    return EXIT_SUCCESS;
}
