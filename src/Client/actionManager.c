//
// Created by Nour on 27/11/2023.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "actionManager.h"
#include "../../lib/cJSON/cJSON.h"

#define MAX_WORDS 50

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
            return "error";
        } else {
            return parseLogin(words);
        }
    }

    if (strcmp(words[0], "list") == 0) {
        if (i != 1) {
            return "error";
        } else {
            cJSON *request = cJSON_CreateObject();

            cJSON *command = cJSON_CreateString("list");


            cJSON_AddItemToObject(request, "command", command);

            return cJSON_Print(request);
        }
    }


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
