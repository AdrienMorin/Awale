/* Serveur sockets TCP
 * affichage de ce qui arrive sur la socket
 *    socket_server port (port > 1024 sauf root)
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char **argv) {
    char data[128];
    int sockfd, newsockfd, clilen, chilpid, ok, nleft, nbwriten;
    char c;
    struct sockaddr_in cli_addr, serv_addr;
    // Déclaration et initialisation du tableau des clients
    int clients[20][5] = {0};


    if (argc != 2) {
        printf("usage: socket_server port\n");
        exit(0);
    }

    printf("server starting...\n");

    /* ouverture du socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("impossible d'ouvrir le socket\n");
        exit(0);
    }

    /* initialisation des parametres */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    /* effecture le bind */
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("impossible de faire le bind\n");
        exit(0);
    }

    /* petit initialisation */
    listen(sockfd, 1);

    /* attend la connection d'un client */
    clilen = sizeof(cli_addr);

    while(1){
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            printf("accept error\n");
            exit(0);
        }
        // Obtenir l'adresse IP du client
        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(cli_addr.sin_addr), clientIP, INET_ADDRSTRLEN);

        printf("Connection accepted from %s:%d\n", clientIP, ntohs(cli_addr.sin_port));

        int pid = fork();
        if (pid == 0) { // C'est le fils
            close(sockfd); // socket inutile pour le fils
            // Traiter la communication
            while (data[0] != EOF) {
                // On vide le buffer
                memset(data, 0, sizeof(data));
                // On lit sur le socket
                read(newsockfd, data, 128);
                if (data[0] == EOF) {
                    printf("Client disconnected : %s:%d\r\n", clientIP, ntohs(cli_addr.sin_port));
                } else
                printf("Message from %s:%d : %s\r\n", clientIP, ntohs(cli_addr.sin_port), data);
            }
            close(newsockfd);
            exit(0);
        } else { // C'est le père
            close(newsockfd);
        }
    }

    close(sockfd);

    /*  attention il s'agit d'une boucle infinie
     *  le socket nn'est jamais ferme !
     */

    return 1;
}
