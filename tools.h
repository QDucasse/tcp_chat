#ifndef TOOLS_H
#define TOOLS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>

/* Correct error handling */
void error(char *msg);

/* Catches a signal sent to the executable */
int catch_signal(int sig, void (*handler)(int));

/* Opens the socket with error handling */
int open_socket();

/* Sends a message with error handling */
int say(int socket, char *s);

/* Treats recv to a buffer, this way recv is ensured to give back the whole message */
int read_in(int socket, char *buf, int len);

#endif //TOOLS_H
