#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include<arpa/inet.h>
#include <unistd.h>

/*
	UDP,广播客服端
*/
int main(int argc, char** argv)
{


	//创建UDP通信套接字
	int cfd;
	if ((cfd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("socket()");
		exit(-1);
	}

	//绑定广播的端口
	struct sockaddr_in s_addr;
	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(8000);
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(cfd, (struct sockaddr_in*) & s_addr, sizeof(s_addr)) == -1)
	{
		perror("bind()");
		exit(-1);
	}


	//读取广播的数据
	char buf[1024] = { 0 };
	int ret_recvfrom;
	while (1)
	{
		ret_recvfrom = recvfrom(cfd, buf, sizeof(buf), 0, NULL, NULL);
		if (ret_recvfrom == -1)
		{
			perror("recvfrom()");
			exit(-1);
		}
		if (ret_recvfrom > 0)
			printf("接收到了[%d]个字节的数据：[%s]\n", ret_recvfrom, buf);
	}



	exit(0);
}