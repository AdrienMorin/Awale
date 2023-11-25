#ifndef AWALE_JOUEUR_H
#define AWALE_JOUEUR_H

#define JOUEUR1 1
#define JOUEUR2 2

typedef struct joueur
{
    int cases[6];
    char nomUtilisateur[20];
    int nbGraines;

} joueur;

joueur* initialiserJoueur(char* nom);

void initialiserCases(joueur* j, int numeroJoueur);

void afficherJoueur(joueur* j);

#endif //AWALE_JOUEUR_H
