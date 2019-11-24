#include <sys/types.h>        
#include <sys/socket.h>
#include<arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>


#define BUF_SIZE 100

int main(int argc, char** argv)
{
	//套接字
	int s_sock, c_sock;
	//地址信息
	struct sockaddr_in s_addr, c_addr;
	//需要监听的文件描述符集合
	fd_set reads, cpy_reads;
	//读写数据
	char buf[BUF_SIZE];
	//设置监听时间使用
	struct timeval timeout;
	//
	socklen_t adr_sz;
	//
	int fd_max, str_len, fd_num, i;

	//判断是否传入了端口
	if (argc!=2)
	{
		perror("argc");
		return -1;
	}

	//创建套接字
	s_sock = socket(PF_INET,SOCK_STREAM,0);
	if (s_sock==-1)
	{
		perror("socket()");
		return -1;
	}

	//获取IP和端口
	memset(&s_addr,0,sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(atoi(argv[1]));
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	//给套接字分配ip和端口
	if (bind(s_sock,
		(struct sockaddr*)&s_addr,sizeof(s_addr))  ==-1)
	{
		perror("bind()");
		return -1;
	}

	//接收请求队列
	if (listen(s_sock,1)==-1)
	{
		perror("listem()");
		return -1;
	}

	//初始化监听集合
	FD_ZERO(&reads);
	//把服务器套接字加入监听集合
	FD_SET(s_sock,&reads);
	//初始监听数量
	fd_max = s_sock;

	//循环监听变化
	while (1)
	{
		//监听结束后监听集合会有变化，所以使用临时集合
		cpy_reads = reads;
		//单次监听最大时间
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		//监听函数调用失败,结束程序
		if ((fd_num=select(fd_max+1,
			&cpy_reads,0,0,&timeout))==-1)
		{
			perror("select()");
			break;
		}
		//printf("监听\n");

		//单次监听没有变化,继续下一次监听
		if (fd_num == 0)
			continue;

		if (FD_ISSET(s_sock, &cpy_reads))
		{
			//printf("333[%d]\n", i);
			//创建服务套接字
			adr_sz = sizeof(c_addr);
			c_sock = accept(s_sock,
				(struct sockaddr*)&c_sock, &adr_sz);
			//把服务服务的套接字加入监听
			FD_SET(c_sock, &reads);
			//如果新的服务套接字大于过去的监听数量
			//更新最大监听数量
			if (fd_max < c_sock)
				fd_max = c_sock;
			//输出当前服务套接字的id
			printf("NEW c_socket=[%d]\n", c_sock);
			if (--fd_num <= 0)
			{
				continue;
			}
		}

		//遍历所有套接字，查看有变化的套接字
		for (i = 0; i < fd_max+1; i++)
		{
			//查看当前套接字是否有变化
			if (FD_ISSET(i,&cpy_reads))
			{
				//printf("222[%d]\n",i);
				//如果是服务器主套接字有变化，添加新的服务套接字
				{
					//printf("客服端套接字\n");
					//读取变化套接字的数据
					str_len = read(i, buf, BUF_SIZE);
					//如果没有读取到数据
					//取消对当前服务套接字的监听
					//关闭当前服务套接字
					//打印关闭的服务套接字id
					if (str_len == 0)
					{
						FD_CLR(i, &reads);
						close(i);
						printf("close c_socke=[%d]\n", i);
					}
					//如果读取到数据
					//发送读取到的数据给客服端
					else
					{
						write(i, buf, str_len);
					}

				}
			}
		}
	}
	
	//关闭服务套接字
	close(s_sock);
	return 0;

}
