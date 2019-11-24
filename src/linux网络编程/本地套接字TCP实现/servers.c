#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include<sys/un.h>

/*
	本地套接字通信，回声服务器
*/

int main()
{
	//如果套接字文件存在删除
	unlink("./servers.s");
	
	//创建监听的套接字
	int lfd=  socket(PF_LOCAL,SOCK_STREAM,0);
	if (lfd == -1)
	{
		perror("socket()");
		exit(-1);
	}

	//监听套接字绑定套接字文件
	struct sockaddr_un s_addr_un;
	memset(&s_addr_un,0,sizeof(s_addr_un));
	s_addr_un.sun_family = AF_LOCAL;
	strcpy(s_addr_un.sun_path,"./servers.s");
	if (bind(lfd, (struct sockaddr*) & s_addr_un, sizeof(s_addr_un))==-1)
	{
		perror("bind()");
		exit(-1);
	}

	//开始监听
	if (listen(lfd, 128)==-1)
	{
		perror("listen()");
		exit(-1);
	}
	
	//处理连接
	struct sockaddr_un c_addr_un;
	int c_addr_un_siz = sizeof(c_addr_un);
	memset(&c_addr_un,0,c_addr_un_siz);
	int cfd;
goto_accept:
	if ((cfd = accept(lfd, (struct sockaddr*)&c_addr_un, &c_addr_un_siz)) == -1 )
	{
		perror("accept()");
		exit(-1);
	}

	printf("连接成功:对方套接字文件[%s]\n",c_addr_un.sun_path);
	
	//通信
	char buf[1024] = { 0 };
	while (1)
	{
		int ret = read(cfd, buf, sizeof(buf));
		//读取客服端发送的数据
		if (ret == -1)
		{
			perror("read()");
			exit(-1);
		}
		//客服端已关闭，且读缓冲区所有数据已经读完
		else if (ret == 0)
		{
			printf("写端已关闭\n");
			close(cfd);
			goto goto_accept;
		}

		//把读取到的数据，发回给客服端
		if (write(cfd, buf, sizeof(buf)) == -1)
		{
			perror("write()");
			exit(-1);
		}
	}


	exit(0);
}