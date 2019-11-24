#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include<arpa/inet.h>

/*
	UDP,接收数据，并回发
*/
int main()
{
	//创建UDP通信套接字
	int cfd;
	if ((cfd=socket(PF_INET,SOCK_DGRAM,0))== -1)
	{
		perror("socket()");
		exit(-1);
	}

	//bind端口和ip
	struct sockaddr_in s_addr;
	memset(&s_addr,0,sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(8000);
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(cfd,(struct sockaddr*)&s_addr, sizeof(s_addr)) == -1)
	{
		perror("bind()");
		exit(-1);
	}


	//阻塞等待数据接收
	char buf[1024] = {0};
	struct sockaddr_in c_addr;
	int c_addr_siz = sizeof(c_addr);
	memset(&c_addr, 0, c_addr_siz);
	int ret_recvfrom;
goto_recvfro:
	ret_recvfrom = recvfrom(cfd, buf, sizeof(buf), 0, &c_addr, &c_addr_siz);
	if (ret_recvfrom == -1 )
	{
		perror("recvfrom()");
		exit(-1);
	}

	//发送数据
	if (ret_recvfrom > 0)
	{
		printf("接收到[%d]个字节的数据：[%s]\n",ret_recvfrom,buf);
		int ret_sendto;
		ret_sendto = sendto(cfd,buf,sizeof(buf),0,&c_addr,c_addr_siz);
		if (ret_sendto == -1)
		{
			perror("sendto()");
			exit(-1);
		}

		if (ret_sendto > 0)
			printf("发了[%d]个字节的数据：[%s]\n", ret_sendto, buf);
	}

	goto goto_recvfro;

	exit(-1);
}


