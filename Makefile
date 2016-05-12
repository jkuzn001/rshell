CC=g++

CFLAGS =-c -Wall -Werror -ansi -pedantic
all: rshell

rshell: main.o Connector.o Cmd.o AND.o OR.o Semicolon.o Exit.o
	$(CC) main.o Connector.o Cmd.o AND.o OR.o Semicolon.o Exit.o -o rshell

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

Connector.o: Connector.cpp
	$(CC) $(CFLAGS) Connector.cpp

Cmd.o: Cmd.cpp
	$(CC) $(CFLAGS) Cmd.cpp

AND.o: AND.cpp Connector.cpp
	$(CC) $(CFLAGS) AND.cpp

OR.o: OR.cpp Connector.cpp
	$(CC) $(CFLAGS) OR.cpp

Semicolon.o: Semicolon.cpp Connector.cpp
	$(CC) $(CFLAGS) Semicolon.cpp

Exit.o: Exit.cpp Cmd.cpp
	$(CC) $(CFLAGS) Exit.cpp


clean:
	rm -rf *o rshell
