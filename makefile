compile:
	gcc -Wall -g3 -fsanitize=address -pthread ./src/server.c -o ./bin/server
	gcc -Wall -g3 -fsanitize=address -pthread ./src/client.c -o ./bin/client
FLAGS    = -L /lib64
LIBS     = -lusb-1.0 -l pthread

