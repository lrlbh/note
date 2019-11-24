#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <arpa/inet.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include<unistd.h>
#include<poll.h>

int main(int argc, char** argv)
{

	if (argc != 2)
	{
		printf("argc\n");
		exit(-1);
	}

	//创建负责监听的套接字
	int lfd = socket(AF_INET,SOCK_STREAM,0);
	if (lfd == -1)
	{
		perror("socket()");
		exit(-1);
	}

	//创建结构体保存客服端和服务器的IP、端口
	struct sockaddr_in s_addr;
	struct sockaddr_in c_addr;
	int s_addr_siz = sizeof(s_addr);
	int c_addr_siz = sizeof(c_addr);
	memset(&s_addr,0,s_addr_siz);
	memset(&c_addr,0,c_addr_siz);

	//初始化服务器IP和端口
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(atoi(argv[1]));

	//给监视套接字绑定IP和端口
	if (bind(lfd,(struct sockaddr*)&s_addr,s_addr_siz)==-1)
	{
		perror("bind()");
		exit(-1);
	}

	//开始监听
	if (listen(lfd,128)==-1)
	{
		perror("listen()");
		exit(-1);
	}

	//初始化 pollfd 数组，里面保存需要监听的套接字
	struct pollfd fds[1024];
	for (int i = 0; i < 1024; i++)
	{
		//需要监听的事件
		fds[i].events = POLLIN;
		//需要监听的套接字
		fds[i].fd = -1;
	
	}
	fds[0].fd = lfd;
	//目前需要监听多少套接字
	int max_fd = 1;

	
	while (1)
	{
		//使用poll 监听套接字
		int ret_poll = poll(fds,max_fd,-1);
		if (ret_poll == -1)
		{
			perror("ret_poll()");
			exit(-1);
		}

		//判断是否是 监听连接的套接字 读缓冲区有数据
		if (fds[0].revents & POLLIN)
		{
			//创建和客服端通信的套接字
			int cfd = accept(lfd,(struct sockaddr*)&c_addr,&c_addr_siz);
			if (cfd == -1)
			{
				perror("accept()");
				exit(-1);
			}

			//通信套接字加入监听
			for (int i = 0; i < 1024; i++)
			{
				if (fds[i].fd == -1)
				{
					fds[i].fd = cfd;
					//最大需要监听的套接字数量
					max_fd = (i + 1) > max_fd ? (i + 1) : max_fd;
					break;
				}
			}

			//如果只有一个 监听套接字 缓冲区有数据，就不用遍历下的通信套接字了
			if (--ret_poll <= 0)
				continue;

		}

		//遍历 数组 找出缓冲区准备就绪的套接字
		for (int i = 1; i < max_fd; i++)
		{
			if (fds[i].revents & POLLIN)
			{

				//读取客服端发送数据
					int read_ret;
					char buf[1024] = { 0 };
					read_ret = read(fds[i].fd, buf, sizeof(buf));
					//读取失败
					if (read_ret == -1)
					{
						perror("read()");
						continue;
						//exit(-1);
					}

					//对方断开连接
					else if (read_ret == 0)
					{
						printf("连接已关闭\n");
						close(fds[i].fd);
						
						//移除 监听数组 
						fds[i].fd == -1;
						break;
					}

					//数据写回客服端
					if (!write(fds[i].fd, buf, sizeof(buf)))
					{
						perror("writes()");
						exit(-1);
					}






			}


		}


	}

	close(lfd);
	exit(0);
}