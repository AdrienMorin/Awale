//
// Created by Nour on 27/11/2023.
//

#ifndef AWALE_ACTIONMANAGER_H
#define AWALE_ACTIONMANAGER_H

#include "../joueur.h"
#include "../../lib/cJSON/cJSON.h"
#include "../partie.h"


typedef char *jsonString;


jsonString parseRequest(char *buffer);

jsonString parseLogin(char *args[]);

jsonString parseChallenge(char *args[]);

jsonString acceptChallenge();

jsonString refuseChallenge();

void lancerPartie(char *playerName, char *opponentName, char *quiJoueName);

jsonString play(int caseChoisie);

void updateGameState(partie *partie, cJSON response);

jsonString quit();

void processResponse(cJSON *response, int *connected);
#endif //AWALE_ACTIONMANAGER_H
