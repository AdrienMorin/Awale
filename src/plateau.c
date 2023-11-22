#include "plateau.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>


void initialiserPlateau(int *plateau)
{
    for (int i = 0; i < NB_CASES; ++i) {
        plateau[i] = 4;
    }
}

void afficherPlateau(int *plateau)
{
    printf("  0 1 2 3 4 5\n");
    printf(" ____________\n");
    printf("|%d|%d|%d|%d|%d|%d|\n", plateau[11], plateau[10], plateau[9], plateau[8], plateau[7], plateau[6]);
    printf("|%d|%d|%d|%d|%d|%d|\n", plateau[0], plateau[1], plateau[2], plateau[3], plateau[4], plateau[5]);
    printf("|__|_|_|_|_|_|__|\n");
    printf("  5 4 3 2 1 0\n");
}