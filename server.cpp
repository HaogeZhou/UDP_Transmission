#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <cstring>
#define SIZE 1024

void write_file(int sockfd, sockaddr_in& addr)
{
  char* filename = "received.bin";
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

    // 
    const int port = 8080;
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
    write_file(fd, agent_addr);
    // communication
    // while (1)
    // {
    //     char buf[128];
    //     char ipbuf[16];

    //     struct sockaddr_in caddr;
    //     socklen_t len = sizeof(caddr);

    //     //receive
    //     int num = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *)&caddr, &len);

    //     printf("client ip:%s,port :%d\n",
    //         inet_ntop(AF_INET, &caddr.sin_addr.s_addr, ipbuf, sizeof(ipbuf)),
    //         ntohs(caddr.sin_port));

    //     printf("client say:%s\n", buf);

    //     //send
    //     sendto(fd, buf, strlen(buf) + 1,0,(struct sockaddr*)&caddr,sizeof(caddr));
    // }

    close(fd);

    return 0;
 

}


