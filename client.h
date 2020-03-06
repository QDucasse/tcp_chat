#ifndef CLIENT_H
#define CLIENT_H

#include "tools.h"

/* Gives the client a default behavior in case of Ctrl-C */
void handle_shutdown_client(int sig);

/* Allows a client to connect to a host/port with error handling*/
void connect_client(int socket_d,  struct hostent *host, int port);

/* Handler of the chat between client and server */
void chat(int socket_d);

#endif //CLIENT_H
