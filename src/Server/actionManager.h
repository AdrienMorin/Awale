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

void processRequest(Client *client, Client clients[], int *nbClients, jsonString req);

cJSON login(Client *client, char *username, char *password, Client *clients, int nbClients);

cJSON registerClient(char *username, char *password);

cJSON sendChallengeRequest(Client *client, Client clients[], int nbClients, char *username);

cJSON sendChat(Client *client, Client clients[], int nbClients, char *username, char *message);

cJSON acceptChallenge(Client *client);

cJSON refuseChallenge(Client *client);

cJSON listConnectedPlayers(Client *c, Client *clients, int nbClients);

joueur *getPlayerWithCredentials(char *username, char *password);

Client *getClientByUsername(char *username, Client *clients, int nbClients);

void lancerPartie(Client *client, Client *clients, int nbClients);

void jouerCoupServeur(Client *client, int caseChoisie);

cJSON buildValidMoveResponse(Client *client, Client *opponent, int caseChoisie);

cJSON buildInvalidMoveResponse(Client *client, Client *opponent);

cJSON buildDisconnectResponse(Client *client);

cJSON buildWinnerResponse();

cJSON buildLoserResponse();

cJSON buildDrawResponse();

cJSON disconnectClient(Client *client);

cJSON surrender(Client *client);

void sendWinByForfeitResponse(Client *client);

void sendEndOfGame(Client *client1, Client *client2);

void removePartie(Client *client);

int getClientIndex(Client *client, Client *clients, int nbClients);


#endif //AWALE_ACTIONMANAGER_H
