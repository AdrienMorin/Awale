//
// Created by Adrien MORIN on 28/11/2023.
//

#include "csvWriter.h"
#include <stdio.h>

int addRowToCSV(const char *filename, const char *row) {
    FILE *file = fopen("data/users.txt", "wt");  // Ouvrir le fichier en mode ajout ("a")

    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return -1;
    }

    fputs(row, file);
    fclose(file);
    return 1;
}
