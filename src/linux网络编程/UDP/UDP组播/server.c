#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
    // 1. 创建通信的套接字
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd == -1)
    {
        perror("socket");
        exit(0);
    }

#if 0
    // 2. 通信的fd绑定本地的IP和端口
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = INADDR_ANY;  // 0 -> 0.0.0.0
    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret == -1)
    {
        perror("bind");
        exit(0);
    }
#endif

    // 设置组播属性
    struct in_addr imr_multiaddr; 
    // 初始化组播地址
    inet_pton(AF_INET, "239.0.0.10", &imr_multiaddr.s_addr);
    setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, &imr_multiaddr, sizeof(imr_multiaddr));

    // 将数据发送给客户端, 使用广播地址和固定端口
    // 初始化客户端的地址信息
    struct sockaddr_in cliaddr;
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(8989); // 客户端也需要绑定这端口
    inet_pton(AF_INET, "239.0.0.10", &cliaddr.sin_addr.s_addr);

    int num = 0;
    // 3. 通信
    while(1)
    {
        // 接收数据
        char buf[128];
#if 0
        char ipbuf[64];
        struct sockaddr_in cliaddr;
        int len = sizeof(cliaddr);
        recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr*)&cliaddr, &len);
        printf("client IP: %s, Port: %d\n", 
               inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ipbuf, sizeof(ipbuf)),
               ntohs(cliaddr.sin_port));
        printf("client say: %s\n", buf);
#endif

        // 发送数据
        sprintf(buf, "hello, client...%d\n ", num++);
        sendto(fd, buf, strlen(buf)+1, 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
        printf("组播的数据: %s\n", buf);
        sleep(1);
    }

    close(fd);

    return 0;
}
