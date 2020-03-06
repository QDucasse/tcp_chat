#include "client.h"

int socket_d;

/* Allows a client to connect to a host/port with error handling*/
void connect_client(int socket_d,  struct hostent *host, int port){
	struct sockaddr_in adr;
	memset(&adr, 0, sizeof(adr));
	adr.sin_family = AF_INET;
	adr.sin_port = htons(port);
	bcopy(host->h_addr, &adr.sin_addr.s_addr, host->h_length);
	int res = connect(socket_d, (struct sockaddr *) &adr, sizeof(adr));
	if (res==-1){
		error("Cannot connect to the socket");
	}
};

/* Gives the client a default behavior in case of Ctrl-C */
void handle_shutdown_client(int sig) {
	//Closes the socket in case of use of Ctrl-C
	if (socket_d)
		close(socket_d);
	fprintf(stderr, "Client socket closed!\n");
	exit(0);
} //handle_shutdown

/* Handler of the chat between client and server */
void chat_client(int socket_d) {
    char buf_cli[255];
		int n;
		// Infinite loop
    while(1) {
        bzero(buf_cli, sizeof(buf_cli));
        printf("Enter the string : ");
        n = 0;
        while ((buf_cli[n++] = getchar()) != '\n'); // Get the message in the buffer
        write(socket_d, buf_cli, sizeof(buf_cli));     // Write it to the socket
        bzero(buf_cli, sizeof(buf_cli));							 // Reinitialize the buffer
        read(socket_d, buf_cli, sizeof(buf_cli));      // Read the answer
        printf("From Server : %s", buf_cli);

        if ((strncmp(buf_cli, "exit", 4)) == 0) { // If the message is exit, the client exits
            printf("Client Exit...\n");
            break;
        }
    }
}


/* Handler of random time + sending number */
void func_client(int socket_d) {
		char buf_cli[255];
		int random_time, random_number;
		while(1) {
        bzero(buf_cli, sizeof(buf_cli));
				random_time = rand()%5 + 1;
				printf("Random time to wait: %d\n", random_time);
				sleep(random_time);
				random_number = rand()%10 + 1;
				printf("Random number to send: %d\n", random_number);
				sprintf(buf_cli, "%d",random_number);

        write(socket_d, buf_cli, sizeof(buf_cli));     // Write it to the socket
        bzero(buf_cli, sizeof(buf_cli));							 // Reinitialize the buffer
    }
}


int main(int argc, char *argv[]) {
	//Runs the code handle_shutdown_client if Ctrl-C is used
	if (catch_signal(SIGINT, handle_shutdown_client) == -1)
		error("Cannot set the interrupt handler");

	if (argc != 3)
		error("Wrong number of arguments, please specify the ip and port");

	//Argument storage
	struct hostent *host=gethostbyname(argv[1]);
	int port=atoi(argv[2]);

	//Open the socket
	socket_d = open_socket();
	puts("Socket open");

	// Connect to the server
	puts("Connection...");
	printf("Port: %d\n",port);
	connect_client(socket_d, host, port);

	// Launch the chatt application
	// chat_client(socket_d);
	func_client(socket_d);

} //main()
