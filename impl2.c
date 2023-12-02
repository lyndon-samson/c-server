#include "stdio.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
// --------------
static int op1 ;
static int op2 ;
static char operator ;

int server_start(int argc, char *argv[]) {
  op1 = atoi(argv[1]);
  op2 = atoi(argv[3]);
  operator = argv[2][0];
  printf("Users have to answer the math question correctly (%d %c %d)\n",op1,operator,op2);
}

int server_end() { printf("The user gave a correct answer\n");}

char *server_prompt(void (*sendstr)(char *msg)) {
  static char buff[100];
  sprintf(buff,"%d %c %d = ",op1,operator,op2);

  sendstr(buff);

  return buff;
}

int server_logic(char *buff, char *resp) {
  int run = 1, guess = atoi(buff);
  int answer;

  if ( operator == '*' ) answer = op1 * op2;
  if ( operator == '/' ) answer = op1 / op2;
  if ( operator == '+' ) answer = op1 + op2;
  if ( operator == '-' ) answer = op1 - op2;

  if ( guess == answer ) {
     strcpy(resp, "Correct\n");
     run = 0;
  } else
     strcpy(resp, "Try again\n");

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
