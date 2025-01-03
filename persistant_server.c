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
    signal(SIGINT, sighandler);
//   while(1){
    int from_client=server_setup();
    char pp[256]; 
    read(from_client, pp, 256); // read SYN 
    // printf("got SYN: %s \n", pp);
    int * sack=malloc(sizeof(int));
    int r_file = open("/dev/random", O_RDONLY,0);
    read(r_file, sack, sizeof(int)); // get random int
    *sack = abs(*sack)% 101;
    // printf("sending SYN_ACK: %d\n", *sack);
    * to_client = open(pp, O_WRONLY,0); // open PP
    write(*to_client, sack, sizeof(int)); // send SYN_ACK
    int * ack=malloc(sizeof(int));
    read(from_client, ack, sizeof(int));
    // printf("got ACK: %d \n", *ack);
    return from_client;
//   }
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
  printf("my PP: %s\n", privfifo);
  write(*to_server, privfifo, 256); // write PP to WKP
  int readPPFD = open(privfifo, O_RDONLY, 0644); // open PP
  remove(privfifo); // delete PP
  int * ack=malloc(sizeof(int));
  read(readPPFD, ack, sizeof(int)); // read SYN_ACK
  printf("got SYN_ACK! %d\n", *ack); 
  * ack += 1;
  printf("sending ACK: %d \n", *ack);
  write(*to_server, ack, sizeof(int)); // send ACK
  printf("wrote it!\n");
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

// static void sighandler(int signo){
//     if(signo == SIGINT){
//         remove("./myWKP");
//     }
// }