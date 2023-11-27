//
// Created by Nour on 27/11/2023.
//

#ifndef AWALE_ACTIONMANAGER_H
#define AWALE_ACTIONMANAGER_H

#define MAX_WORDS 50
#define MAX_WORD_LENGTH 50

#include "../joueur.h"
#include "csvparser.h"

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

command parseCommand(char *buffer);

joueur *login(char *username, char *password);

#endif //AWALE_ACTIONMANAGER_H
