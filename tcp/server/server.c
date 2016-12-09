#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "server.h"
#include "client.h"
#include "Partie.h"

static void init(void)
{
#ifdef WIN32
   WSADATA wsa;
   int err = WSAStartup(MAKEWORD(2, 2), &wsa);
   if(err < 0)
   {
      puts("WSAStartup failed !");
      exit(EXIT_FAILURE);
   }
#endif
}

static void end(void)
{
#ifdef WIN32
   WSACleanup();
#endif
}

static void app(void)
{
   SOCKET sock = init_connection();
   char buffer[BUF_SIZE];
   char command[BUF_SIZE];
   char crtgame[BUF_SIZE];
   char nbgames[BUF_SIZE];
   char nbplayers[5];
   /* the index for the array */
   int actual = 0;
   int max = sock;
   /* an array for all clients */
   Client clients[MAX_CLIENTS];
   /* game list init */
   llist gamelist = NULL;
   llist gamejoin = NULL;
   char map[NB_ROWS][NB_COLS];

   

   char* token;

   fd_set rdfs;

   while(1)
   {
      int i = 0;
      FD_ZERO(&rdfs);

      /* add STDIN_FILENO */
      FD_SET(STDIN_FILENO, &rdfs);

      /* add the connection socket */
      FD_SET(sock, &rdfs);

      /* add socket of each client */
      for(i = 0; i < actual; i++)
      {
         FD_SET(clients[i].sock, &rdfs);
      }

      if(select(max + 1, &rdfs, NULL, NULL, NULL) == -1)
      {
         perror("select()");
         exit(errno);
      }

      /* something from standard input : i.e keyboard */
      if(FD_ISSET(STDIN_FILENO, &rdfs))
      {
         /* stop process when type on keyboard */
         break;
      }
      else if(FD_ISSET(sock, &rdfs))
      {
         /* new client */
         SOCKADDR_IN csin = { 0 };
         socklen_t sinsize = sizeof csin;
         int csock = accept(sock, (SOCKADDR *)&csin, &sinsize);
         if(csock == SOCKET_ERROR)
         {
            perror("accept()");
            continue;
         }

         memset(buffer, 0, BUF_SIZE);
         /* after connecting the client sends its name */
         if(read_client(csock, buffer) == -1)
         {
            /* disconnected */
            continue;
         }

         /* what is the new maximum fd ? */
         max = csock > max ? csock : max;

         FD_SET(csock, &rdfs);

         Client c = { csock };
         strncpy(c.name, buffer, BUF_SIZE - 1);
         clients[actual] = c;
         actual++;
         buffer[BUF_SIZE - 1] = '\0';
         memset(buffer, 0, BUF_SIZE);
      }
      else
      {
         int i = 0;
         strncpy(command, "require game list", BUF_SIZE - 1);
         int nbgame = number_game(gamelist);
         strncpy(crtgame, "number of game ", BUF_SIZE - 1);
         sprintf(nbgames, "%d", nbgame);
         strncat(crtgame, nbgames, BUF_SIZE - strlen(buffer) - 1);
         strncat(crtgame, "\n", BUF_SIZE - strlen(buffer) - 1);
         for(i = 0; i < actual; i++)
         {
            /* a client is talking */
            if(FD_ISSET(clients[i].sock, &rdfs))
            {
               Client client = clients[i];
               int c = read_client(clients[i].sock, buffer);
               /* client disconnected */
               buffer[c - 1] = '\0';
               if(c == 0)
               {
                  closesocket(clients[i].sock);
                  remove_client(clients, i, &actual);
                  strncpy(buffer, client.name, BUF_SIZE - 1);
                  strncat(buffer, " disconnected !", BUF_SIZE - strlen(buffer) - 1);
                  //send_message_back(clients, client, actual, buffer, 1);
                  puts("disconnected");
               }
               else
               {
                  if (strcmp(buffer, command) == 0)
                  {
                     int nbgame = number_game(gamelist);
                     strncpy(crtgame, "number of games ", BUF_SIZE - 1);
                     sprintf(nbgames, "%d", nbgame);
                     strncat(crtgame, nbgames, BUF_SIZE - strlen(buffer) - 1);
                     strncat(crtgame, "\n", BUF_SIZE - strlen(buffer) - 1);
                     send_message_back(clients, client, actual, crtgame);
                     llist tmp;
                     tmp = gamelist;

                     while(tmp != NULL)
                     {
                        char gname[BUF_SIZE];
                        char nbp[BUF_SIZE];
                        int var;
                        var = tmp->nmbr_player;
                        printf("*****players-in-game%d\n",var );
                        strncpy(nbp, "players-in-game-", BUF_SIZE - 1);
                        sprintf(nbplayers, "%d", var);
                        strncat(nbp, nbplayers, BUF_SIZE - strlen(buffer) - 1);
                        strncat(nbp, " ", BUF_SIZE - strlen(nbp) - 1);
                        send_message_back(clients, client, actual, nbp);
                        strncat(nbp, gname, sizeof(nbp) - strlen(buffer) - 1);
                        strncpy(gname, "game-name-", BUF_SIZE - 1);
                        strncat(gname, tmp->name, sizeof(gname) - strlen(buffer) - 1);
                        strncat(gname, "\n", sizeof(gname) - strlen(buffer) - 1);
                        send_message_back(clients, client, actual, gname);
                        tmp = tmp->nxt;
                     }
                  }
                  else if (strstr(buffer, "create game"))
                  {
                     token = strtok(buffer, " ");
                     int j = 0;
                     while(j < 2)
                     {
                        token = strtok(NULL, " ");
                        j++;
                     }
                     llist tmpFind;
                     tmpFind = find_game(gamelist, token);
                     if (tmpFind == NULL)
                     {
                       puts("game found");
                     }
                     gamelist = ajouterEnTete(gamelist, token, clients[actual - 1]);
                     Client cli;
                     cli = gamelist->tabplayer[0];
                     int nbgame = number_game(gamelist);
                     if(nbgame > 0)
                     {
                        send_message_back(clients, client, actual, "game created\n");
                     }
                     else
                     {
                        send_message_back(clients, client, actual, "cannot create game\n");
                     }
                  }
                  else if (strstr(buffer, "join game"))
                  {
                     token = strtok(buffer, " ");
                     int j = 0;
                     while(j < 2)
                     {
                        token = strtok(NULL, " ");
                        j++;
                     }
                     gamejoin=join_game(gamelist, token, clients[actual - 1]);
                     nbgame = number_game(gamelist);

                     if (gamejoin != NULL)
                     {
                        char header[BUF_SIZE];
                        char nbr[BUF_SIZE];
                        char nbc[BUF_SIZE];
                        sprintf(nbr, "%d", NB_ROWS);
                        sprintf(nbc, "%d", NB_COLS);
                        strcpy(header, "map nb-rows-");
                        strncat(header, nbr, sizeof(header) - strlen(header) - 1);
                        strncat(header, " nb-cols-", sizeof(header) - strlen(header) - 1);
                        strncat(header, nbc, sizeof(header) - strlen(header) - 1);
                        strncat(header, "\n", sizeof(header) - strlen(header) - 1);
                        printf("game joined   %s\n", gamejoin->name);
                        send_message_back(clients, client, actual, header);
                        /* Try to open the file */
                         FILE *fp = fopen("map.txt", "r");
                         if (!fp)
                         {
                             fprintf(stderr, "Unable to open %s: %s\n",
                                     "map.txt", strerror(errno));
                             exit(-1);
                         }

                         /* Our array of lines is actually a pointer to an array of character        
                            pointers, one for each line, so keep track of the number of lines. */
                         char **lines = NULL;
                         size_t lineCount = 0;
                         char *line = NULL;
                         while ((line = readMap(fp)) != NULL)
                         {
                             char **temp = realloc(lines, sizeof(*lines) * lineCount + 1);
                             if (!temp)
                             {
                                 fprintf(stderr, "Unable to realloc for lines!\n");
                                 /* Cleanup the existing memory */
                                 size_t i = 0;
                                 for (; i < lineCount; ++i)
                                 {
                                     free(lines[i]);
                                 }
                                 free(lines);
                                 fclose(fp);
                                 exit(-1);
                             }
                             /* Assign our lines variable to point to the reallocated memory */
                             lines = temp;
                             /* Set our newly allocated pointer to point to the line returned */
                             lines[lineCount++] = line;
                         }
				
			int x = my_rand(0,NB_ROWS);
			int y = my_rand(0,NB_COLS);
			lines[x][y]='0';
				

                         /* Print our lines and clean up */
                         for (int j = 0; j < lineCount; j++)
                         {
                             //printf("%s\n", lines[j]);
                           char* mapline = malloc(sizeof(line[j] + 1));
                           strcpy(mapline, lines[j]);
                           strcat(mapline, "\n");
                           send_message_back(clients, client, actual, mapline);
                             /* Free the line's memory allocated in the readLine function */
                             free(lines[j]);
                         }

                         /* Free the array that held the lines */
                         free(lines);

                         fclose(fp);
                     }
                     else
                     {
                        send_message_back(clients, client, actual, "cannot join game\n");
                     }
                  }
                  else
                  {
                     puts("no compar");
                     send_message_back(clients, client, actual, buffer);
                  }
               }
               break;
            }
         }
      }
   }

   clear_clients(clients, actual);
   end_connection(sock);
}

static void clear_clients(Client *clients, int actual)
{
   int i = 0;
   for(i = 0; i < actual; i++)
   {
      closesocket(clients[i].sock);
   }
}

static void remove_client(Client *clients, int to_remove, int *actual)
{
   /* we remove the client in the array */
   memmove(clients + to_remove, clients + to_remove + 1, (*actual - to_remove - 1) * sizeof(Client));
   /* number client - 1 */
   (*actual)--;
}

static void send_message_back(Client *clients, Client sender, int actual, const char *buf)
{
   int i = 0;
   char message[BUF_SIZE];
   message[0] = 0;
   for(i = 0; i < actual; i++)
   {
      /* we send back to client message to the sender */
      if(sender.sock == clients[i].sock)
      {
         strncat(message, buf, sizeof message - strlen(message) - 1);
         write_client(clients[i].sock, message);
         printf("--message to send--%s--\n", message);
      }
   }
}

static void send_chat_message(Client *clients, Client sender, int actual, const char *buf)
{
   int i = 0;
   char message[BUF_SIZE];
   message[0] = 0;
   for(i = 0; i < actual; i++)
   {
      /* we don't send message to the sender */
      if(sender.sock != clients[i].sock)
      {
         strncpy(message, sender.name, BUF_SIZE - 1);
         strncat(message, sender.name, sizeof message - strlen(message) - 1);
         strncat(message, buf, sizeof message - strlen(message) - 1);
         write_client(clients[i].sock, message);
      }
   }
}

static int init_connection(void)
{
   SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
   SOCKADDR_IN sin = { 0 };

   if(sock == INVALID_SOCKET)
   {
      perror("socket()");
      exit(errno);
   }

   sin.sin_addr.s_addr = htonl(INADDR_ANY);
   sin.sin_port = htons(PORT);
   sin.sin_family = AF_INET;

   if(bind(sock,(SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR)
   {
      perror("bind()");
      exit(errno);
   }

   if(listen(sock, MAX_CLIENTS) == SOCKET_ERROR)
   {
      perror("listen()");
      exit(errno);
   }

   return sock;
}

static void end_connection(int sock)
{
   closesocket(sock);
}

static int read_client(SOCKET sock, char *buffer)
{
   int n = 0;

   if((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0)
   {
      perror("recv()");
      /* if recv error we disonnect the client */
      n = 0;
   }

   buffer[n] = 0;

   return n;
}

static void write_client(SOCKET sock, const char *buffer)
{
   if(send(sock, buffer, strlen(buffer), 0) < 0)
   {
      perror("send()");
      exit(errno);
   }
}

char *readMap(FILE *fp)
{
    if (!fp)
        return NULL;
    char buffer[BUF_SIZE] = { 0 };
    char *line = NULL;
    if (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        size_t len = strlen(buffer);
        if (buffer[len - 1] == '\n')
        {
            buffer[len - 1] = '\0';
            line = malloc(len);
            if (!line)
            {
                fclose(fp);
                fprintf(stderr, "Could not allocate memory!\n");
                exit(-1);
            }
            strcpy(line, buffer);
            return line;
        }
        line = malloc(strlen(buffer) + 1);
        strcpy(line, buffer);
        int done = 0;
        while (!done && fgets(buffer, sizeof(buffer), fp) != NULL)
        {
            len = strlen(buffer);
            if (buffer[len - 1] == '\n')
            {
                buffer[len - 1] = '\0';
                done = 1;
            }
            char *temp = realloc(line, strlen(line) + len + 1);
            if (!temp)
            {
                free(line);
                fclose(fp);
                fprintf(stderr, "Could not reallocate memory!\n");
                exit(-1);
            }
            line = temp;
            strcat(line, buffer);
        }
    }
    return line;
}

int main(int argc, char **argv)
{
   init();

   app();

   end();

   return EXIT_SUCCESS;
}
