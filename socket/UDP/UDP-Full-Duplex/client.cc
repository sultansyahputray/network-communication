#include <stdio.h>
#include <stdlib.h>
#include "udp.hh"

int main() {
  int port1 = 3838; // Port untuk menerima data dari server
  int port2 = 3939; // Port untuk mengirim data ke server

  int sockfd_send, sockfd_receive;
  struct sockaddr_in server_addr, client_addr;
  char buffer_send[sizeof(custom)];
  char buffer_receive[sizeof(custom)];
  socklen_t addr_size;

  // Socket untuk menerima data dari server
  sockfd_receive = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd_receive < 0) {
    perror("[-]socket error");
    exit(1);
  }

  int reuse = 1;
  if (setsockopt(sockfd_receive, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
    perror("[-]setsockopt error");
    exit(1);
  }

  memset(&server_addr, '\0', sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port1);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(sockfd_receive, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    perror("[-]bind error");
    exit(1);
  }

  // Socket untuk mengirim data ke server
  sockfd_send = socket(AF_INET, SOCK_DGRAM, 0);
  memset(&client_addr, '\0', sizeof(client_addr));
  client_addr.sin_family = AF_INET;
  client_addr.sin_port = htons(port2);
  client_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Ganti dengan IP server jika perlu

  while (true) {
    // Menerima data dari server
    bzero(buffer_receive, sizeof(custom));
    addr_size = sizeof(server_addr);
    recvfrom(sockfd_receive, buffer_receive, sizeof(custom), 0, (struct sockaddr*)&server_addr, &addr_size);
    memcpy(&custom, buffer_receive, sizeof(custom));

    printf("[+]Data received from server: %i, %f, %i\n", custom.value1, custom.value2, custom.value3);

    // Mengirim data ke server
    custom.value1 = 42;
    custom.value2 = 2.71828;
    custom.value3 = false;

    memcpy(buffer_send, &custom, sizeof(custom));

    if (sendto(sockfd_send, buffer_send, sizeof(custom), 0, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0) {
      perror("[-]error sending data");
    } else {
      printf("[+]Data send to server: %i, %f, %i\n", custom.value1, custom.value2, custom.value3);
    }
  }

  return 0;
}
