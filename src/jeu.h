//
// Created by Nour on 25/11/2023.
//

#ifndef AWALE_JEU_H
#define AWALE_JEU_H
#include "partie.h"

joueur * connecterJoueur(partie * p, int socket);
// Le client envoie une requete de connexion au serveur avec un
// username et un password. Le serveur verifie si le joueur existe grâce
// à un csv. Il verifie si le joueur est déjà connecté. Il verifie si le
// joueur est déjà dans une partie.
// et si le mot de passe est correct. Si oui, il renvoie un pointeur
// vers le joueur, sinon il renvoie NULL

#endif //AWALE_JEU_H
