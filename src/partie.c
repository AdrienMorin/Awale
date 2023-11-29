#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "partie.h"

partie *initialiserPartie(joueur *j1, joueur *j2) {

    partie* p = malloc(sizeof(partie));

    p->joueur1 = j1;
    p->joueur2 = j2;

    initialiserCases(j1, JOUEUR1);
    initialiserCases(j2, JOUEUR2);

    initialiserPlateau(p->plateau);

    return p;
}

joueur* choisirJoueurAleatoire(joueur* j1, joueur* j2){
    // Seed the random number generator with the current time
    srand(time(NULL));

    // Generate a random number (0 or 1) to choose between the two values
    int randomIndex = rand() % 2;

    // Return the randomly chosen value
    return (randomIndex == 0) ? j1 : j2;
}

int jouerCoup(partie *p, char *nomJoueur, int caseChoisie) {
    if(!coupValide(p, nomJoueur, caseChoisie)){
        return FALSE;
    }

    int caseArrivee = egrener(p, p->quiJoue, caseChoisie);

    manger(p, p->quiJoue, caseArrivee);



    p->quiJoue = (p->quiJoue == p->joueur1) ? p->joueur2 : p->joueur1;

    return TRUE;
}

void finirPartie(partie* p){
    free(p);
}

int egrener(partie *p, joueur *j, int caseChoisie) {

    int graines = p->plateau[caseChoisie];
    p->plateau[caseChoisie] = 0;

    for(int i = 0; i < graines; i++){
        // si on revient à la case choisie, on la saute
        if((caseChoisie + i + 1) % 12 == caseChoisie){
            ++graines;
            continue;
        }

        p->plateau[(caseChoisie + i + 1) % 12]++;

    }

    int caseArrivee = (caseChoisie + graines) % 12;

    p->nbCoups++;

    return caseArrivee;
}

void manger(partie *p, joueur *j, int caseArrivee) {

    int nbGrainesMangees = 0;

    // Tant qu'on tombe sur une case avec 2 ou 3 graines, on mange, puis on passe à la case précédente
    while( p->plateau[caseArrivee] == 2 || p->plateau[caseArrivee] == 3 ){

        nbGrainesMangees += p->plateau[caseArrivee];

        p->plateau[caseArrivee] = 0;

        caseArrivee = (caseArrivee - 1) % 12;

    }

    j->nbGraines += nbGrainesMangees;

}

int coupValide(partie *p, char* nomJoueur, int caseChoisie) {

    if(strcmp(nomJoueur, p->quiJoue->nomUtilisateur) != 0){
        perror("Ce n'est pas a votre tour de jouer\n");
        return FALSE;
    }

    if((p->quiJoue == p->joueur1 && (caseChoisie < 0 || caseChoisie > 5))
       || (p->quiJoue == p->joueur2 && (caseChoisie < 6 || caseChoisie > 11))){
        perror("Vous ne pouvez jouer que les cases de votre côté\n");
        return FALSE;
    }

    if(p->plateau[caseChoisie] == 0){
        perror("Vous ne pouvez pas jouer une case vide\n");
        return FALSE;
    }

    return TRUE;
}

void afficherPartie(partie *p) {

    afficherJoueur(p->joueur2);
    printf("\n");

    afficherPlateau(p->plateau);

    afficherJoueur(p->joueur1);

    printf("\n");

    // print the name of the player whose turn it is, bold format only the name of the player, and make it purple
    printf("\033[1;35m%s\033[0m, c'est a vous de jouer !\n", p->quiJoue->nomUtilisateur);

}

int partieTerminee(partie* p){
    return finParFamine(p) || finParScore(p);
}
