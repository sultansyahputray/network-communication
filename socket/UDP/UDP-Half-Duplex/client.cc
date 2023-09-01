#include <stdio.h>
#include <stdlib.h>
#include "udp.hh"

int main(){

  int port = 3838;

  int sockfd;
  struct sockaddr_in server_addr, client_addr;
  char buffer[sizeof(custom)];
  socklen_t addr_size;

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0){
    perror("[-]socket error");
    exit(1);
  }

  int reuse = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
    perror("[-]setsockopt error");
    exit(1);
  }

  memset(&server_addr, '\0', sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    perror("[-]bind error");
    exit(1);
  }
  
  while (true) {
    bzero(buffer, sizeof(custom));
    addr_size = sizeof(server_addr);
    recvfrom(sockfd, buffer, sizeof(custom), 0, (struct sockaddr*)&server_addr, &addr_size);
    memcpy(&custom, buffer, sizeof(custom));

    printf("[+]Data recv: %i, %f, %i\n", custom.value1, custom.value2, custom.value3);
  }

  return 0;
}