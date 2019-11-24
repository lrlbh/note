#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include<arpa/inet.h>
#include <unistd.h>
/*
	UDP广播服务端
*/
int main()
{
	

	//创建UDP通信套接字
	int cfd;
	if ((cfd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("socket()");
		exit(-1);
	}

	//设置套接字属性为广播
	int op = 1;
	if (setsockopt(cfd, SOL_SOCKET, SO_BROADCAST, &op, sizeof(op)) ==-1)
	{
		perror("setsockopt()");
		exit(-1);
	}


	//广播发送数据
	struct sockaddr_in s_addr;
	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(8000);
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	char buf[1024] = { 0 };
	int ret_sendto;

	//发送广播数据
	srand(time(NULL));
	while (1)
	{
		sprintf(buf, "广播数据:[%d]", rand()%1000+1);
		ret_sendto = sendto(cfd, buf, sizeof(buf), 0,(struct sockaddr*)&s_addr, sizeof(s_addr));
		if (ret_sendto == -1)
		{
			perror("sendto()");
			exit(-1);
		}

		if (ret_sendto > 0)
			printf("发了[%d]个字节的数据：[%s]\n", ret_sendto, buf);

		sleep(1);
	}
	
	

	exit(0);
}


