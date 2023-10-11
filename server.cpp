#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    //
    int fd = socket(PF_INET, SOCK_DGRAM, 0);

    if (fd == -1)
    {
        perror("socket");
        exit(-1);
    }

    // 
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = INADDR_ANY;

    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if (ret == -1)
    {
        perror("bind");
        exit(-1);
    }

    // communication
    while (1)
    {
        char buf[128];
        char ipbuf[16];

        struct sockaddr_in caddr;
        socklen_t len = sizeof(caddr);

        //receive
        int num = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *)&caddr, &len);

        printf("client ip:%s,port :%d\n",
            inet_ntop(AF_INET, &caddr.sin_addr.s_addr, ipbuf, sizeof(ipbuf)),
            ntohs(caddr.sin_port));

        printf("client say:%s\n", buf);

        //send
        sendto(fd, buf, strlen(buf) + 1,0,(struct sockaddr*)&caddr,sizeof(caddr));
    }

    close(fd);

    return 0;
 

}


