#include "stdio.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
// --------------
int init(struct sockaddr_in *soca, int socl, short int port) {
  int soc = socket(AF_INET, SOCK_STREAM, 0);

  soca->sin_family = AF_INET;
  soca->sin_addr.s_addr=htonl(INADDR_ANY);
  soca->sin_port=htons(port);

  int bnd = bind(soc, (struct sockaddr *)soca, socl);
  int lis = listen(soc , 5);

  return soc;
}
// --------------
void sendstr(int soc, char *msg) { send(soc, msg, strlen(msg),0); }
  // --------------
int readinput(int acc, char *b, int bsz) {
  int n;
  n = recv(acc, b, bsz, 0);

  return n;

}
// --------------
int run(int argc, char *argv[]) {
  struct sockaddr_in soca;
  struct sockaddr    soca_c;
  socklen_t          socl_c = sizeof(soca_c) ;
  short              port   = 8888;
  int                run    = 1;
  int                soc    = init(&soca, (socklen_t)sizeof(soca), port);

  int (*server_start_impl)(int argc, char *argv[]);
  int (*server_end_impl)();
  int (*server_loop)(void (*sendstr)(char *msg), int(readinput)(char *b, int bsz));

  printf("Impl %s\n",argv[1]);

  void *handle      = dlopen(argv[1],RTLD_LAZY);
  server_start_impl = dlsym(handle,"server_start");
  server_end_impl   = dlsym(handle,"server_end");
  server_loop       = dlsym(handle,"server_loop");

  server_start_impl(argc, &argv[1]);

  while ( run ) {
    printf("The server is waiting for an inbound connection on port %d\n", port);
    int acc = accept(soc, &soca_c, &socl_c);
    printf("Opened an inbound connection from %s\\n",inet_ntoa(((struct sockaddr_in *)&soca_c)->sin_addr));

    void sendstr2(char *msg) { sendstr(acc, msg); }
    int readinput2(char *b, int bsz) { return readinput(acc, b, bsz); }

    run = (*server_loop)(&sendstr2, &readinput2);
    close(acc);

  }
  server_end_impl();

  close(soc);
  dlclose(handle);
}

int main(int argc, char *argv[]) {
  run(argc, argv);
}

