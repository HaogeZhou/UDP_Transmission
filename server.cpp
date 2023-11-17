#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <cstring>
#define SIZE 1024

void write_file(int sockfd, sockaddr_in& addr, const char *filename)
{
  // char* filename = "received.bin";
  int n;
  char buffer[SIZE];
  socklen_t addr_size = sizeof(addr);

  // Creating a file.
  std::ofstream file(filename);

  // Receiving the data and writing it into the file.
  while (true)
  {
    n = recvfrom(sockfd, buffer, SIZE, 0, (struct sockaddr*)&addr, &addr_size);

    if (strcmp(buffer, "END") == 0)
    {
      break;
    }
    std::cout << "[RECEIVING] Data: " << buffer;
    file << buffer;
    std::memset(buffer, 0, SIZE);
  }
  file.close();
}

int main()
{
    // Creating a UDP socket
    int fd = socket(PF_INET, SOCK_DGRAM, 0);

    if (fd < 0 )
    {
        perror("socket");
        exit(-1);
    }

    const int port = 2000;
    struct sockaddr_in addr, agent_addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if (ret < 0 )
    {
        perror("bind");
        exit(-1);
    }
    for (int i = 1; i <= 13098; ++i) // Change the loop condition as needed
        {
          char filename[20];
          snprintf(filename, sizeof(filename), "received_%d.bin", i);

          write_file(fd, agent_addr,filename);
          sleep(1);
        }

    close(fd);

    return 0;
 

}


