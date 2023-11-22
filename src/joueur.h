#ifndef AWALE_JOUEUR_H
#define AWALE_JOUEUR_H

typedef struct joueur
{
    char nomUtilisateur[20];
    int nbGraines;

} joueur;

joueur* initialiserJoueur(char* nom);

void afficherJoueur(joueur* j);

#endif //AWALE_JOUEUR_H
