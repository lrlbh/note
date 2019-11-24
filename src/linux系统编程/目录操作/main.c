#include<stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include<string.h>

//案例1：查看当前目录文件属性
void test01()
{
	//打开需要查找的目录
	DIR *dir = opendir("/home/lr");

	//循环查找当前目录中的每一个文件
	struct dirent* ls = NULL;
	int i = 0;
	while ((ls = readdir(dir)) != NULL)
	{
		i++;
		//通过返回的结构体获取文件属性
		printf("第%d个文件：\n",i);
		printf("inode节点= [%ld]\n", ls->d_ino);
		printf("文件名= [%s]\n", ls->d_name);
		printf("目录文件开头至此目录进入点的位移= [%ld]\n", ls->d_off);
		printf("文件名的长度= [%d]\n", ls->d_reclen);

		if (ls->d_type == DT_BLK)
			printf("文件类型：块设备\n");

		if (ls->d_type == DT_CHR)
			printf("文件类型：字符设备\n");

		if (ls->d_type == DT_DIR)
			printf("文件类型：目录\n");

		if (ls->d_type == DT_LNK)
			printf("文件类型：软连接\n");

		if (ls->d_type == DT_FIFO)
			printf("文件类型：管道\n");

		if (ls->d_type == DT_REG)
			printf("文件类型：普通文件\n");

		if (ls->d_type == DT_UNKNOWN)
			printf("文件类型：未知文件\n");

		if (ls->d_type == DT_SOCK)
			printf("文件类型：套接字\n");
		printf("----------------------------\n");
	}
}

//案例2：递归查找某个目录下普通文件个数
//记录文件的个数
int n=0;
void test02(char * p)
{
	//printf("11\n");
	DIR *dir = opendir(p);
	if (dir == NULL)
	{
		return;
	}

	//循环查找当前目录中的每一个文件
	struct dirent* ls = NULL;
	while ((ls = readdir(dir)) != NULL)
	{
		//两个目录排除
		if ( !strcmp(ls->d_name ,".") || !strcmp(ls->d_name, ".."))
		{
			continue;
		}

		//如果有目录递归进去
		if (ls->d_type == DT_DIR)
		{
			char new_dir[1024] = {0};
			strcpy(new_dir, p);
			strcat(new_dir,ls->d_name);
			strcat(new_dir,"/");
			test02(new_dir);
		}

		if (ls->d_type== DT_REG)
		{
			n++;
			//printf("asd\n");
		}
	}

	closedir(dir);
}


int main(int argc, char **argv)
{
	//test01();
	test02("/");
	printf("当前目录下的普通文件数：[%d]\n",n);


	getchar();
}