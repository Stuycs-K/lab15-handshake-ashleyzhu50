#include "pipe_networking.h"
#include "signal.h"
#include <unistd.h>

static void sighandler(int signo){
  if(signo==2){
    remove("./myWKP");
    exit(0);
  }
  if(signo==0){
    exit(0);
  }
}

int main() {
  while(1){
    int to_client;
    int from_client=server_setup();
    if(fork()==0){
        from_client=server_connect(&to_client, from_client);
        close(to_client);
        close(from_client);
        signal(0,sighandler);
        exit(0);
    }
    signal(2,sighandler);
  }
}
