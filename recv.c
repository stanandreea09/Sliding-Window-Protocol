//Stan Mihaela-Andreea
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "link_emulator/lib.h"

#define HOST "127.0.0.1"
#define PORT 10001

int main(int argc,char** argv){
  msg r;
  init(HOST,PORT);
  int numar_cadre;
  int  sent, received, i=0;
  int file;

  received = recv_message(&r);
  if (received < 0){
    perror("Receive message");
    return -1;
  }
  numar_cadre = r.len;
  file = open("recv_fileX", O_WRONLY | O_TRUNC | O_CREAT, 0666);
  sent = send_message(&r);
  if(sent < 0){
    perror("[RECEIVER] Receiver error.\n");
    return -1;
  }

 for(i=0; i<numar_cadre; i++){

    received = recv_message(&r);
    if(received > 0){
      write(file, r.payload, r.len);
      sent = send_message(&r);
      if(sent < 0){
        perror("[RECEIVER] Receiver error.\n");
        return -1;
      }
    }else {
      perror("[RECEIVER] Receive error. Exiting.\n");
      return -1;
    }
      
 } 
  close(file);
  return 0;
}
