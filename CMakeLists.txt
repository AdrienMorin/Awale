cmake_minimum_required(VERSION 3.26)
project(Awale C)

set(CMAKE_C_STANDARD 11)

add_executable(Awale
        client.c
        server_chat.c
        "Client Serveur V2/Serveur/client2.h"
        "Client Serveur V2/Serveur/server2.c"
        "Client Serveur V2/Client/client2.c")
