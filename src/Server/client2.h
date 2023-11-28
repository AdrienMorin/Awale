#ifndef CLIENT_H
#define CLIENT_H

#include "server2.h"
#include "../partie.h"

typedef struct
{
   SOCKET sock;
    joueur *j;
   char name[BUF_SIZE];
}Client;

#endif /* guard */
