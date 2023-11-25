#ifndef AWALE_PARTIE_H
#define AWALE_PARTIE_H
#include "plateau.h"
#include "joueur.h"

#define TRUE 1
#define FALSE 0

typedef struct partie {
    int plateau[12];
    joueur* joueur1; // peut jouer les cases 0 à 5
    joueur* joueur2; // peut jouer les cases 6 à 11
    int nbCoups;
    joueur* quiJoue;
} partie;

partie* initialiserPartie();

joueur* choisirJoueurAleatoire(joueur* j1, joueur* j2);

void jouerCoup(partie* p, char* nomJoueur, int caseChoisie);

int coupValide(partie* p, char* nomJoueur, int caseChoisie);

int adversaireNonAffame(partie *p, joueur *j, int caseChoisie);

int finParFamine(partie *p);
// Si un joueur n'a plus de graines, la partie se termine si son adversaire
// n'a pas assez de graines pour le nourrir

int finParScore(partie *p);
// La partie se termine si un joueur a plus de 25 graines

int joueurVide(partie *p, joueur *j);

int egrener(partie* p, joueur* j, int caseChoisie);

void manger(partie* p, joueur* j, int caseArrivee);

void afficherPartie(partie* p);

int partieTerminee(partie* p);

void finirPartie(partie* p);


#endif //AWALE_PARTIE_H
