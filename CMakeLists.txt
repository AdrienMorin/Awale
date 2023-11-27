cmake_minimum_required(VERSION 3.15)

project(Awale C)

set(CMAKE_C_STANDARD 11)

add_executable(Awale
        src/partie.c
        src/plateau.c
        src/joueur.c
        src/utils.c
        src/utils.h
        src/regles.c
        src/jeu.c
        src/jeu.h)
