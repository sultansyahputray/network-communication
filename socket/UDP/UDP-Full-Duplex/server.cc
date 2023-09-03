#include "udp.hh"

int main() {
  char *ip = "127.0.0.1"; // local
  int port1 = 3838; // Port untuk mengirim data ke klien
  int port2 = 3939; // Port untuk menerima data dari klien

  int sockfd_send, sockfd_receive;
  struct sockaddr_in addr_send, addr_receive;
  char buffer_send[sizeof(custom)];
  char buffer_receive[sizeof(custom)];
  socklen_t addr_size;

  // Socket untuk mengirim data ke klien
  sockfd_send = socket(AF_INET, SOCK_DGRAM, 0);
  memset(&addr_send, '\0', sizeof(addr_send));
  addr_send.sin_family = AF_INET;
  addr_send.sin_port = htons(port1);
  addr_send.sin_addr.s_addr = inet_addr(ip);

  // Socket untuk menerima data dari klien
  sockfd_receive = socket(AF_INET, SOCK_DGRAM, 0);
  memset(&addr_receive, '\0', sizeof(addr_receive));
  addr_receive.sin_family = AF_INET;
  addr_receive.sin_port = htons(port2);
  addr_receive.sin_addr.s_addr = INADDR_ANY;

  // Bind socket menerima ke alamat yang tepat
  if (bind(sockfd_receive, (struct sockaddr*)&addr_receive, sizeof(addr_receive)) < 0) {
    perror("[-]bind error");
    exit(1);
  }

  int broadcast = 1;
  if (setsockopt(sockfd_send, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0) {
    perror("[-]setsockopt error");
    exit(1);
  }

  while (true) {
    custom.value1 = 10;
    custom.value2 = 3.14;
    custom.value3 = true;

    memcpy(buffer_send, &custom, sizeof(custom));

    if (sendto(sockfd_send, buffer_send, sizeof(custom), 0, (struct sockaddr*)&addr_send, sizeof(addr_send)) < 0) {
      perror("[-]error sending data");
    } else {
      printf("[+]Data send: %i, %f, %i\n", custom.value1, custom.value2, custom.value3);
    }

    // Menerima data dari klien
    bzero(buffer_receive, sizeof(custom));
    addr_size = sizeof(addr_receive);
    recvfrom(sockfd_receive, buffer_receive, sizeof(custom), 0, (struct sockaddr*)&addr_receive, &addr_size);
    memcpy(&custom, buffer_receive, sizeof(custom));

    printf("[+]Data received from client: %i, %f, %i\n", custom.value1, custom.value2, custom.value3);

    this_thread::sleep_for(chrono::seconds(1));
  }

  return 0;
}
