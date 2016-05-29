CC= g++

CFLAGS = -Wall -Werror -ansi -pedantic

all:
	mkdir -p ./bin
	$(CC) $(CFLAGS) ./src/main.cpp -o ./bin/rshell

main:
	$(CC) $(CFLAGS) ./src/main.cpp

Connector:
	$(CC) $(CFLAGS) ./src/Connector.cpp

Cmd:
	$(CC) $(CFLAGS) ./src/Cmd.cpp

AND:
	$(CC) $(CFLAGS) ./src/AND.cpp

OR:
	$(CC) $(CFLAGS) ./src/OR.cpp

Semicolon:
	$(CC) $(CFLAGS) ./src/Semicolon.cpp

Exit:
	$(CC) $(CFLAGS) ./src/Exit.cpp

Test:
	$(CC) $(CFLAGS) ./src/Test.cpp

clean:
	rm -rf ./bin
