//
// Created by Nour on 27/11/2023.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "actionManager.h"
#include "../../lib/cJSON/cJSON.h"
#include "../partie.h"

#define MAX_WORDS 50


joueur *j;


jsonString parseRequest(char *buffer) {
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

    // according to the first word, we call the proper command parsing function

    if (strcmp(words[0], "login") == 0) {
        if (i != 3) {
            printf("Tentative de connexion: nombre invalide d'arguments\n");
            printf("Usage: login <username> <password>\n");

            return "error";
        } else {
            return parseLogin(words);
        }
    } else if (strcmp(words[0], "list") == 0) {
        if (i != 1) {
            printf("Tentative de list: nombre invalide d'arguments\n");
            printf("Usage: list\n");

            return "error";
        } else {
            cJSON *request = cJSON_CreateObject();

            cJSON *command = cJSON_CreateString("list");


            cJSON_AddItemToObject(request, "command", command);

            return cJSON_Print(request);
        }
    } else if (strncmp(words[0], "challenge", 9) == 0) {
        if (i != 2) {
            printf("Tentative de challenge: nombre invalide d'arguments\n");
            printf("Usage: challenge <username>\n");
            return "error";
        } else {

            return parseChallenge(words);
        }
    } else if (strncmp(words[0], "accept", 6) == 0) {
        if (i != 1) {
            printf("Tentative de challenge: nombre invalide d'arguments\n");
            printf("Usage: accept\n");
            return "error";
        } else {

            return acceptChallenge();
        }
    } else if (strncmp(words[0], "refuse", 6) == 0) {
        if (i != 1) {
            printf("Tentative de challenge: nombre invalide d'arguments\n");
            printf("Usage: refuse\n");
            return "error";
        } else {
            return refuseChallenge();
        }
    }

    return "error";

}

jsonString parseLogin(char *args[]) {
    cJSON *request = cJSON_CreateObject();

    cJSON *command = cJSON_CreateString("login");
    cJSON *username = cJSON_CreateString(args[1]);
    cJSON *password = cJSON_CreateString(args[2]);

    cJSON_AddItemToObject(request, "command", command);
    cJSON_AddItemToObject(request, "username", username);
    cJSON_AddItemToObject(request, "password", password);

    return cJSON_Print(request);

}

jsonString parseChallenge(char *args[]) {
    cJSON *request = cJSON_CreateObject();

    cJSON *command = cJSON_CreateString("challenge");
    cJSON *username = cJSON_CreateString(args[1]);
    cJSON *state = cJSON_CreateString("pending");

    cJSON_AddItemToObject(request, "command", command);
    cJSON_AddItemToObject(request, "username", username);
    cJSON_AddItemToObject(request, "state", state);

    return cJSON_Print(request);
}

void processResponse(cJSON *response, int *connected) {

    if (response == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
    }

    cJSON *command = cJSON_GetObjectItemCaseSensitive(response, "command");

    char *commandString = cJSON_GetStringValue(command);

    if (strncmp(commandString, "login", 5) == 0) {
        cJSON *status = cJSON_GetObjectItemCaseSensitive(response, "status");

        char *statusString = cJSON_GetStringValue(status);

        char *username = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(response, "username"));

        if (strncmp(statusString, "success", 7) == 0) {
            j = initialiserJoueur(username);
            *connected = TRUE;
        } else {
            printf("Login failed\n");
        }
    }


    if (strncmp(commandString, "challenge", 9) == 0) {
        cJSON *status = cJSON_GetObjectItemCaseSensitive(response, "status");

        char *statusString = cJSON_GetStringValue(status);

        char *opponent = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(response, "opponent"));

        char *message = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(response, "message"));

        if (strncmp(statusString, "success", 7) == 0) {
            printf("%s %s !\n", message, opponent);
        } else {
            printf("%s\n", message);
        }
    }

    if (strncmp(commandString, "accept", 6) == 0) {
        cJSON *status = cJSON_GetObjectItemCaseSensitive(response, "status");

        char *statusString = cJSON_GetStringValue(status);

        char *opponent = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(response, "opponent"));

        char *message = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(response, "message"));

        if (strncmp(statusString, "success", 7) == 0) {
            printf("Entering game with %s !\n", opponent);
        } else {
            printf("%s\n", message);
        }
    }

    if (strncmp(commandString, "refuse", 6) == 0) {
        cJSON *status = cJSON_GetObjectItemCaseSensitive(response, "status");

        char *statusString = cJSON_GetStringValue(status);

        char *opponent = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(response, "opponent"));

        char *message = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(response, "message"));

        if (strncmp(statusString, "success", 7) == 0) {
            printf("%s refused your challenge !\n", opponent);
        } else {
            printf("%s\n", message);
        }
    }

}

jsonString acceptChallenge() {
    cJSON *request = cJSON_CreateObject();

    cJSON *command = cJSON_CreateString("accept");

    cJSON_AddItemToObject(request, "command", command);

    return cJSON_Print(request);
}

jsonString refuseChallenge() {
    cJSON *request = cJSON_CreateObject();

    cJSON *command = cJSON_CreateString("refuse");

    cJSON_AddItemToObject(request, "command", command);

    return cJSON_Print(request);
}
