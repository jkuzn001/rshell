CC=g++

CFLAGS =-c -Wall -Werror -ansi -pedantic
all: rshell

rshell:
	main Connector Cmd AND OR Semicolon Exit
	$(CC) main Connector Cmd AND OR Semicolon Exit -o rshell

main:
	main.cpp
	$(CC) $(CFLAGS) main.cpp

Connector:
	Connector.cpp
	$(CC) $(CFLAGS) Connector.cpp

Cmd:
	Cmd.cpp
	$(CC) $(CFLAGS) Cmd.cpp

AND:
	AND.cpp Connector.cpp
	$(CC) $(CFLAGS) AND.cpp

OR:
	OR.cpp Connector.cpp
	$(CC) $(CFLAGS) OR.cpp

Semicolon:
	Semicolon.cpp Connector.cpp
	$(CC) $(CFLAGS) Semicolon.cpp

Exit:
	Exit.cpp Cmd.cpp
	$(CC) $(CFLAGS) Exit.cpp

clean:
	rm -rf *o rshell
