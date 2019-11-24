#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include<arpa/inet.h>
#include <unistd.h>

/*
	UDP,发送数据，在接收回发的数据
*/
int main(int argc, char** argv)
{
	if (argc != 3 )
	{
		perror("argc");
		exit(-1);
	}

	//创建UDP通信套接字
	int cfd;
	if ((cfd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("socket()");
		exit(-1);
	}

	//初始化目标信息
	struct sockaddr_in s_addr;
	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(atoi(argv[2]));
	if (inet_pton(AF_INET, argv[1], &s_addr.sin_addr.s_addr) != 1)
	{
		perror("inet_pton()");
		exit(-1);
	}
	int s_addr_siz = sizeof(s_addr);
	
	//读取键盘输入
	char buf[1024] = {0};
	read(0,buf,sizeof(buf));

	//发送数据
	int ret_sendto;
	ret_sendto = sendto(cfd, buf, sizeof(buf), 0, &s_addr, s_addr_siz);
	if (ret_sendto == -1)
	{
		perror("sendto()");
		exit(-1);
	}
	if (ret_sendto > 0)
		printf("发了[%d]个字节的数据：[%s]\n", ret_sendto, buf);

	
	//阻塞接收数据
	memset(buf,0,sizeof(buf));
	int ret_recvfrom;
	ret_recvfrom= recvfrom(cfd,buf,sizeof(buf),0,NULL,NULL);
	if (ret_recvfrom == -1)
	{
		perror("recvfrom()");
		exit(-1);
	}
	if (ret_recvfrom > 0 )
		printf("接收到了[%d]个字节的数据：[%s]\n",ret_recvfrom,buf);


	exit(0);
}