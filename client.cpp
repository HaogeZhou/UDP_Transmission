#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <cstring>
#define SIZE 20480



void send_file_data(std::ifstream& file, int sockfd, sockaddr_in& addr)
{
  int n;
  char buffer[SIZE];

  // Sending the data
  while (file.getline(buffer, SIZE))
  {
    std::cout << "[SENDING] Data: " << buffer << std::endl;

    n = sendto(sockfd, buffer, SIZE, 0, (struct sockaddr*)&addr, sizeof(addr));
    if (n == -1)
    {
      perror("[ERROR] sending data to the server.");
      exit(1);
    }
    std::memset(buffer, 0, SIZE);
  }

  // Sending the 'END'
  std::strcpy(buffer, "END");
  sendto(sockfd, buffer, SIZE, 0, (struct sockaddr*)&addr, sizeof(addr));
  file.close();
}
int main()
{
     // Defining the IP and Port
    char* ip = "192.168.1.96";
    const int port = 2000;
    
    // make socket for transmission
    int fd = socket(PF_INET, SOCK_DGRAM, 0);

    if (fd <0 )
    {
        perror("socket");
        exit(-1);
    }

    //server_info
    sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    //inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr.s_addr);
    inet_pton(AF_INET, ip, &saddr.sin_addr.s_addr);

    std::ifstream file("00_MP_TXT/allMPs.txt");
    if (!file.is_open()){
      std::cerr << "unable to open allmps txt" << std::endl;
      exit(1);
    }
    std::string filename ;
    while (std::getline(file,filename)){
      filename = "00_MP_TXT/"+filename + ".txt";
      std::ifstream inFile(filename );

      if (!inFile.is_open()){
      std::cerr << "cannot open " << filename  << std::endl;
      continue;
    }
    std::cerr << "sending" << filename << std::endl;
    send_file_data(inFile, fd, saddr);
    inFile.close();
    sleep(1);
    }
    
    file.close();
    close(fd);

    return 0;
}

