#include "udp.hh"

int main(){

  // char *ip = "192.168.1.31"; //ip broadcast
  char *ip = "127.0.0.1"; //local
  int port = 3838;

  int sockfd;
  struct sockaddr_in addr;
  char buffer[sizeof(custom)];
  socklen_t addr_size;

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  memset(&addr, '\0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip);

  int broadcast = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0) {
    perror("[-]setsockopt error");
    exit(1);
  }

  while (true) {
    custom.value1 = 10;
    custom.value2 = 3.14;
    custom.value3 = true;

    memcpy(buffer, &custom, sizeof(custom));

    if (sendto(sockfd, buffer, sizeof(custom), 0, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
      perror("[-]error sending data");
    }

    else
      printf("[+]Data send: %i, %f, %i\n", custom.value1, custom.value2, custom.value3);
    
    this_thread::sleep_for(chrono::seconds(1));
  }

  return 0;
}