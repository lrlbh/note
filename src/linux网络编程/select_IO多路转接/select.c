#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include<errno.h>

//回声服务器
int main(int argc, char** argv)
{
	//参数传入有误！
	if (argc != 2)
	{
		perror("argv[]");
		return -1;
	}

	//创建监听连接请求的套接字
	int lfd;
	if ((lfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket()");
		return -1;
	}

	//给监听连接的套接字绑定IP和端口
	struct sockaddr_in saddr;
	memset(&saddr, 0x0, sizeof(saddr));
	saddr.sin_port = htons(atoi(argv[1]));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(lfd, (struct sockaddr*) & saddr, sizeof(saddr)) == -1)
	{
		perror("bind()");
		return -1;
	}

	//开始监听
	if (listen(lfd, 128) == -1)
	{
		perror("listen()");
		return -1;
	}

	//初始化需要监听集合
	fd_set reads, temp_reads;
	FD_ZERO(&reads);

	//监听连接请求的套接字，加入select监听集合
	FD_SET(lfd, &reads);

	//需要select需要监听的最大文件描述符数量
	int max_fd = lfd;

	//保存每个套接字对应的客服端信息
	struct sockaddr_in caddr[1024];

	while (1)
	{
		//集合传入时保存的是需要监听的套接字集合
		//select函数结束时传出的是读缓冲区发生改变的套接字集合
		//所以要使用一个临时集合，传入select函数
		temp_reads = reads;

		//开始监听读缓冲区有变化的套接字
		int ret = select(max_fd + 1, &temp_reads, NULL, NULL, NULL);
		//设置了第五个参数，单次监听时间上限
		//同时单次监听结束没有文件描述符变化
		if (ret == 0)
		{
			continue;
		}
		//select函数调用失败
		else if (ret == -1)
		{
			perror("select()");
			return -1;
		}
		//有套接字的读缓冲区发生了变化
		else if (ret > 0)
		{
			//判断是否是监听连接的套接字发生了改变
			if (FD_ISSET(lfd, &temp_reads))
			{
				//如果是监听连接的套接字发生了改变，表示有新的连接请求
				//使用accept函数受理，连接请求
				int cfd;
				struct sockaddr_in temp_caddr;
				int caddr_siz = sizeof(temp_caddr);
				memset(&temp_caddr, 0, caddr_siz);
			continue_accept:
				if ((cfd = accept(lfd, (struct sockaddr*) & temp_caddr, &caddr_siz)) == -1)
				{
					if (errno == EINTR)
						goto continue_accept;
					perror("accept()");
					return -1;
				}

				//保存套接字对应的客服端信息
				memset(&caddr[cfd], 0, caddr_siz);
				memcpy(&caddr[cfd], &temp_caddr, caddr_siz);

				//连接建立后打印客服端IP
				char cip[100] = { 0 };
				if (inet_ntop(AF_INET, &caddr[cfd].sin_addr.s_addr, cip, sizeof(cip)) == NULL)
				{
					perror("inet_ntop()");
					return -1;
				}
				printf("[%s]连接到了服务器，\n", cip);

				//建立连接通信套接字加入监听集合中
				FD_SET(cfd, &reads);

				//如果现在创建出来的套接字编号大于max_fd，就把max_fd重新赋值一下
				if (cfd > max_fd)
				{
					max_fd = cfd;
				}

				if (--ret <= 0)
				{
					//如果只有一个文件文件描述符发生变化
					//那已经处理，可以不再执行下面的代码
					continue;
				}

			}

			//遍历集合查看读缓冲区有数据的套接字
			for (int i = lfd + 1; i < (max_fd + 1); i++)
			{
				if (FD_ISSET(i, &temp_reads))
				{
					char buf[1024] = { 0 };
					int ret_reads = read(i, buf, sizeof(buf));
					//如果客服端已经断开连接
					if (ret_reads == 0)
					{
						//关闭套接字，套接字移除监听集合
						char cip[100] = { 0 };
						if (inet_ntop(AF_INET, &caddr[i].sin_addr.s_addr, cip, sizeof(cip)) == NULL)
						{
							perror("inet_ntop()");
							return -1;
						}
						printf("[%s]退出了连接\n", cip);
						close(i);
						FD_CLR(i, &reads);
						continue;
					}

					//如果套接字读取失败
					if (ret_reads == -1)
					{
						if (104 == errno)
						{
							perror("read()");
							continue;
						}
						printf("read()");
						return -1;
					}

					//连接正常情况下，发送读取到的数据给客服端
					if (!write(i, buf, strlen(buf) + 1))
					{
						perror("write()");
						return -1;
					}
				}
			}

		}

	}

	close(lfd);
	return 0;
}




