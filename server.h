#ifndef SERVER_H
#define SERVER_H

#include "tools.h"

/* Binds the socket to the port with error handling */
void bind_to_port(int socket, int port);

/* Places the socket in listen position with a given queue length and error handling*/
void listen_socket(int listener_d,int queue);

/* Accepts a connection on the socket given by the socket descriptor listener_d */
int accept_connection(int listener_d,struct sockaddr_storage client_addr,unsigned int address_size);

/* Closes the socket in case of Ctrl-C */
void handle_shutdown_server(int sig);

/* Handler of the chat between client and server */
void chat_serv(int sockfd);

#endif //SERVER_H
