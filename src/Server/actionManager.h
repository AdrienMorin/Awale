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

cJSON processRequest(Client client, jsonString req);

cJSON login(Client client, char *username, char *password);


joueur *getPlayerWithCredentials(char *username, char *password);



#endif //AWALE_ACTIONMANAGER_H
