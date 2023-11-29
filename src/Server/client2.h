#ifndef CLIENT_H
#define CLIENT_H

#include "server2.h"
#include "../partie.h"


typedef enum {
    CONNECTED,
    CHALLENGED,
    IN_GAME,
    DISCONNECTED
} playerStatus;

typedef struct Client
{
    SOCKET sock;
    joueur *j;
    playerStatus status;
    char name[BUF_SIZE];
    struct challenge *challenge;
}Client;

typedef struct challenge {
    Client *challenger;
    partie *p;
} challenge;

#endif /* guard */
