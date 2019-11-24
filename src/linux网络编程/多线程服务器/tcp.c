#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <pthread.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
int pr=0;

//子线程号
pthread_t pth;

//子进程代码
void* my_tcp(void *temp_data)
{
	//处理传入数据
	void** data = temp_data;
	int cfd = *((int*)((unsigned long int)(*data) + sizeof(struct sockaddr_in)));
	struct sockaddr_in caddr;
	memcpy(&caddr, *data, sizeof(struct sockaddr_in));
	free(*data);
	//data数据处理完毕，允许下一个线程使用
	*data = NULL;

	//打印连接的IP和负责处理的子线程
	char cip[100] = { 0 };
	if (inet_ntop(AF_INET, &caddr.sin_addr.s_addr, cip, sizeof(cip)) == NULL)
	{
		perror("inet_ntop()");
		pthread_exit(NULL);
	}
	printf("[%s]连接到了服务器，子线程[%ld]负责处理\n", cip, pth);



	//通信
	char buf[1024] = { 0 };
	while (1)
	{
		int ret = read(cfd, buf, sizeof(buf));
		//读取客服端发送的数据
		if (ret == -1)
		{
			perror("read()");
			pthread_exit(NULL);
		}
		else if(ret == 0)
		{
			printf("写端已关闭!!\n");
			close(cfd);
			pthread_exit(NULL);
		}

		//把读取到的数据，发回给客服端
		if (write(cfd, buf, sizeof(buf)) == -1)
		{
			perror("write()");
			pthread_exit(NULL);
		}
	}
}

int main(int argc ,char ** argv)
{
	//参数传入有误！需要传入端口
	if (argc != 2)
	{
		perror("argv[]");
		return -1;
	}

	//创建监听连接的套接字
	int sfd;
	if ((sfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
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
	if (bind(sfd, (struct sockaddr*) & saddr, sizeof(saddr)) == -1)
	{
		perror("bind()");
		return -1;
	}

	//开始监听
	if (listen(sfd, 128) == -1)
	{
		perror("listen()");
		return -1;
	}

	//保存和客服端通信的套接字
	int cfd;
	//保存结构体的长度
	int caddr_siz = sizeof(struct sockaddr_in);
	struct sockaddr_in caddr;

	while (1)
	{
		//处理连接请求
		if ((cfd = accept(sfd, (struct sockaddr*)&caddr, &caddr_siz)) == -1)
		{
			perror("accept()");
			return -1;
		}

		//循环等待确保上一个进程已经对data处理完毕
		static void* data =NULL;
		while (data != NULL)
			usleep(100);

		//保存传入子进程的数据,传入数据也可以使用结构体
		data = malloc(caddr_siz + sizeof(int));
		memcpy(data,&caddr,caddr_siz);
		*((int*)((unsigned long int)data + caddr_siz)) = cfd;

		//创建子进程处理客服端通信
		if (pthread_create(&pth, NULL, my_tcp, (void*)&data))
		{
			perror("pthread_create");
			return -1;
		}
		
		//设置线程分离
		if (pthread_detach(pth))
		{
			perror("pthread_detach");
			return -1;
		}
	}

	getchar();
	return 0;
}