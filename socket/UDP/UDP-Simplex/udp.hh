#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <chrono>
#include <thread>

using namespace std;

struct data_custom {
  int value1;
  float value2;
  bool value3;
};

data_custom custom;

// note :
// we try to send and receive data with custom type of data. 
// so we can custamize it whatever we need