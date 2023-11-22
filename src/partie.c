#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "partie.h"

partie* initialiserPartie(){

    partie* p = malloc(sizeof(partie));
    p->joueur1 = initialiserJoueur("morin");
    p->joueur2 = initialiserJoueur("mohamed");
    p->quiJoue = choisirJoueurAleatoire(p->joueur1, p->joueur2);
    // tirer au sort qui joue en premier aleatoirement

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

void jouerCoup(partie* p, char* nomJoueur, int caseChoisie){
    if(!coupValide(p, nomJoueur, caseChoisie)){
        return;
    }

    int caseArrivee = egrener(p, p->quiJoue, caseChoisie);

    p->quiJoue = (p->quiJoue == p->joueur1) ? p->joueur2 : p->joueur1;

    printf("Case arrivee : %d\n", caseArrivee);

    char * auTourDe = (p->quiJoue == p->joueur1) ? p->joueur1->nomUtilisateur : p->joueur2->nomUtilisateur;

    printf("Au tour de : %s", auTourDe);

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

    return caseArrivee;
}

int coupValide(partie *p, char* nomJoueur, int caseChoisie) {

    if(strcmp(nomJoueur, p->quiJoue->nomUtilisateur) != 0){
        printf("Ce n'est pas a votre tour de jouer\n");
        return FALSE;
    }

    if((p->quiJoue == p->joueur1 && (caseChoisie < 0 || caseChoisie > 5))
       || (p->quiJoue == p->joueur2 && (caseChoisie < 6 || caseChoisie > 11))){
        printf("Vous ne pouvez jouer que les cases de votre côté\n");
        return FALSE;
    }

    if(p->plateau[caseChoisie] == 0){
        printf("Vous ne pouvez pas jouer une case vide\n");
        return FALSE;
    }

    return TRUE;
}

int main(){
    partie* p = initialiserPartie();
    afficherPlateau(p->plateau);

    printf("Au tour de : ");
    afficherJoueur(p->quiJoue);

    while(TRUE){
        char nomJoueur[50];
        int caseChoisie;

        printf("Entrez le nom du joueur et la case choisie séparés par un espace : ");

        if(scanf("%49s %d", nomJoueur, &caseChoisie) != 2){
            printf("Erreur de saisie\n");
        }
        else{
            jouerCoup(p, nomJoueur, caseChoisie);
            afficherPlateau(p->plateau);
        }

    }

    finirPartie(p);
    return 0;
}
