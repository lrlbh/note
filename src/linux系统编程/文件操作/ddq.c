#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//复制文件描述符  dup
void test01()
{
	int fd = open("tmp.txt", O_RDWR | O_CREAT, 0777);

	if (fd == -1)
	{
		perror("open");
	}

	//复制文件描述符
	int dup_fd = dup(fd);
	
	
	
	write(dup_fd, "嘻嘻哈", 10);

	printf("fd=[%d]", fd);
	printf("dup_fd=[%d]", dup_fd);

	if (close(fd) == -1)
	{
		perror("fd");
	}

	if (close(dup_fd) == -1 )
	{
		perror("dup_fd");
	}

}

//重定向文件文件描述符 duq2
void test02()
{
	int fd = open("tmp2.txt", O_RDWR | O_CREAT, 0777);
	int fd_1 = open("tmp3.txt", O_RDWR | O_CREAT, 0777);

	//把文件描述符fd_1 重定向到fd
	int tmp_fd = dup2(fd,fd_1);
	
	
	write(tmp_fd, "嘻嘻\n", 8);
	write(fd,"呵呵\n",8);
	write(1,"ok\n",3);
	write(fd_1,"阿萨\n",8);
	write(1, "ok\n", 3);

	printf("tmep_fd=[%d]",tmp_fd );
	printf("fd=[%d]",fd );
	printf("fd_1=[%d]",fd_1 );
	close(fd);
	close(tmp_fd);

}



int main(int argc, char* argv)
{
	test01();



	getchar();
}