//
// Created by Adrien MORIN on 28/11/2023.
//

#include "csvWriter.h"
#include "../../lib/libcsv/src/csv.h"
#include <stdio.h>
#include <string.h>


int addRowToCSV(const char *filename, const char *username, const char *password) {
    FILE *file = fopen("data/users.csv", "a");  // Ouvrir le fichier en mode ajout ("a")

    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return -1;
    }
    char row[100]= "\n";
    strcat(row, username);
    strcat(row, ",");
    strcat(row, password);

    fputs(row, file);
    fclose(file);
    return 1;
}
