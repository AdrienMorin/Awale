//
// Created by Nour on 22/11/2023.
//

#include <stdlib.h>
#include <time.h>
#include "utils.h"
#include "regles.h"

int contains(const int* tab, int taille, int element){
    for(int i = 0; i < taille; i++){
        if(tab[i] == element){
            return TRUE;
        }
    }
    return FALSE;
}