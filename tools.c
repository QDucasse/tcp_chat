#include "tools.h"

/* Correct error handling */
void error(char *msg) {
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(1);
} //error

/* Catches a signal sent to the executable */
int catch_signal(int sig, void (*handler)(int)) {
	struct sigaction action;
	action.sa_handler = handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	return sigaction (sig, &action, NULL);
} //catch_signal

/* Opens the socket with error handling */
int open_socket() {
	int socket_d = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_d == -1)
		error("Cannot open socket");
	return socket_d; //returns a socket descriptor
} //open_socket
