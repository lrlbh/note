#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include<sys/un.h>

/*
	本地套接字通信，客服端
*/

int main()
{
	//如果套接字文件存在删除
	unlink("./client.s");

	//创建通信的套接字
	int cfd = socket(AF_LOCAL, SOCK_STREAM, 0);
	if (cfd == -1)
	{
		perror("socket()");
		exit(-1);
	}

	//绑定
	//struct sockaddr_un c_addr_un;
	//int c_addr_un_size = sizeof(c_addr_un);
	//memset(&c_addr_un, 0, c_addr_un_size);
	//c_addr_un.sun_family = AF_LOCAL;
	//strcpy(c_addr_un.sun_path, "client.s");
	//if (bind(cfd, (struct sockaddr*) & c_addr_un, c_addr_un_size) == -1)
	//{
	//	perror("bind()");
	//	exit(-1);
	//}

	//连接
	struct sockaddr_un s_addr_un;
	int s_addr_un_size = sizeof(s_addr_un);
	memset(&s_addr_un, 0, s_addr_un_size);
	s_addr_un.sun_family = AF_LOCAL;
	strcpy(s_addr_un.sun_path, "servers.s");
	if (connect(cfd, (struct sockaddr*) & s_addr_un, s_addr_un_size)==-1)
	{
		perror("connect()");
		exit(-1);
	}
	
	while (1)
	{
		char buf[1024] = { 0 };
		//memset(buf,'a',1024);

		//读取键盘输入
		if ((read(1, buf, sizeof(buf))) == -1)
		{
			perror("read()");
			return -1;
		}

		//把键盘输入发送给服务器
		if (write(cfd, buf, sizeof(buf)) == -1)
		{
			perror("write()");
			return -1;
		}

		//读取服务器发送的数据
		memset(&buf, 0, sizeof(buf));
		if ((read(cfd, buf, sizeof(buf))) == -1)
		{
			perror("read2()");
			return -1;
		}

		//打印服务器发送的数据
		if (write(1, buf, sizeof(buf)) == -1)
		{
			perror("write()2");
			return -1;
		}

	}
	eixt(-1);
}