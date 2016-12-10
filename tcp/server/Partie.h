#ifndef H_PARTIE
#define H_PARTIE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "server.h"
#include "client.h"

typedef struct game game;
struct game
{
	char* name;
	int nmbr_player;
	char** map;
	struct Client tabplayer[5];
	struct game *nxt;
};
 
typedef game* llist;

llist ajouterEnTete(llist liste, char*  name);
void afficherListe(llist liste);
int number_game(llist liste);
llist find_game(llist liste, char* name_game);
llist join_game(llist liste,char* name_game, Client player);
int my_rand(int a, int b);


#endif
