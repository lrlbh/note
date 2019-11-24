#pragma one
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include<error.h>
#include<errno.h>

//目录操作
int touch_mmap(char * p)
{
	//打开当前目录
	DIR* dir = NULL;
	dir = opendir(p);
	if (dir == NULL)
	{
		//perror("main-->opendir()");
		return -1;
	}

	struct dirent* redir =NULL;
	while (1)
	{
		//循环读取目录，为判断是否读取失败使用errno
		errno = 0;
		redir = readdir(dir);

		if (redir == NULL)
		{
			//判断是否读到目录尾
			if (errno == 0)
			{ 
				break;
			}
			//判断目录读取是否失败
			//因为不知道到遍历目录什么情况下会失败，无法测试
			//所以这个判断不一定真确
			if (errno != 0)
			{
				perror("main-->readdir()");
				return -1;
			}
		}

		//点和点点目录不需要
		if (!strcmp(redir->d_name, ".") || !strcmp(redir->d_name, ".."))
			continue;

		//递归读取目录
		if (redir->d_type == DT_DIR)
		{
			//保存目录路径
			char new_dir[1024] = { 0 };
			//拷贝上一级路径
			strcpy(new_dir, p);
			//加入需要进入的路径名称
			strcat(new_dir, redir->d_name);
			//末尾加个反斜杆
			strcat(new_dir, "/");
			//递归进入下一个路径
			touch_mmap(new_dir);
		}
		
	} 
	closedir(dir);
}

int main()
{
	touch_mmap("/");

	printf("ok\n");
	getchar();
	return 0;
}