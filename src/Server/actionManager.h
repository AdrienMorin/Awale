//
// Created by Nour on 27/11/2023.
//

#ifndef AWALE_ACTIONMANAGER_H
#define AWALE_ACTIONMANAGER_H

#define MAX_WORDS 50
#define MAX_WORD_LENGTH 50

#include "../joueur.h"
#include "../../lib/CsvParser/csvparser.h"
#include "../../lib/cJSON/cJSON.h"
#include "server2.h"

typedef char *jsonString;

typedef struct {
    int code;
    char *args[MAX_WORDS];
} command;

typedef enum {
    LOGIN,
    REGISTER,
    QUIT,
    PLAY,
    LIST,
    HELP,
    UNKNOWN
} commandCode;

cJSON parseRequest(jsonString request);

cJSON processRequest(Client *client, Client clients[], int nbClients, jsonString req);

cJSON login(Client *client, char *username, char *password);

cJSON sendChallengeRequest(Client *client, Client clients[], int nbClients, char *username);

cJSON listConnectedPlayers(Client *c, Client *clients, int nbClients);

joueur *getPlayerWithCredentials(char *username, char *password);

Client *getClientByUsername(char *username, Client *clients, int nbClients);



#endif //AWALE_ACTIONMANAGER_H
