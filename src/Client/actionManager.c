//
// Created by Nour on 27/11/2023.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "actionManager.h"

#define MAX_WORDS 50


int inGame = FALSE;
partie *p;


jsonString parseRequest(char *buffer) {
    // split the content of the buffer on the space elements, and store it in an array
    char bufferCopy[200];
    strcpy(bufferCopy, buffer);
    char *token = strtok(buffer, " ");
    char *words[MAX_WORDS];
    int i = 0;

    while (token != NULL && i < MAX_WORDS) {
        words[i] = malloc(strlen(token) + 1);
        strcpy(words[i], token);
        token = strtok(NULL, " ");
        i++;
    }

    // clear the buffer
    //memset(buffer, 0, strlen(buffer));

    // according to the first word, we call the proper command parsing function

    if (strcmp(words[0], "login") == 0) {
        if (i != 3) {
            printf("Tentative de connexion: nombre invalide d'arguments\n");
            printf("Usage: login <username> <password>\n");

            return "error";
        } else {
            return parseLogin(words);
        }
    } else if (strcmp(words[0], "register") == 0) {
        if (i != 3) {
            printf("Tentative d'inscription: nombre invalide d'arguments\n");
            printf("Usage: register <username> <password>\n");

            return "error";
        } else {
            return parseRegister(words);
        }
    } else if (strcmp(words[0], "list") == 0) {
        if (i != 2 || (strcmp(words[1], "ingame") != 0 && strcmp(words[1], "available") != 0)) {

            printf("Usage: list <ingame> or <available>\n");

            return "error";
        } else {
            cJSON *request = cJSON_CreateObject();

            cJSON *command = cJSON_CreateString("list");
            cJSON *listType = cJSON_CreateString(words[1]);

            cJSON_AddItemToObject(request, "command", command);
            cJSON_AddItemToObject(request, "listType", listType);

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
    } else if (strncmp(words[0], "chat", 4) == 0) {
        if (i < 3) {
            printf("Tentative de chat: nombre invalide d'arguments\n");
            printf("Usage: chat <username> [message]\n");
            return "error";
        } else {

            int argumentsLength = (int) (strlen(words[0]) + strlen(words[1]) + 2);

            char subtext[200-argumentsLength];

            // Using memcpy to extract a substring
            memcpy(subtext, &bufferCopy[argumentsLength], 199-argumentsLength);
            subtext[199-argumentsLength] = '\0';  // Null-terminate the substring

            return parseChat(words, subtext);
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
            printf("Refus de challenge: nombre invalide d'arguments\n");
            printf("Usage: refuse\n");
            return "error";
        } else {
            return refuseChallenge();
        }
    } else if (strncmp(words[0], "play", 4) == 0) {
        if (i != 2) {
            printf("Tentative de jouer un coup: nombre invalide d'arguments\n");
            printf("Usage: play <case>\n");
            return "error";
        } else {
            return play((int) strtol(words[1], NULL, 10));
        }
    } else if (strncmp(words[0], "quit", 4) == 0) {
        if (i != 1) {
            printf("Tentative de quitter: nombre invalide d'arguments\n");
            printf("Usage: quit\n");
            return "error";
        } else {
            quit();
        }
    } else if (strncmp(words[0], "help", 4) == 0) {
        printHelpMessage();
        return NULL;
    } else if (strncmp(words[0], "disconnect", 10) == 0) {
        if (i != 1) {
            printf("Tentative de déconnexion: nombre invalide d'arguments\n");
            printf("Usage: disconnect\n");
            return "error";
        } else {

            return disconnect();

        }
    } else if (strncmp(words[0], "surrender", 9) == 0) {
        if (i != 1) {
            printf("Tentative d'abandon: nombre invalide d'arguments\n");
            printf("Usage: surrender\n");
            return "error";
        } else {

            return surrender();

        }
    }

    printf("Commande inconnue...\n");
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

jsonString parseRegister(char *args[]) {
    cJSON *request = cJSON_CreateObject();

    cJSON *command = cJSON_CreateString("register");
    cJSON *username = cJSON_CreateString(args[1]);
    cJSON *password = cJSON_CreateString(args[2]);

    cJSON_AddItemToObject(request, "command", command);
    cJSON_AddItemToObject(request, "username", username);
    cJSON_AddItemToObject(request, "password", password);

    return cJSON_Print(request);

}

jsonString parseChat(char *args[], char *message){
    cJSON *request = cJSON_CreateObject();

    cJSON *command = cJSON_CreateString("chat");
    cJSON *username = cJSON_CreateString(args[1]);
    cJSON *jsonMessage = cJSON_CreateString(message);

    cJSON_AddItemToObject(request, "command", command);
    cJSON_AddItemToObject(request, "username", username);
    cJSON_AddItemToObject(request, "message", jsonMessage);

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

void printHelloMessage(){
    printf("Bonjour et Bienvenue dans le serveur de jeux Awale en ligne !\n\n");
    printf("Veuillez vous connecter : login <username> <password>\n");
    printf("Ou bien vous enregistrer : register <username> <password>\n");
}

void printHelpMessage(){
    printf("Voici la liste des commandes que vous pouvez utiliser :\n"
           "\n"
           "- login <pseudo> <motdepasse>\n"
           "    -> pour vous login au serveur à l'aide d'un pseudo et mot de passe.\n"
           "- register <username> <password>\n"
           "    -> pour créer un compte assocé à un pseudo et mot de passe.\n"
           "- list <ingame> or <available>\n"
           "    -> pour lister les utilisateurs en fonction de leur status.\n"
           "    list ingame renverra les utilisateurs actuellement en jeu, tandis que list available renverra les utilisateurs disponibles.\n"
           "- chat <username> [message]\n"
           "    -> permet d'envoyer un message à un utilisateur donné. exemple : chat adrien bonjour, comment vas-tu ?\n"
           "- challenge <username>\n"
           "    -> permet de challenger un utilisateur, c'est à dire, lui proposer de jouer contre vous.\n"
           "- accept\n"
           "    -> pour accepter un challenge et lancer une partie.\n"
           "- refuse\n"
           "    -> pour refuser un challenge.\n"
           "- play <case>\n"
           "    -> permet de jouer une case\n"
           "- quit\n"
           "    -> pour quitter une partie\n"
           "- disconnect\n"
           "    -> pour se déconnecter du serveur\n"
           "- surrender\n"
           "    -> pour terminer la partie en se déclarant perdant.\n");
}

void processResponse(cJSON *response, int *connected) {

    if (response == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
    }

    cJSON *command = cJSON_GetObjectItemCaseSensitive(response, "command");
    cJSON *status = cJSON_GetObjectItemCaseSensitive(response, "status");

    char *commandString = cJSON_GetStringValue(command);
    char *statusString = cJSON_GetStringValue(status);

    if (strncmp(statusString, "error", 5) == 0) {

        char *errorMessage = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(response, "message"));

        printf("Erreur : %s\n", errorMessage);
        return;
    }

    if (strncmp(commandString, "login", 5) == 0) {

        cJSON *message = cJSON_GetObjectItemCaseSensitive(response, "message");

        char *messageString = cJSON_GetStringValue(message);


        if (strncmp(statusString, "success", 7) == 0) {
            *connected = TRUE;
            printf("Vous êtes connecté au serveur !\n");
        } else {
            printf("Erreur lors de la connexion: %s\n",messageString);
        }
    }


    if (strncmp(commandString, "challenge", 9) == 0) {


        char *opponentString = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(response, "opponent"));

        char *message = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(response, "message"));

        if (strncmp(statusString, "success", 7) == 0) {
            if(strncmp(statusString, "Vous avez reçu une demande de challenge de", 42) == 0){
                printf("%s %s !\n Tappez 'accept' pour accepter ou 'refuse' pour refuser.\n", message, opponentString);
            } else {
                printf("%s %s !\n", message, opponentString);
            }

        } else {
            printf("%s\n", message);
        }
    }

    if (strncmp(commandString, "register", 8) == 0) {

        char *message = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(response, "message"));

        printf("%s : %s\n", statusString, message);
    }

    if (strncmp(commandString, "chat", 4) == 0) {

        char *from = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(response, "from"));

        char *message = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(response, "message"));

        if (from != NULL){
            if (strncmp(statusString, "success", 7) == 0) {
                printf("Message de %s : %s\n", from, message);
            } else {
                printf("erreur de communication chat avec %s\n", from);
            }
        }
    }

    if (strncmp(commandString, "list", 4) == 0) {
        cJSON *playersArray = cJSON_GetObjectItemCaseSensitive(response, "players");

        int i, arraySize;

        if (playersArray == NULL || playersArray->type != cJSON_Array) {
            fprintf(stderr, "Format JSON invalide ou argument manquant dans la liste des joueurs.\n");
            return;
        }

        arraySize = cJSON_GetArraySize(playersArray);

        printf("Liste des joueurs :\n");
        for (i = 0; i < arraySize; i++) {
            cJSON *player = cJSON_GetArrayItem(playersArray, i);

            cJSON *username = cJSON_GetObjectItem(player, "username");

            if (username && username->type == cJSON_String) {
                printf("- %s\n", username->valuestring);
            } else {
                fprintf(stderr, "Erreur de format JSON à l'élément numéro %d.\n", i);
            }
        }

        if (i==0){
            printf("Aucun joueur...\n");
        }
    }

    if (strncmp(commandString, "accept", 6) == 0) {

        char *message = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(response, "message"));


        if (strncmp(statusString, "success", 7) == 0) {
            printf("Lancement du jeu ...\n");
        } else {
            printf("%s\n", message);
        }
    }

    if (strncmp(commandString, "refuse", 6) == 0) {
        char *message = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(response, "message"));

        if (strncmp(statusString, "success", 7) == 0) {
            char *opponentName = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(response, "opponent"));
            printf("%s a refusé votre challenge !\n", opponentName);
        } else {

            printf("%s\n", message);
        }
    }

    if (strncmp(commandString, "game started", 10) == 0) {

        char *joueur1Name = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(response, "joueur1"));
        char *joueur2Name = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(response, "joueur2"));
        char *quiJoueName = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(response, "quiJoue"));

        if (strncmp(statusString, "success", 7) == 0) {
            lancerPartie(joueur1Name, joueur2Name, quiJoueName);
        } else {
            printf("error\n");
        }
    }

    if (strncmp(commandString, "play", 4) == 0) {
        updateGameState(p, *response);
    }

    if (strncmp(commandString, "disconnect", 10) == 0) {

        if (strncmp(statusString, "success", 7) == 0) {
            *connected = FALSE;
            inGame = FALSE;
            printf("Vous êtes déconnecté du serveur\n");

        } else {
            printf("error\n");
        }

    }

    if (strncmp(commandString, "surrender", 9) == 0) {

        if (strncmp(statusString, "success", 7) == 0) {
            *connected = FALSE;
            inGame = FALSE;
            printf("Vous avez abandonné la partie\n");

        } else {
            printf("%s", cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(response, "message")));
        }

    }

    if (strncmp(commandString, "game end", 8) == 0) {
        cJSON *winOrLose = cJSON_GetObjectItemCaseSensitive(response, "state");

        char *winOrLoseString = cJSON_GetStringValue(winOrLose);

        if (strncmp(statusString, "success", 7) != 0) {
            perror("Erreur lors de la fin de jeu");
            return;
        }

        inGame = FALSE;
        printf("La partie est terminée\n");

        if (strncmp(winOrLoseString, "win", 3) == 0) {
            printf("Vous avez gagné !\n");
        } else if (strncmp(winOrLoseString, "lose", 4) == 0) {
            printf("Vous avez perdu...\n");
        } else if (strncmp(winOrLoseString, "draw", 4) == 0) {
            printf("Vous avez fait match nul !\n");
        }

        finirPartie(p);
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

void lancerPartie(char *playerName, char *opponentName, char *quiJoueName) {
    joueur *player = initialiserJoueur(playerName);
    joueur *opponent = initialiserJoueur(opponentName);

    p = initialiserPartie(player, opponent);

    if (strcmp(quiJoueName, player->nomUtilisateur) == 0) {
        p->quiJoue = player;
    } else {
        p->quiJoue = opponent;
    }

    afficherPartie(p);
    inGame = TRUE;
}

jsonString play(int caseChoisie) {

    // build the response string to send to the server
    cJSON *response = cJSON_CreateObject();

    cJSON *command = cJSON_CreateString("play");
    cJSON *caseChoisieJson = cJSON_CreateNumber(caseChoisie);

    cJSON_AddItemToObject(response, "command", command);
    cJSON_AddItemToObject(response, "caseChoisie", caseChoisieJson);

    return cJSON_Print(response);

}

void updateGameState(partie *partie, cJSON response) {

    cJSON *status = cJSON_GetObjectItemCaseSensitive(&response, "status");

    char *statusString = cJSON_GetStringValue(status);

    cJSON *state = cJSON_GetObjectItemCaseSensitive(&response, "state");

    char *stateString = cJSON_GetStringValue(state);

    if (strncmp(statusString, "success", 7) == 0) {
        if (strncmp(stateString, "valid", 5) == 0) {
            jouerCoup(partie, partie->quiJoue->nomUtilisateur,
                      (int) cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(&response, "caseChoisie")));
            afficherPartie(partie);
        }

    } else {
        printf("error\n");
    }
}

jsonString disconnect() {
    cJSON *request = cJSON_CreateObject();

    cJSON *command = cJSON_CreateString("disconnect");
    cJSON_AddItemToObject(request, "command", command);
    return cJSON_Print(request);

}

void quit() {
    exit(0);
}

jsonString surrender() {

    cJSON *request = cJSON_CreateObject();

    cJSON *command = cJSON_CreateString("surrender");

    cJSON_AddItemToObject(request, "command", command);

    return cJSON_Print(request);
}

