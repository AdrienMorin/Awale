#include "regles.h"
#include "utils.h"

int adversaireNonAffame(partie* p, joueur* j, int caseChoisie){

    if(!joueurVide(p, j)){
        return TRUE;
    }

    int * plateau = p->plateau;
    joueur * adversaire = (j == p->joueur1) ? p->joueur2 : p->joueur1;

    for (int i = 0; i < plateau[caseChoisie]; ++i) {
        int casei = (caseChoisie + i + 1) % 12;

        if (contains(adversaire->cases, 6, casei)) {
            return TRUE;
        }
    }

    return FALSE;

}

int joueurVide(partie* p, joueur* j){
    int * plateau = p->plateau;

    for (int i : j->cases) {
        if (plateau[i] != 0) {
            return FALSE;
        }
    }

    return TRUE;
}

int finParFamine(partie *p) {
    joueur * joueurAffame = (joueurVide(p, p->joueur1)) ? p->joueur1 : p->joueur2;

    joueur * adversaire = (joueurAffame == p->joueur1) ? p->joueur2 : p->joueur1;

    for(int i = 0 ; i < 6; i++){
        if(adversaire->cases[i] >= 6 - i) return FALSE;
    }

    return TRUE;

}

int finParScore(partie *p) {
    return p->joueur1->nbGraines >= 25 || p->joueur2->nbGraines >= 25;
}
