#include "pipe_networking.h"
//UPSTREAM = to the server / from the client
//DOWNSTREAM = to the client / from the server
/*=========================
  server_setup

  creates the WKP and opens it, waiting for a  connection.
  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
  int from_client = 0;
  char * path = "./myWKP";
  mkfifo(path, 0640);
  open(path, O_RDONLY, 0644);
  remove(path);
  char * privfifo;
  sprintf(privfifo,"%d", getpid());
  mkfifo(privfifo, 0640);
  return open(privfifo);
}

/*=========================
  server_handshake 
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe (Client's private pipe).

  returns the file descriptor for the upstream pipe (see server setup).
  =========================*/
int server_handshake(int *to_client) {
  int from_client;
  char * privfifo; 
  sprintf(privfifo,"%d", getpid());
  mkfifo(privfifo, 0640); // make PP
  char * path = "./myWKP";
  mkfifo(path, 0640);
  open(path, O_WRONLY, 0644); // open WKP
  char buff[256];
  read(server_setup, buff, 255);
  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  int from_server;
  return from_server;
}


/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {
  int to_client  = 0;
  return to_client;
}


