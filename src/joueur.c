//
// Created by Nour on 22/11/2023.
//

#include "joueur.h"
#include <malloc.h>
#include <string.h>

joueur* initialiserJoueur(char* nom){
    joueur* j = malloc(sizeof(joueur));
    strcpy(j->nomUtilisateur, nom);
    j->nbGraines = 0;
    return j;
}

void afficherJoueur(joueur *j) {
    printf("Joueur %s : %d graines\n", j->nomUtilisateur, j->nbGraines);
}
