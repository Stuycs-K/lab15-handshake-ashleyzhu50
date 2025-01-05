#include "pipe_networking.h"
#include "signal.h"
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
  mkfifo(path, 0640); // create WKP
  from_client = open(path, O_RDONLY, 0644); // open WKP 
  remove(path); //remove WKP
  return from_client; // return upstream WKP
}

/*=========================
  server_handshake 
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe (Client's private pipe).

  returns the file descriptor for the upstream pipe (see server setup).
  =========================*/
int server_handshake(int *to_client) {
  int from_client=server_setup();
  char pp[256]; 
  read(from_client, pp, 256); // read SYN 
  // printf("got SYN: %s \n", pp);
  int * sack=malloc(sizeof(int));
  int * ack=malloc(sizeof(int));
  int r_file = open("/dev/random", O_RDONLY,0);
  * to_client = open(pp, O_WRONLY,0); // open PP
  while(1){
    read(r_file, sack, sizeof(int)); // get random int
    *sack = abs(*sack)% 101;
    // printf("sending SYN_ACK: %d\n", *sack);
    write(*to_client, sack, sizeof(int)); // send SYN_ACK 
    if(read(from_client, ack, sizeof(int))<=0){
      break;
    }
    // printf("got ACK: %d \n", *ack);
    sleep(1);
    signal(SIGPIPE,SIG_IGN);
  }
  exit(0);
  close(*to_client);
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
  char privfifo[256]; 
  sprintf(privfifo,"%d", getpid());
  mkfifo(privfifo, 0640); // make PP
  *to_server = open("./myWKP", O_WRONLY, 0644); // open WKP
  // printf("my PP: %s\n", privfifo);
  write(*to_server, privfifo, 256); // write PP to WKP
  int * ack=malloc(sizeof(int));
  from_server = open(privfifo, O_RDONLY, 0644); // open PP
  remove(privfifo); // delete PP
  while(read(from_server, ack, sizeof(int))>0){
  // read(from_server, ack, sizeof(int)); // read SYN_ACK
  printf("%d\n", *ack); 
  * ack += 1;
  // printf("sending ACK: %d \n", *ack);
  write(*to_server, ack, sizeof(int)); // send ACK
  // printf("wrote it!\n");
  }
  return from_server;
}


/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int *to_client, int from_client) {
  char pp[256]; 
  read(from_client, pp, 256); // read SYN 
  // printf("got SYN: %s \n", pp);
  int * sack=malloc(sizeof(int));
  int * ack=malloc(sizeof(int));
  int r_file = open("/dev/random", O_RDONLY,0);
  * to_client = open(pp, O_WRONLY,0); // open PP
  while(1){
    read(r_file, sack, sizeof(int)); // get random int
    *sack = abs(*sack)% 101;
    // printf("sending SYN_ACK: %d\n", *sack);
    write(*to_client, sack, sizeof(int)); // send SYN_ACK 
    if(read(from_client, ack, sizeof(int))<=0){
      break;
    }
    // printf("got ACK: %d \n", *ack);
    sleep(1);
    signal(SIGPIPE,SIG_IGN);
  }
  close(*to_client);
  return from_client;
}