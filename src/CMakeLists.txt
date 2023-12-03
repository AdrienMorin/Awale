project(Source)

add_subdirectory(Server)

add_subdirectory(Client)


add_library(awale STATIC
        joueur.c joueur.h
        plateau.c plateau.h
        partie.c regles.c partie.h
        utils.c utils.h
)


target_compile_options(server PRIVATE -lm)