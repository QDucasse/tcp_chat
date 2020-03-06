CC=gcc
OBJFILES = tools.o server.o client.o
TARGETS = client server

all: $(TARGETS)

client: client.o tools.o
	$(CC) -Wall -g client.o tools.o -o client

server: server.o tools.o
	$(CC) -Wall -g server.o tools.o -o server

client.o: client.c client.h tools.o
	$(CC) -Wall -g -c client.c

server.o: server.c server.h tools.o
	$(CC) -Wall -g -c server.c

tools.o: tools.c tools.h
	$(CC) -Wall -g -c tools.c

clean:
	$(RM) $(TARGETS) $(OBJFILES)
