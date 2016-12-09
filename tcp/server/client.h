#ifndef CLIENT_H
#define CLIENT_H

#include "server.h"

typedef struct Client Client;
struct Client
{
    SOCKET sock;
    char name[BUF_SIZE];
};

#endif /* guard */
