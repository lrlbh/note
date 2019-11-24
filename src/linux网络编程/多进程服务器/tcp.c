#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <arpa/inet.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include<errno.h>

//回收子进程
void dele_fork(int signo)
{
	pid_t wpid;
	int wstatus;
	while ((wpid = waitpid(-1, &wstatus, WNOHANG)) > 0)
	{
		printf("子进程pid=[%d],已回收\n", wpid);

		if (WIFEXITED(wstatus) != 0)
		{
			printf("程序正常退出，返回值[%d]\n", WEXITSTATUS(wstatus));
		}
		else if (WIFSIGNALED(wstatus) != 0)
		{
			printf("程序异常退出，终止进程的信号[%d]\n", WTERMSIG(wstatus));
		}

	}
}

int main(int argc ,char** argv)
{
	//注册子进程死亡信号
	signal(SIGCHLD, dele_fork);

	//参数传入有误！
	if (argc != 2)
	{
		perror("argv[]");
		return -1;
	}

	//创建监听连接的套接字
	int sfd;
	if ((sfd = socket(PF_INET, SOCK_STREAM, 0))==-1)
	{
		perror("socket()");
		return -1;
	}

	//给监听连接的套接字绑定IP和端口
	struct sockaddr_in saddr;
	memset(&saddr,0x0,sizeof(saddr));
	saddr.sin_port = htons(atoi(argv[1]));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sfd,(struct sockaddr *)&saddr,sizeof(saddr))==-1)
	{
		perror("bind()");
		return -1;
	}

	//开始监听
	if (listen(sfd,128) == -1)
	{
		perror("listen()");
		return -1;
	}

	//保存客服端信息
	struct sockaddr_in caddr;
	int caddr_siz=sizeof(caddr);
	//保存和客服端通信的套接字
	int cfd;

	while (1)
	{
		//处理连接请求
		if ((cfd = accept(sfd, (struct sockaddr*) &caddr, &caddr_siz)) == -1)
		{
			if (errno == EINTR)
				continue;
			perror("accept()");
			return -1;
			
		}
		//子进程处理连接请求
		pid_t pid = fork();
		if (pid == 0)
		{
			//打印连接的IP和负责处理的子进程
			char cip[100] = {0};
			if (inet_ntop(AF_INET,&caddr.sin_addr.s_addr,cip,sizeof(cip))==NULL)
			{
				perror("inet_ntop()");
				return -1;
			}
			printf("[%s]连接到了服务器，子进程[%d]负责处理\n", cip,getpid());

			
			char buf[1024] = {0};
			while (1)
			{
				int ret = read(cfd, buf, sizeof(buf));
				//读取客服端发送的数据
				if (ret == -1)
				{
					perror("read()");
					return -1;
				}
				//客服端已关闭，且读缓冲区所有数据已经读完
				else if (ret == 0)
				{
					printf("写端已关闭");
					close(cfd);
					return 0;
				}

				//把读取到的数据，发回给客服端
				if (write(cfd, buf, sizeof(buf)) == -1)
				{
					perror("write()");
					return -1;
				}
			}
		}

		//父进程继续受理连接请求
		if (pid > 0)
			continue;

		//错误退出
		if (1)
		{
			perror("fork()");
			return -1;
		}

	}

	getchar();
	return 0;
}