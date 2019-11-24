#pragma one
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include<error.h>
#include<errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

//创建一个文件映射到程序内存中用于进程通信
int touch_mmap()
{
	//打开当前目录
	DIR* dir = NULL;
	dir = opendir("./");
	if (dir == NULL)
	{
		//perror("main-->opendir()");
		return -1;
	}

	//循环读取目录下的文件
	struct dirent* redir =NULL;
	while (1)
	{
		//循环读取目录，为判断是否读取失败使用errno
		errno = 0;
		redir = readdir(dir);

		if (redir == NULL)
		{
			//判断是否读到目录尾
			//如果当前目录文件不存在创建文件
			if (errno == 0)
			{ 
				//创建文件
				int fd = open("./my_mmap",O_RDWR| O_CREAT,0776);
				if (fd == -1)
				{
					perror("main-->open()");
					return -1;
				}

				//扩展文件
				lseek(fd, 1024 * 10 - 1, SEEK_SET);
				write(fd, " ", 1);

				close(fd);
				break;
			}
			//判断目录读取是否失败
			//因为不知道到遍历目录什么情况下会失败，无法测试
			//所以这个判断不一定正确
			if (errno != 0)
			{
				perror("main-->readdir()");
				return -1;
			}
		}

		//查看dmymmap文件是否存在
		if (redir->d_name == "my_mmap")
		{
			perror("my_mmap exist");
			return -1;
		}
//查看上一级目录名
#ifdef DEBUF
		static int i = 0;
		if (i == 0)
		{
			printf("%s\n", redir->d_name);
			DIR* temp_dir = opendir("..");
			if (temp_dir == NULL)
			{
				perror("main-->temp_dir");
			}
			struct dirent* temp_redir = NULL;
			while ((temp_redir = readdir(temp_dir)) != NULL)
			{
				printf("[%s]\n", temp_redir->d_name);
			}
			closedir(temp_dir);
			i++;
		}
#endif // 0

		

		
	} 

	closedir(dir);
}

//
void test()
{
	//内存映射通信需要获取被映射文件的文件描述符
	int fd = open("./my_mmap", O_RDWR);
	if (fd==-1)
	{
		perror("main-->open()");
		return ;
	}

	//把文件映射到内存中
	char* mp= mmap(NULL,1024*10, PROT_WRITE, MAP_SHARED,fd,0);
	if (mp == MAP_FAILED)
	{
		perror("main-->mmap");
		return -1;
	}	
	
	
	//设置映射同步后，通信就是修改文件的数据
	strcpy(mp,"嘻嘻哈as哈哈阿萨德");
	sleep(10);
	strcpy(mp, "22222");
	sleep(10);
	strcpy(mp, "333");

	//释放映射区和关闭文件描述符
	//文件映射后马上关闭文件描述符也可以
	munmap(mp,1024*10);
	close(fd);
}

//测试进程通信的代码
//创建一个独立的进程运行
void test01()
{
	int fd = open("./my_mmap", O_RDWR);
	if (fd == -1)
	{
		perror("main-->open()");
		return -1;
	}

	char* mp = mmap(NULL, 1024 * 10, PROT_WRITE, MAP_SHARED, fd, 0);
	if (mp == MAP_FAILED)
	{
		perror("main-->mmap");
		return -1;
	}

	while (1)
	{
		sleep(2);
		printf("%s\n",mp);
	}

	munmap(mp, 1024 * 10);
	close(fd);
}

int main()
{
	
	touch_mmap();
	test();


	printf("ok\n");
	getchar();
	return 0;
}