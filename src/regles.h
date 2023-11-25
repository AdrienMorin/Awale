#ifndef AWALE_REGLES_H
#define AWALE_REGLES_H
#include "partie.h"

int adversaireNonAffame(partie* p, joueur* j, int caseChoisie);

int finParFamine(partie* p);
// Si un joueur n'a plus de graines, la partie se termine si son adversaire
// n'a pas assez de graines pour le nourrir

int finParScore(partie* p);
// La partie se termine si un joueur a plus de 25 graines

int joueurVide(partie* p, joueur* j);

#endif //AWALE_REGLES_H
