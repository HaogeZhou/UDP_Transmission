#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    // make socket for transmission
    int fd = socket(PF_INET, SOCK_DGRAM, 0);

    if (fd == -1)
    {
        perror("socket");
        exit(-1);
    }

    //server_info
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    //inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr.s_addr);
    inet_pton(AF_INET, "192.168.1.66", &saddr.sin_addr.s_addr);


    int num = 0;
    // Communication
    while (1)
    {
        char buf[128];
        sprintf(buf, "hello ,i am client %d\n", num++);

        //send
        sendto(fd, buf, strlen(buf) + 1,0,(struct sockaddr*)&saddr,sizeof(saddr));

        //receive
        int num = recvfrom(fd, buf, sizeof(buf), 0, NULL, NULL);

        printf("server say:%s\n", buf);

        sleep(1);
    }

    close(fd);

    return 0;
}


