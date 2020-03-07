//Stan Mihaela-Andreea
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "link_emulator/lib.h"

#define HOST "127.0.0.1"
#define PORT 10000

int main(int argc,char** argv){
  init(HOST,PORT);
  msg t;
  int i, sent, received;
  int speed = atoi(argv[2]);//speed
  int delay = atoi(argv[3]);//delay

  int dim_fereastra= (1000*speed*delay)/(sizeof(msg)*8);
  int filename;
  int len;
  int numar_cadre;
  filename = open("fileX", O_RDONLY);
  len = lseek(filename, 0, SEEK_END); //lungimea fisierului

  numar_cadre = len / MSGSIZE;
  if(len%MSGSIZE != 0){
    numar_cadre++;
  }

  memset(&t,0,sizeof(msg));
  t.len = numar_cadre;
  strcpy(t.payload, "recv_fileX");
  sent = send_message(&t);
  if(sent < 0){
    perror("[SENDER] Send error.\n" );
    return -1;
  }
  received = recv_message(&t);
  if(received < 0){
    perror("[SENDER] Receive error.\n");
    return -1;
  }

  len = lseek(filename, 0, SEEK_SET);

  if(numar_cadre > dim_fereastra){  
    for(i=0; i < dim_fereastra; i++){
      t.len = read(filename, t.payload, MSGSIZE);
      sent = send_message(&t);
        if (sent < 0) {
          perror("[SENDER] Send error. Exiting.\n");
          return -1;
        }
    }

    for(i = 0; i<(numar_cadre-dim_fereastra); i++){
      received = recv_message(&t);
      if(received < 0){
        perror("[SENDER] Receive error.\n");
        return -1;
      }
      t.len = read(filename, t.payload, MSGSIZE);
      printf("trimis %d\n", i);
      sent = send_message(&t);
      if (sent < 0){
          perror("[SENDER] Send error. Exiting.\n");
          return -1;
        }
    }

    for(i = 0; i < dim_fereastra; i++ ){
      received = recv_message(&t);
      if(received < 0){
        perror("[SENDER] Receive error.\n");
        return -1;
      }
    }
  } else {
    for(i=0; i < numar_cadre; i++){
      t.len = read(filename, t.payload, MSGSIZE);
      sent = send_message(&t);
        if (sent < 0) {
          perror("[SENDER] Send error. Exiting.\n");
          return -1;
        }
    }
    for(i = 0; i < numar_cadre; i++ ){
      received = recv_message(&t);
      if(received < 0){
        perror("[SENDER] Receive error.\n");
        return -1;
      }
    }

  }

  printf("[SENDER] Job done, all sent.\n");
  close(filename);


  return 0;
}
