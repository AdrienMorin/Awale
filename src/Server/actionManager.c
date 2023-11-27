//
// Created by Nour on 27/11/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "actionManager.h"


command parseCommand(char *buffer) {
    // split the content of the buffer on the space elements, and store it in an array
    char *token = strtok(buffer, " ");
    char *words[MAX_WORDS];
    int i = 0;

    while (token != NULL && i < MAX_WORDS) {
        words[i] = malloc(strlen(token) + 1);
        strcpy(words[i], token);
        token = strtok(NULL, " ");
        i++;
    }

    // if the first word is login, we send a message to the server
    if (strcmp(words[0], "login") == 0) {
        if (i == 3) {
            char *username = words[1];
            char *password = words[2];

            printf("login %s, %s\n", words[1], words[2]);

            return (command) {LOGIN, {username, password}};
        } else {
            printf("login: invalid number of arguments\n");
            return (command) {UNKNOWN, {}};
        }
    }

    return (command) {UNKNOWN, {}};
}

joueur *login(char *username, char *password) {
    int i = 0;
    CsvParser *csvparser = CsvParser_new("data/users.csv", ",", 1);
    CsvRow *row;
    const CsvRow *header = CsvParser_getHeader(csvparser);

    if (header == NULL) {
        printf("%s\n", CsvParser_getErrorMessage(csvparser));
        return NULL;
    }

    while ((row = CsvParser_getRow(csvparser))) {
        const char **rowFields = CsvParser_getFields(row);
        if (strcmp(rowFields[0], username) == 0 && strcmp(rowFields[1], password) == 0) {
            joueur *j = initialiserJoueur(username);
            CsvParser_destroy_row(row);
            CsvParser_destroy(csvparser);
            return j;
        }
        CsvParser_destroy_row(row);
    }
    CsvParser_destroy(csvparser);

    return NULL;

}
