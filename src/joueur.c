//
// Created by Nour on 22/11/2023.
//

#include "joueur.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

joueur* initialiserJoueur(char* nom){
    joueur* j = malloc(sizeof(joueur));
    strcpy(j->nomUtilisateur, nom);
    j->nbGraines = 0;
    return j;
}

void initialiserCases(joueur* j, int numeroJoueur){
    for(int i = 0; i < 6; i++){
        j->cases[i] = (numeroJoueur == JOUEUR1) ? i : 11 - i;
    }
}

void afficherJoueur(joueur *j) {
    printf("%s : %d graines\n", j->nomUtilisateur, j->nbGraines);
}

joueur *copierJoueur(joueur *j) {

    joueur *j2 = malloc(sizeof(joueur));

    strcpy(j2->nomUtilisateur, j->nomUtilisateur);

    j2->nbGraines = j->nbGraines;

    for (int i = 0; i < 6; i++) {
        j2->cases[i] = j->cases[i];
    }
    return j2;
}