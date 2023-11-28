//
// Created by Nour on 27/11/2023.
//

#ifndef AWALE_ACTIONMANAGER_H
#define AWALE_ACTIONMANAGER_H
typedef char *jsonString;


jsonString parseRequest(char *buffer);

jsonString parseLogin(char *args[]);

jsonString parseChallenge(char *args[]);
#endif //AWALE_ACTIONMANAGER_H
