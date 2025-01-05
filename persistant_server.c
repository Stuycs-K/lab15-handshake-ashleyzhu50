#include "pipe_networking.h"
#include "signal.h"

static void sighandler(int signo){
  if(signo==2){
    remove("./myWKP");
    exit(1);
  }
}

int main() {
  while(1){
    int to_client;
    int from_client;
    from_client = server_handshake( &to_client );
    signal(2,sighandler);
    close(from_client);
    close(to_client);
  }
}
