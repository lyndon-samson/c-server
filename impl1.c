#include "stdio.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
// --------------
static int num ;

int server_start(int argc, char *argv[]) {
  num    = atoi(argv[1]);
  printf("Users have to guess this %d\n",num);
}

int server_end() { printf("The user guessed the number, exiting server\n");}

char *server_prompt(void (*sendstr)(char *msg)) {
  static char buff[100];
  strcpy(buff,"Guess the number:");

  sendstr(buff);

  return buff;
}

int server_logic(char *buff, char *resp) {
  int run = 1, guess = atoi(buff);

  if ( guess == num ) {
     strcpy(resp, "Correct\n");
     run = 0;
  } else if ( guess < num )
     strcpy(resp, "Try again, it is higher\n");
  else
     strcpy(resp, "Try again, it is lower\n");

  return run;
}

int server_loop(void (*sendstr)(char *msg), int(readinput)(char *b, int bsz) ) {
  server_prompt(sendstr);

  char buff[100], resp[100];
  int n,run = 1;

  n = readinput(buff, 100);
  printf("Client Sent:%s\n",buff);
  run = server_logic(buff, resp);
  sendstr(resp);

  return run;
}

