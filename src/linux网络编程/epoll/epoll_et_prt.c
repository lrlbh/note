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
//#define VS
//#define TEST


/*
   epoll_et_ptr
*/

//使用联合体ptr成员
typedef struct
{
	//第一个成员必须是fd，epoll_wait函数才能真确赋值 
	int fd;
	void (*read_event)(struct epoll_event ev);
	void (*writre_event)(struct epoll_event ev);
	void (*error_event)(struct epoll_event ev);
	void* ev_free;

}ptr;

int epfd, lfd;
struct epoll_event temp_event;


//打印错误
//参数1：自定义打印内容
void my_error(char* err)
{
	perror(err);
	exit(-1);
}

//cfd读事件处理函数
void read_event_cfd(struct epoll_event ev)
{
	printf("cfd 开始\n");
	char buf[1024] = { 0 };

	//回发数据
	while (1)
	{
		ptr* cfd_ptr = ev.data.ptr;
		int ret_read = read(cfd_ptr->fd, buf, sizeof(buf));


		//连接关闭
		if (ret_read == 0)
		{
			printf("连接已关闭\n");
			epoll_ctl(epfd, EPOLL_CTL_DEL, cfd_ptr->fd, NULL);
			free(cfd_ptr);
			close(cfd_ptr->fd);
			return;
		}
		else if (ret_read == -1)
		{
			//epoll节点使用ET模式  读取非阻塞套接字  套接字缓冲区中没有数据返回这个宏  
			if (errno = 11)
				return;	

			my_error("read_cfd() --->> read()");
		}
		write(cfd_ptr->fd, buf, sizeof(buf));
	}
	printf("cfd 结束\n");

}






//lfd读缓冲区事件处理函数
void read_event_lfd(struct epoll_event ev)
{
	printf("lfd 开始\n");
	//使用accept函数受理，处理连接
	int cfd;
	struct sockaddr_in temp_caddr;
	int caddr_siz = sizeof(temp_caddr);
continue_accept:
	if ((cfd = accept(lfd, (struct sockaddr*) & temp_caddr, &caddr_siz)) == -1)
	{
		if (errno == EINTR)
			goto continue_accept;
		my_error("accept");
	}

	//新的套接字上树
	memset(&temp_event, 0, sizeof(temp_event));
	ptr* cfd_ptr = malloc(sizeof(ptr));
	cfd_ptr->read_event = read_event_cfd;
	cfd_ptr->fd = cfd;
	temp_event.data.ptr = cfd_ptr;
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
		my_error("inet_ntop");
	printf("[%s]连接到了服务器，\n", cip);

}




//功能：创建监听套接字，绑定，监听,创建epoll树，lfd上树
//参数1：传入端口
void new_lfd_bind_list(int port)
{

	//创建监听套接字
	lfd = socket(PF_INET, SOCK_STREAM, 0);
	if (lfd == -1)
		my_error("socket()");

	//绑定
	struct sockaddr_in s_addr;
	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(port);
	s_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(lfd, (struct sockaddr*) & s_addr,
		sizeof(s_addr)) == -1)
		my_error("bind()");

	//监听
	if (listen(lfd, 128))
		my_error("listen()");

	//创建epoll树
	if ((epfd = epoll_create(1)) == -1)
		my_error("epoll_create()");

	//监视连接的lfd套接字上树
	ptr *lfd_ptr = malloc(sizeof(ptr));
	lfd_ptr->read_event = read_event_lfd;
	lfd_ptr->fd = lfd;
	memset(&temp_event, 0, sizeof(temp_event));
	temp_event.data.ptr = lfd_ptr;
	temp_event.events = EPOLLIN;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &temp_event) == -1)
		my_error("new_lfd_bind_list() --->> epoll_create()");


}




int main(int argc, char** argv)
{
#ifndef VS
	//需要传入端口
	if (argc != 2)
		my_error("argc");

	//创建监听套接字，绑定，监听,创建epoll树，lfd上树
	new_lfd_bind_list(atoi(argv[1]));
#endif //不是用VS执行

#ifdef VS
	//创建监听套接字，绑定，监听,创建epoll树，lfd上树
	new_lfd_bind_list(8001);
#endif //使用VS执行

	struct epoll_event events[1024];
	while (1)
	{
		//epoll监听
		int ret_epollwait = epoll_wait(epfd, events, sizeof(events) / sizeof(events[0]), -1);

		//只关心读缓冲区的变化，也只监听了读缓冲区，所以不用判断
		for (int i = 0; i < ret_epollwait; i++)
		{
			ptr* temp_ptr = events[i].data.ptr;
			//套接字自己的读事件处理函数
			temp_ptr->read_event(events[i]);
		}
	}

	close(lfd);
	exit(0);
}