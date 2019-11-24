#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include<errno.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>

/*
   epoll实现并发服务器示例

   服务器功能“回声服务器”
*/


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

	//创建树，维护需要监视套接字
	int epfd = epoll_create(1);
	if (epfd == -1)
	{
		perror("epoll_create()");
		return -1;
	}

	//传出参数，保存有变化的套接字
	struct epoll_event events[1000];
	//数据上树时使用
	struct epoll_event temp_event;

	//监视连接的lfd套接字上树
	temp_event.data.fd = lfd;
	temp_event.events = EPOLLIN;
	epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &temp_event);



	while (1)
	{
		//epoll_wait函数调用
		memset(&events, 0, sizeof(events));
		int ret = epoll_wait(epfd, events, sizeof(events) / sizeof(events[0]), -1);

		//epoll_wait调用失败
		if (ret == -1)
		{
			perror("epoll_wait()");
			return -1;
		}

		//有套接字变化
		if (ret > 0)
		{
			//遍历有变化的套接字
			for (int i = 0; i < ret; ++i)
			{
				//如果是监视连接的lfd套接字
				if (events[i].data.fd == lfd)
				{
					//使用accept函数受理，处理连接
					int cfd;
					struct sockaddr_in temp_caddr;
					int caddr_siz = sizeof(temp_caddr);
				continue_accept:
					if ((cfd = accept(lfd, (struct sockaddr*) & temp_caddr, &caddr_siz)) == -1)
					{
						if (errno == EINTR)
							goto continue_accept;
						perror("accept()");
						return -1;
					}

					//新的套接字上树
					memset(&temp_event, 0, sizeof(temp_event));
					temp_event.data.fd = cfd;
					temp_event.events = EPOLLIN | EPOLLET;
					epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &temp_event);

					// 设置套接字为非阻塞
				// 获取套接字flag属性
					int flag = fcntl(cfd, F_GETFL);
					// 添加非阻塞属性
					flag = flag | O_NONBLOCK;
					// 将新的flag属性设置给cfd
					fcntl(cfd, F_SETFL, flag);


					//连接建立后打印客服端IP
					char cip[100] = { 0 };
					if (inet_ntop(AF_INET, &temp_caddr.sin_addr.s_addr, cip, sizeof(cip)) == NULL)
					{
						perror("inet_ntop()");
						return -1;
					}
					printf("[%s]连接到了服务器，\n", cip);
				}
				//通信
				else
				{
					//判断具体某个事件的触发
					//if(events[i].events & EPOLLOUT) 
					//{
						//- EPOLLIN  读事件触发
						//- EPOLOUT 写事件触发
						//- EPOLLERR 错误事件触发	
						//- EPOLLET  判断触发模式 ET触发模式
						
					//}
#if 0
					//测试ET模式的通知
					static int pr = 0;
					printf("[%d]\n", ++pr);
#endif // 0




					//读取客服端发送数据
					int read_ret;
					char read_buf[1024] = { 0 };
					/*while ((read_ret = read(events[i].data.fd, read_buf + strlen(read_buf), sizeof(read_buf) - strlen(read_buf)  )) > 0)
					{
						
					}*/
					read_ret = read(events[i].data.fd, read_buf, 1024);
					
					//读取失败
					if (read_ret == -1)
					{
						if (104 == errno)
						{
							perror("read()");
							continue;
						}

						perror("read()");
						exit(-1);
					}

					//对方断开连接
					else if (read_ret == 0)
					{
						printf("连接已关闭\n");
						close(events[i].data.fd);

						//下树
						epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
						continue;
					}

					//数据写回客服端
					if (!write(events[i].data.fd, read_buf, sizeof(read_buf)))
					{
						perror("writes()");
						exit(-1);
					}

				}

			}

		}
	}

	close(lfd);
	getchar();
	exit(0);
}