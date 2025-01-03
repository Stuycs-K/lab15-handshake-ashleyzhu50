all: client server
client: basic_client.o persistant_server.o
	gcc -o client basic_client.o persistant_server.o

server: basic_server.o persistant_server.o
	gcc -o server basic_server.o pipe_networking.o

basic_client.o: basic_client.c pipe_networking.h
	gcc -c basic_client.c

basic_server.o: basic_server.c pipe_networking.h
	gcc -c basic_server.c

pipe_networking.o: persistant_server.c pipe_networking.h
	gcc -c persistant_server.c

clean:
	rm *.o
	rm *~