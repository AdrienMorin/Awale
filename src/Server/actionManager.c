//
// Created by Nour on 27/11/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "actionManager.h"
#include "csvWriter.h"
#include "../../lib/libcsv/src/csv.h"

#define FILEPATH "data/users.csv"

//command parseRequest(jsonString request) {
//    // split the content of the buffer on the space elements, and store it in an array
//    char *token = strtok(buffer, " ");
//    char *words[MAX_WORDS];
//    int i = 0;
//
//    while (token != NULL && i < MAX_WORDS) {
//        words[i] = malloc(strlen(token) + 1);
//        strcpy(words[i], token);
//        token = strtok(NULL, " ");
//        i++;
//    }
//
//    // if the first word is getPlayerWithCredentials, we send a message to the server
//    if (strcmp(words[0], "getPlayerWithCredentials") == 0) {
//        if (i == 3) {
//            char *username = words[1];
//            char *password = words[2];
//
//            printf("getPlayerWithCredentials %s, %s\n", words[1], words[2]);
//
//            return (command) {LOGIN, {username, password}};
//        } else {
//            printf("tentative de getPlayerWithCredentials: nombre invalide d'arguments\n");
//            return (command) {UNKNOWN, {}};
//        }
//    }
//
//    return (command) {UNKNOWN, {}};
//}

cJSON processRequest(Client *client, Client clients[], int nbClients, jsonString req) {
    cJSON *requestJson = cJSON_Parse(req);

    cJSON *command = cJSON_GetObjectItemCaseSensitive(requestJson, "command");

    char *commandString = cJSON_GetStringValue(command);

    if (strncmp(commandString, "login", 5) == 0) {
        cJSON *username = cJSON_GetObjectItemCaseSensitive(requestJson, "username");
        cJSON *password = cJSON_GetObjectItemCaseSensitive(requestJson, "password");

        char *usernameString = cJSON_GetStringValue(username);
        char *passwordString = cJSON_GetStringValue(password);

        printf("Tentative de connexion, username : %s password : %s \n", usernameString, passwordString);

        return login(client, usernameString, passwordString, clients, nbClients);
    }

    if (strncmp(commandString, "register", 8) == 0) {
        cJSON *username = cJSON_GetObjectItemCaseSensitive(requestJson, "username");
        cJSON *password = cJSON_GetObjectItemCaseSensitive(requestJson, "password");

        char *usernameString = cJSON_GetStringValue(username);
        char *passwordString = cJSON_GetStringValue(password);

        printf("Tentative d'inscription, username : %s password : %s \n", usernameString, passwordString);

        return registerClient(usernameString, passwordString);
    }

    if (strncmp(commandString, "list", 4) == 0) {
        return listConnectedPlayers(client, clients, nbClients);
    }

    if (strncmp(commandString, "challenge", 9) == 0) {
        cJSON *username = cJSON_GetObjectItemCaseSensitive(requestJson, "username");
        char *usernameString = cJSON_GetStringValue(username);

        // Il faut savoir si le challenge est en attente ou non
        char *stateString = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(requestJson, "state"));

        if (strncmp(stateString, "pending", 7) == 0) {

            // On envoie une demande de challenge
            return sendChallengeRequest(client, clients, nbClients, usernameString);
        }
    }

    if (strncmp(commandString, "chat", 4) == 0) {
        cJSON *username = cJSON_GetObjectItemCaseSensitive(requestJson, "username");
        cJSON *message = cJSON_GetObjectItemCaseSensitive(requestJson, "message");
        char *usernameString = cJSON_GetStringValue(username);

        // On envoie un chat
        return sendChat(client, clients, nbClients, usernameString, cJSON_Print(message));
    }

    if (strncmp(commandString, "accept", 6) == 0) {

        return acceptChallenge(client);
    }

    if (strncmp(commandString, "refuse", 6) == 0) {

        return refuseChallenge(client);
    }

}

cJSON registerClient(char *username, char *password){
    cJSON *response = cJSON_CreateObject();

    joueur *j = getPlayerWithCredentials(username, password);
    if (j == NULL){
        printf("Inscription de %s\n", username);
        char row[100];
        strcpy(row, username);
        strcat(row, ",");
        strcat(row, password);
        strcat(row, ",0");
        printf("adding row : %s\n", row);
        if (addRowToCSV("data\\users.csv", row) > 0) {
            cJSON_AddStringToObject(response, "command", "register");
            cJSON_AddStringToObject(response, "status", "success");
            cJSON_AddStringToObject(response, "message", "Vous avez bien été inscrit !");
        } else {
            cJSON_AddStringToObject(response, "command", "register");
            cJSON_AddStringToObject(response, "status", "error");
            cJSON_AddStringToObject(response, "message", "Erreur lors de l'ajout dans la base de données…");
        }
    } else {
        printf("Tentative d'inscription de %s, pseudo déjà pris…\n", username);
        cJSON_AddStringToObject(response, "command", "register");
        cJSON_AddStringToObject(response, "status", "error");
        cJSON_AddStringToObject(response, "message", "Ce pseudo est déjà pris…");
    }
    return *response;
}

cJSON login(Client *client, char *username, char *password, Client *clients, int nbClients) {

    if (nbClients > 0){
        for (int i = 0; i < nbClients-1; i++) {
            if (strcmp(clients[i].j->nomUtilisateur, username) == 0) {
                cJSON *response = cJSON_CreateObject();
                cJSON_AddStringToObject(response, "command", "login");
                cJSON_AddStringToObject(response, "status", "error");
                cJSON_AddStringToObject(response, "message", "Ce compte est déjà connecté au serveur...");
                return *response;
            }
        }
    }

    joueur *j = getPlayerWithCredentials(username, password);


    if (j != NULL) {

        strncpy(client->name, j->nomUtilisateur, 50);

        cJSON *response = cJSON_CreateObject();
        cJSON_AddStringToObject(response, "command", "login");
        cJSON_AddStringToObject(response, "status", "success");
        cJSON_AddStringToObject(response, "message", "vous êtes connecté au serveur");
        cJSON_AddStringToObject(response, "username", j->nomUtilisateur);
        cJSON_AddNumberToObject(response, "nbGraines", j->nbGraines);

        // parse cases
        cJSON *cases = cJSON_CreateArray();
        for (int i = 0; i < 6; i++) {
            cJSON_AddItemToArray(cases, cJSON_CreateNumber(j->cases[i]));
        }
        cJSON_AddItemToObject(response, "cases", cases);

        client->j = copierJoueur(j);
        client->status = CONNECTED;

        return *response;
    } else {
        cJSON *response = cJSON_CreateObject();
        cJSON_AddStringToObject(response, "response", "error");
        cJSON_AddStringToObject(response, "message", "mauvais username ou mot de passe");
        return *response;
    }
}

joueur *getPlayerWithCredentials(char *username, char *password) {
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


cJSON listConnectedPlayers(Client *c, Client *clients, int nbClients) {
    cJSON *response = cJSON_CreateObject();
    cJSON_AddStringToObject(response, "command", "list");

    cJSON *players = cJSON_CreateArray();

    for (int i = 0; i < nbClients; i++) {

        // On ne va quand meme pas dire au client qu'il est connecté dans la liste des participants !
        if (clients[i].sock == c->sock) {
            continue;
        }

        cJSON *player = cJSON_CreateObject();
        cJSON_AddStringToObject(player, "username", clients[i].j->nomUtilisateur);
        cJSON_AddNumberToObject(player, "socket", clients[i].sock);
        cJSON_AddItemToArray(players, player);

        cJSON_AddItemToObject(response, "players", players);
    }

    return *response;
}

cJSON sendChallengeRequest(Client *client, Client *clients, int nbClients, char *username) {
    // envoyer la requete de challenge au bon client
    cJSON *response = cJSON_CreateObject();

    Client *adversaire = getClientByUsername(username, clients, nbClients);

    if (adversaire == NULL) {
        cJSON_AddStringToObject(response, "command", "challenge");
        cJSON_AddStringToObject(response, "status", "error");
        cJSON_AddStringToObject(response, "message", "L'utilisateur n'existe pas");

        return *response;
    }
    if (adversaire->status == DISCONNECTED) {
        cJSON_AddStringToObject(response, "command", "challenge");
        cJSON_AddStringToObject(response, "status", "fail");
        cJSON_AddStringToObject(response, "message", "L'utilisateur n'est pas connecté");

        return *response;
    }
    if (adversaire->status == CHALLENGED) {
        cJSON_AddStringToObject(response, "command", "challenge");
        cJSON_AddStringToObject(response, "status", "fail");
        cJSON_AddStringToObject(response, "message", "Votre adversaire a déjà une demande de challenge en attente");

        return *response;
    }
    if (adversaire->status == IN_GAME) {
        cJSON_AddStringToObject(response, "command", "challenge");
        cJSON_AddStringToObject(response, "status", "fail");
        cJSON_AddStringToObject(response, "message", "L'utilisateur est déjà en train de jouer");

        return *response;
    }

    cJSON_AddStringToObject(response, "command", "challenge");
    cJSON_AddStringToObject(response, "status", "success");
    cJSON_AddStringToObject(response, "message", "Demande de challenge envoyée à");
    cJSON_AddStringToObject(response, "sender", username);
    cJSON_AddNumberToObject(response, "sender_socket", client->sock);
    cJSON_AddStringToObject(response, "opponent", adversaire->name);
    cJSON_AddNumberToObject(response, "opponent_socket", adversaire->sock);
    cJSON_AddStringToObject(response, "state", "sent");

    adversaire->status = CHALLENGED;

    adversaire->challenge = malloc(sizeof(challenge));
    adversaire->challenge->challenger = client;


    // write the challenge request to the right client too

    cJSON *challengeRequest = cJSON_CreateObject();
    cJSON_AddStringToObject(challengeRequest, "command", "challenge");
    cJSON_AddStringToObject(challengeRequest, "status", "success");
    cJSON_AddStringToObject(challengeRequest, "message", "Vous avez reçu une demande de challenge de");
    cJSON_AddStringToObject(challengeRequest, "opponent", client->name);
    cJSON_AddNumberToObject(challengeRequest, "socket", client->sock);
    cJSON_AddStringToObject(challengeRequest, "state", "pending");

    write_client(adversaire->sock, cJSON_Print(challengeRequest));

    cJSON_free(challengeRequest);

    return *response;
}

cJSON sendChat(Client *client, Client *clients, int nbClients, char *username, char *message) {
    // envoyer la requete de challenge au bon client
    cJSON *response = cJSON_CreateObject();

    Client *dest = getClientByUsername(username, clients, nbClients);

    if (dest == NULL) {
        cJSON_AddStringToObject(response, "command", "challenge");
        cJSON_AddStringToObject(response, "status", "error");
        cJSON_AddStringToObject(response, "message", "L'utilisateur n'existe pas");

        return *response;
    }

    cJSON_AddStringToObject(response, "command", "chat");
    cJSON_AddStringToObject(response, "status", "success");
    cJSON_AddStringToObject(response, "message", "Chat envoyée à");
    cJSON_AddStringToObject(response, "from", username);
    cJSON_AddNumberToObject(response, "sender_socket", client->sock);
    cJSON_AddStringToObject(response, "to", dest->name);
    cJSON_AddNumberToObject(response, "opponent_socket", dest->sock);

    // send the chat

    cJSON *chat = cJSON_CreateObject();
    cJSON_AddStringToObject(chat, "command", "chat");
    cJSON_AddStringToObject(chat, "status", "success");
    cJSON_AddStringToObject(chat, "message", message);
    cJSON_AddStringToObject(chat, "from", client->name);
    cJSON_AddNumberToObject(chat, "socket", client->sock);

    write_client(dest->sock, cJSON_Print(chat));

    cJSON_free(chat);

    return *response;
}


cJSON acceptChallenge(Client *client) {
    Client *adversaire = client->challenge->challenger;

    // On envoie la réponse au client qui a accepté le challenge
    cJSON *response = cJSON_CreateObject();
    cJSON_AddStringToObject(response, "command", "accept");
    cJSON_AddStringToObject(response, "status", "success");
    cJSON_AddStringToObject(response, "message", "Vous avez accepté la demande de challenge de");
    cJSON_AddStringToObject(response, "opponent", adversaire->name);
    cJSON_AddNumberToObject(response, "opponent_socket", adversaire->sock);

    // On envoie la challengeRequest au client qui a envoyé la demande de challenge
    cJSON *challengeRequest = cJSON_CreateObject();
    cJSON_AddStringToObject(challengeRequest, "command", "accept");
    cJSON_AddStringToObject(challengeRequest, "status", "success");
    cJSON_AddStringToObject(challengeRequest, "message", "Votre adversaire a accepté votre demande de challenge");
    cJSON_AddStringToObject(challengeRequest, "opponent", client->name);
    cJSON_AddNumberToObject(challengeRequest, "opponent_socket", client->sock);


    write_client(adversaire->sock, cJSON_Print(challengeRequest));

    cJSON_free(challengeRequest);

    client->status = IN_GAME;
    adversaire->status = IN_GAME;

    return *response;
}

cJSON refuseChallenge(Client *client) {
    Client *adversaire = client->challenge->challenger;

    cJSON *response = cJSON_CreateObject();
    cJSON_AddStringToObject(response, "command", "challenge");
    cJSON_AddStringToObject(response, "status", "success");
    cJSON_AddStringToObject(response, "message", "Vous avez refusé la demande de challenge de");
    cJSON_AddStringToObject(response, "opponent", adversaire->name);
    cJSON_AddNumberToObject(response, "opponent_socket", adversaire->sock);
    cJSON_AddStringToObject(response, "state", "refused");

    cJSON *challengeRequest = cJSON_CreateObject();
    cJSON_AddStringToObject(challengeRequest, "command", "challenge");
    cJSON_AddStringToObject(challengeRequest, "status", "success");
    cJSON_AddStringToObject(challengeRequest, "message", "Votre adversaire a refusé votre demande de challenge");
    cJSON_AddStringToObject(challengeRequest, "opponent", client->name);
    cJSON_AddNumberToObject(challengeRequest, "opponent_socket", client->sock);
    cJSON_AddStringToObject(challengeRequest, "state", "refused");

    write_client(adversaire->sock, cJSON_Print(challengeRequest));

    cJSON_free(challengeRequest);

    return *response;
}


Client *getClientByUsername(char *username, Client *clients, int nbClients) {
    for (int i = 0; i < nbClients; i++) {
        if (strcmp(clients[i].j->nomUtilisateur, username) == 0) {
            return &clients[i];
        }
    }
    return NULL;
}