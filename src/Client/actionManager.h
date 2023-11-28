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

void processResponse(cJSON *response, int *connected);
#endif //AWALE_ACTIONMANAGER_H
