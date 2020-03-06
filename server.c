#include "server.h"

// ===================================================
// 						BIND LISTEN ACCEPT TALK
// ===================================================

int listener_d;
int connect_d;

/* Binds the socket to the port with error handling */
void bind_to_port(int socket, int port) {
	struct sockaddr_in name;
	name.sin_family = AF_INET;
	name.sin_port = (in_port_t)htons(port);
	name.sin_addr.s_addr = htonl(INADDR_ANY);
	int reuse = 1;
	if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int)) == -1)
		error("Cannot set the reuse option on the socket");
	//The reuse option allows us to reboot the server immediately if shut down
	//The lack of it would make us wait for 30 seconds for the port to be cleaned and made free
	int c = bind (socket, (struct sockaddr *) &name, sizeof(name));
	if (c == -1)
		error("Cannot bind to socket");
} //bind_to_port

/* Places the socket in listen position with a given queue length and error handling*/
void listen_socket(int listener_d,int queue){
	if(listen(listener_d,queue)==-1)
		error("Cannot listen");
} //listen_socket

/* Accepts a connection on the socket given by the socket descriptor listener_d */
int accept_connection(int listener_d,struct sockaddr_storage client_addr,unsigned int address_size){
	int connect_d = accept(listener_d, (struct sockaddr *)&client_addr, &address_size);
		//Wait until a client contacts the server
		//Create a secondary socket descriptor to hold the conversation with the client
	if (connect_d == -1)
		error("Cannot open secondary socket");
	return connect_d; //Returns the secondary socket descriptor
} //accept_connection

void handle_shutdown_server(int sig) {
	//Closes the socket in case of use of Ctrl-C
	if (listener_d)
		close(listener_d);
	fprintf(stderr, "Socket closed!\n");
	exit(0);
} //handle_shutdown_server

/* Handler of the chat between client and server */
void chat_serv(int socket_d) {
    char buf_serv[255];
    int n;
    // infinite loop for chat
    while(1) {
        bzero(buf_serv, 255);

        // read the message from client and copy it in buffer
        read(socket_d, buf_serv, sizeof(buf_serv));
        // print buffer which contains the client contents
        printf("From client: %s\t To client : ", buf_serv);
        bzero(buf_serv, 255);
        n = 0;
        // copy server message in the buffer
        while ((buf_serv[n++] = getchar()) != '\n');

        // and send that buffer to client
        write(socket_d, buf_serv, sizeof(buf_serv));

        // if msg contains "Exit" then server exit and chat ended.
        if (strncmp("exit", buf_serv, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }
}

/* Handler of the operations between client and server */
void func_serv(int socket_d) {
    char buf_serv[255];
    // infinite loop for chat
    while(1) {
        bzero(buf_serv, 255);
        // read the message from client and copy it in buffer
        read(socket_d, buf_serv, sizeof(buf_serv));
        // print buffer which contains the client contents
        printf("From client: %s\n", buf_serv);
        bzero(buf_serv, 255);
    }
}

// ===================================================
// 												MAIN
// ===================================================

int main(int argc, char *argv[]) {
	//Runs the code handle_shutdown_server if Ctrl-C is used
	if (catch_signal(SIGINT, handle_shutdown_server) == -1)
		error("Cannot set the interrupt handler");

	//Initializing the ocean
	//ocean *oc = initialize_ocean(10,10,3,1);
	if (argc != 2)
		error("Wrong number of arguments, please specify the port");
	//Port we will be using
	int port = atoi(argv[1]);

	//Open
	listener_d = open_socket();
	puts("Socket opened!");

	//Bind
	bind_to_port(listener_d, port);
	puts("Socket bounded!");

	//Listen
	listen_socket(listener_d,8);
	puts("Waiting for incoming connections...");
	/*Variable definition:
	  client_addr ---- client address
	  address_size --- size of the address
	  buf ------------ buffer where the server will read messages from the clients */
	struct sockaddr_storage client_addr;
	unsigned int address_size = sizeof(client_addr);

	//Loop the wait
	int connect_d = accept(listener_d, (struct sockaddr *)&client_addr,&address_size);
	if (connect_d == -1) error("Can't open secondary socket!");
	puts("Client connection accepted");

	// Launch the chat application
	func_serv(connect_d);

	return 0;
} //main()
