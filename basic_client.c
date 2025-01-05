#include "pipe_networking.h"
#include "signal.h"
static void sighandler(int signo){
  if(signo==2){
    // remove("./myWKP");
    exit(0);
  }
}
int main() {

  int to_server;
  int from_server;
  signal(2,sighandler);
  from_server = client_handshake( &to_server );
}
