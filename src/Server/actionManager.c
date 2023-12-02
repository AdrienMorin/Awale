//
// Created by Nour on 27/11/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "actionManager.h"
#include "csvWriter.h"


partie *p[100];
int nbParties = 0;
#define FILEPATH "data/users.csv"

void processRequest(Client *client, Client clients[], int *nbClients, jsonString req) {
    cJSON *requestJson = cJSON_Parse(req);

    cJSON *command = cJSON_GetObjectItemCaseSensitive(requestJson, "command");

    char *commandString = cJSON_GetStringValue(command);

    cJSON response;

    if (strncmp(commandString, "login", 5) == 0) {
        cJSON *username = cJSON_GetObjectItemCaseSensitive(requestJson, "username");
        cJSON *password = cJSON_GetObjectItemCaseSensitive(requestJson, "password");

        char *usernameString = cJSON_GetStringValue(username);
        char *passwordString = cJSON_GetStringValue(password);

        printf("Tentative de connexion, username : %s password : %s \n", usernameString, passwordString);

        response = login(client, usernameString, passwordString, clients, *nbClients);
        write_client(client->sock, cJSON_Print(&response));
    }

    if (strncmp(commandString, "register", 8) == 0) {
        cJSON *username = cJSON_GetObjectItemCaseSensitive(requestJson, "username");
        cJSON *password = cJSON_GetObjectItemCaseSensitive(requestJson, "password");

        char *usernameString = cJSON_GetStringValue(username);
        char *passwordString = cJSON_GetStringValue(password);

        printf("Tentative d'inscription, username : %s password : %s \n", usernameString, passwordString);

        response = registerClient(usernameString, passwordString);
        write_client(client->sock, cJSON_Print(&response));
    }

    if (strncmp(commandString, "list", 4) == 0) {
        response = listConnectedPlayers(client, clients, *nbClients);

        write_client(client->sock, cJSON_Print(&response));
    }

    if (strncmp(commandString, "challenge", 9) == 0) {
        cJSON *username = cJSON_GetObjectItemCaseSensitive(requestJson, "username");
        char *usernameString = cJSON_GetStringValue(username);

        // Il faut savoir si le challenge est en attente ou non
        char *stateString = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(requestJson, "state"));

        if (strncmp(stateString, "pending", 7) == 0) {

            // On envoie une demande de challenge
            response = sendChallengeRequest(client, clients, *nbClients, usernameString);

            write_client(client->sock, cJSON_Print(&response));
        }
    }

    if (strncmp(commandString, "chat", 4) == 0) {
        cJSON *username = cJSON_GetObjectItemCaseSensitive(requestJson, "username");
        cJSON *message = cJSON_GetObjectItemCaseSensitive(requestJson, "message");
        char *usernameString = cJSON_GetStringValue(username);

        // On envoie un chat
        response = sendChat(client, clients, *nbClients, usernameString, cJSON_Print(message));
        write_client(client->sock, cJSON_Print(&response));
    }

    if (strncmp(commandString, "accept", 6) == 0) {

        response = acceptChallenge(client);

        lancerPartie(client, clients, *nbClients);
    }

    if (strncmp(commandString, "refuse", 6) == 0) {

        response = refuseChallenge(client);

        write_client(client->sock, cJSON_Print(&response));
    }

    if (strncmp(commandString, "play", 4) == 0) {
        cJSON *caseChoisie = cJSON_GetObjectItemCaseSensitive(requestJson, "caseChoisie");

        int caseChoisieInt = (int) cJSON_GetNumberValue(caseChoisie);

        jouerCoupServeur(client, caseChoisieInt);
    }

    if (strncmp(commandString, "quit", 4) == 0) {


        write_client(client->sock, cJSON_Print(&response));
    }

    if (strncmp(commandString, "disconnect", 10) == 0) {

        response = disconnectClient(client);
        write_client(client->sock, cJSON_Print(&response));

    }

    if (strncmp(commandString, "surrender", 9) == 0) {

        response = surrender(client);
        write_client(client->sock, cJSON_Print(&response));
    }


}

cJSON registerClient(char *username, char *password){
    cJSON *response = cJSON_CreateObject();

    joueur *j = getPlayerWithCredentials(username, password);
    if (j == NULL){
        printf("Inscription de %s\n", username);
        printf("adding row : %s\n", username);
        if (addRowToCSV("data/users.csv", username, password) > 0) {
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

    Client *c = getClientByUsername(username, clients, nbClients);

    if (nbClients > 0){

        if (c != NULL && c->status != DISCONNECTED) {
            cJSON *response = cJSON_CreateObject();
            cJSON_AddStringToObject(response, "command", "login");
            cJSON_AddStringToObject(response, "status", "error");
            cJSON_AddStringToObject(response, "message", "Ce compte est déjà connecté au serveur...");
            return *response;
        }

    }

    joueur *j = getPlayerWithCredentials(username, password);

    if (j != NULL) {

        cJSON *response = cJSON_CreateObject();

        cJSON_AddStringToObject(response, "command", "login");
        cJSON_AddStringToObject(response, "status", "success");
        cJSON_AddStringToObject(response, "message", "vous êtes connecté au serveur");
        cJSON_AddStringToObject(response, "username", j->nomUtilisateur);


        // If the client was already connected, we just need to update his status
        if (c != NULL) {
            c->status = CONNECTED;
            return *response;
        }


        // else we need to create a new client
        strncpy(client->name, j->nomUtilisateur, 50);

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

    }

    cJSON_AddItemToObject(response, "players", players);

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
    cJSON_AddStringToObject(response, "sender", client->name);
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
    cJSON_AddStringToObject(response, "joueur1", adversaire->name);
    cJSON_AddStringToObject(response, "joueur2", client->name);
    cJSON_AddStringToObject(response, "state", "accepted");

    adversaire->challenge = malloc(sizeof(challenge));
    adversaire->challenge->challenger = client;

    //write_client(adversaire->sock, cJSON_Print(response));

    return *response;
}

cJSON refuseChallenge(Client *client) {
    Client *adversaire = client->challenge->challenger;

    cJSON *response = cJSON_CreateObject();
    cJSON_AddStringToObject(response, "command", "refuse");
    cJSON_AddStringToObject(response, "status", "success");
    cJSON_AddStringToObject(response, "message", "Vous avez refusé la demande de challenge de");
    cJSON_AddStringToObject(response, "opponent", adversaire->name);
    cJSON_AddNumberToObject(response, "opponent_socket", adversaire->sock);
    cJSON_AddStringToObject(response, "state", "refused");

    cJSON *challengeRequest = cJSON_CreateObject();
    cJSON_AddStringToObject(challengeRequest, "command", "refuse");
    cJSON_AddStringToObject(challengeRequest, "status", "success");
    cJSON_AddStringToObject(challengeRequest, "message", "Votre adversaire a refusé votre demande de challenge");
    cJSON_AddStringToObject(challengeRequest, "opponent", client->name);
    cJSON_AddNumberToObject(challengeRequest, "opponent_socket", client->sock);
    cJSON_AddStringToObject(challengeRequest, "state", "refused");

    client->status = CONNECTED;

    write_client(adversaire->sock, cJSON_Print(challengeRequest));

    cJSON_free(challengeRequest);

    return *response;
}

void lancerPartie(Client *client, Client *clients, int nbClients) {

    client->j = initialiserJoueur(client->name);

    Client *opponent = getClientByUsername(client->challenge->challenger->name, clients, nbClients);

    p[nbParties] = initialiserPartie(client->j, opponent->j);

    partie *partie = p[nbParties];

    client->challenge->p = partie;

    opponent->challenge->p = partie;

    partie->quiJoue = choisirJoueurAleatoire(client->j, opponent->j);

    nbParties++;

    client->status = IN_GAME;
    opponent->status = IN_GAME;

    cJSON *response = cJSON_CreateObject();
    cJSON_AddStringToObject(response, "command", "game started");
    cJSON_AddStringToObject(response, "status", "success");
    cJSON_AddStringToObject(response, "joueur1", client->name);
    cJSON_AddStringToObject(response, "joueur2", opponent->name);
    cJSON_AddStringToObject(response, "quiJoue", partie->quiJoue->nomUtilisateur);
    cJSON_AddStringToObject(response, "state", "accepted");

    write_client(client->sock, cJSON_Print(response));
    write_client(opponent->sock, cJSON_Print(response));

    cJSON_free(response);
}

Client *getClientByUsername(char *username, Client *clients, int nbClients) {
    for (int i = 0; i < nbClients; i++) {
        if (clients[i].j == NULL){
            continue;
        }
        if (strcmp(clients[i].j->nomUtilisateur, username) == 0) {
            return &clients[i];
        }
    }
    return NULL;
}

void jouerCoupServeur(Client *client, int caseChoisie) {
    partie *partie = client->challenge->p;

    cJSON response;
    if (coupValide(partie, client->name, caseChoisie)) {

        int caseArrivee = egrener(partie, client->j, caseChoisie);

        manger(partie, partie->quiJoue, caseArrivee);

        partie->quiJoue = (partie->quiJoue == partie->joueur1) ? partie->joueur2 : partie->joueur1;

        // add cases to response
        cJSON *cases = cJSON_CreateArray();
        for (int i = 0; i < 6; i++) {
            cJSON_AddItemToArray(cases, cJSON_CreateNumber(client->j->cases[i]));
        }

        response = buildValidMoveResponse(client, client->challenge->challenger, caseChoisie);

        cJSON_AddItemToObject(&response, "cases", cases);
    } else {
        response = buildInvalidMoveResponse(client, client->challenge->challenger);
    }

    write_client(client->sock, cJSON_Print(&response));
    write_client(client->challenge->challenger->sock, cJSON_Print(&response));

    if (partieTerminee(partie)) {
        Client *winner = (partie->joueur1->nbGraines > partie->joueur2->nbGraines) ? client
                                                                                   : client->challenge->challenger;
        Client *loser = winner->challenge->challenger;

        sendEndOfGame(winner, loser);

        removePartie(client);
    }
}

cJSON buildValidMoveResponse(Client *client, Client *opponent, int caseChoisie) {
    cJSON *response = cJSON_CreateObject();
    cJSON_AddStringToObject(response, "command", "play");
    cJSON_AddStringToObject(response, "status", "success");
    cJSON_AddStringToObject(response, "state", "valid");
    cJSON_AddNumberToObject(response, "caseChoisie", caseChoisie);

    return *response;
}

cJSON buildInvalidMoveResponse(Client *client, Client *opponent) {
    cJSON *response = cJSON_CreateObject();
    cJSON_AddStringToObject(response, "command", "play");
    cJSON_AddStringToObject(response, "status", "success");
    cJSON_AddStringToObject(response, "state", "invalid");

    return *response;
}

cJSON disconnectClient(Client *client) {
    cJSON *response = cJSON_CreateObject();

    if (client->status == IN_GAME) {
        sendWinByForfeitResponse(client);
        removePartie(client);
        Client *opponent = client->challenge->challenger;
        opponent->status = CONNECTED;
    }

    client->status = DISCONNECTED;

    *response = buildDisconnectResponse(client);

    return *response;
}

cJSON buildDisconnectResponse(Client *client) {

    cJSON *response = cJSON_CreateObject();

    cJSON_AddStringToObject(response, "command", "disconnect");
    cJSON_AddStringToObject(response, "status", "success");

    return *response;
}

void sendWinByForfeitResponse(Client *client) {
    cJSON *opponentResponse = cJSON_CreateObject();

    // End the game of the client
    Client *opponent = client->challenge->challenger;

    *opponentResponse = buildWinnerResponse();

    jsonString opponentResponseString = cJSON_Print(opponentResponse);

    write_client(opponent->sock, opponentResponseString);

}

void removePartie(Client *client) {

    // free the partie from the p array, and shift the other elements

    for (int i = 0; i < nbParties; i++) {
        if (p[i] == client->challenge->p) {

            free(p[i]);

            for (int j = i; j < nbParties - 1; j++) {
                p[j] = p[j + 1];
            }

            nbParties--;
            break;
        }
    }
}

int getClientIndex(Client *client, Client *clients, int nbClients) {
    for (int i = 0; i < nbClients; i++) {
        if (clients[i].sock == client->sock) {
            return i;
        }
    }
    return -1;
}

void sendEndOfGame(Client *client1, Client *client2) {

    // Update client statuses
    client1->status = CONNECTED;
    client2->status = CONNECTED;

    // if the scores are equal, it's a draw
    if (client1->j->nbGraines == client2->j->nbGraines) {
        cJSON drawResponse = buildDrawResponse();
        write_client(client1->sock, cJSON_Print(&drawResponse));
        write_client(client2->sock, cJSON_Print(&drawResponse));
        return;
    }

    // Build client1 and client2 responses
    cJSON winnerResponse = buildWinnerResponse();
    cJSON loserResponse = buildLoserResponse();

    // Send responses to clients
    write_client(client1->sock, cJSON_Print(&winnerResponse));
    write_client(client2->sock, cJSON_Print(&loserResponse));

}

cJSON buildWinnerResponse() {
    cJSON *response = cJSON_CreateObject();
    cJSON_AddStringToObject(response, "command", "game end");
    cJSON_AddStringToObject(response, "status", "success");
    cJSON_AddStringToObject(response, "state", "win");
    cJSON_AddStringToObject(response, "message", "Vous avez gagné la partie !");

    return *response;
}

cJSON buildLoserResponse() {
    cJSON *response = cJSON_CreateObject();
    cJSON_AddStringToObject(response, "command", "game end");
    cJSON_AddStringToObject(response, "status", "success");
    cJSON_AddStringToObject(response, "state", "lose");
    cJSON_AddStringToObject(response, "message", "Vous avez perdu la partie !");

    return *response;
}

cJSON buildDrawResponse() {
    cJSON *response = cJSON_CreateObject();
    cJSON_AddStringToObject(response, "command", "game end");
    cJSON_AddStringToObject(response, "status", "success");
    cJSON_AddStringToObject(response, "state", "draw");
    cJSON_AddStringToObject(response, "message", "Vous avez fait match nul !");

    return *response;
}

cJSON surrender(Client *client) {
    cJSON *response = cJSON_CreateObject();

    sendWinByForfeitResponse(client);

    removePartie(client);

    Client *opponent = client->challenge->challenger;

    opponent->status = CONNECTED;
    client->status = CONNECTED;

    *response = buildLoserResponse();

    return *response;
}


