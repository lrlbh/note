**人生有涯，而知无涯，已有涯求无涯，殆矣**



# Linux系统编程



## 文件IO



### 说明

>linux文件IO，没有缓冲区



### 打开或者新建文件

~~~c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int open(const char *pathname, int flags);
int open(const char *pathname, int flags, mode_t mode);
参数：
	pathname:需要打开或者创建的文件
	flags：打开方式
		必选项（三个选项互斥）：
			O_RDONLY:只读
			O_WRONLY: 只写
			O_RDWR: 读写
		可选项:
			O_CREAT: 文件不存在，新建文件，文件存在相当于没写这个选项
			O_APPEND: 在文件末尾追加

	mode：
		若是新建文件需要通过这个参数指定权限，
		填八进制 如 0777，
		创建出来的权限需要减去 umask
		可以通过umask命令查看默认权限 ，open函数生成文件的权限: mode & ~umask

 

返回值：
	成功返回：文件描述符
	失败返回：-1，设置errno

注意：
	C语言默认打开的三个文件描述符
		STDIN_FILENO：输入
		STDOUT_FILENO：输出
		STDERR_FILENO：错误输出
~~~



### 关闭文件文件描述符

~~~c
#include <unistd.h>
int close(int fd);
参数:
	fd: open需要关闭的文件描述符

返回值:
	成功: 返回0
	失败: 返回-1
~~~



### 读文件

~~~c++
#include <unistd.h>
ssize_t read(int fd, void *buf, size_t count);

参数：
	fd：需要读取的文件描述符
	buf：保存读取数据的地方
	count：需要读取的字节数

返回值：
	成功返回：读取到的字节数字
	返回0表示：读到了文件末尾,或者写端关闭
	失败返回：-1，设置errno
		errno = 11，非阻塞情况下读取到缓冲区中没有数据的文件描述符
		errno =104，发送数据太快会出现

注意：
	​ 函数阻塞与否由文件描述符决定
	​ 读取pipe和mkfifo创建的文件描述符，函数会阻塞                               
~~~



### 写文件

~~~c
#include <unistd.h>
ssize_t write(int fd, const void *buf, size_t count);

参数：
	fd：需要写的文件描述符
	buf：需要写的数据
	count：需要写的字节数，如果写入数据不足，末尾自动补 \0

返回值：
	成功返回：写入的字节数
	失败返回：-1，设置errno

注意：
	​ 函数阻塞与否由文件描述符决定
    ​ 对pipe和mkfifo创建的文件描述符进行写操作时，函数会阻塞
~~~



### 文件指针操作

~~~c
#include <sys/types.h>
#include <unistd.h>
off_t lseek(int fd, off_t offset, int whence);

参数：
	fd：文件描述符
	offset：偏移量
	whence：指针的位置
		SEEK_SET--文件头
		SEEK_CUR--当前位置
		SEEK_END--末尾

返回值：返回文件指针的位置

示例：
// 移动文件指针到文件头
lseek(fd, 0, SEEK_SET);

// 获取当前文件指针的位置
lseek(fd, 0, SEEK_CUR);

//获取文件长度
lseek(fd, 0, SEEK_END);

//拓展文件长度, 拓展100字节
lseek(fd, 100, SEEK_END);

// 要进行一次写操作
write(fd, " ", 1);
~~~



### 操作文件描述符

~~~c
#include <unistd.h>
int dup(int oldfd);`

功能：复制一个文件描述符

参数：
	oldfd：需要被复制的文件描述符

返回值：
	成功返回：一个新的文件描述符
	失败返回：-1，设置errno

注意：
	此时有两个文件描述符记得都要关闭

 

#include <unistd.h>
int dup2(int oldfd, int newfd);

功能：重定向一个文件描述符

参数：
	oldfd：不需要不改变指向文件描述符
	newfd：需要改变指向的文件描述符

返回值：
	成功返回： newfd
	失败返回：-1，设置errno

注意：
	​ 两个文件描述符都需要关闭
	​ 此时两个文件描述符指向同一个文件

 

#include <unistd.h>
#include <fcntl.h>
int fcntl(int fd, int cmd, ... /* arg */ );

功能：该函数可以对文件描述符作一些设置，如修改读写操作，设置文件描述符带阻塞等等
    
    

//代码示例
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
~~~





##  linux常用系统函数

### 目录操作函数

#### 文件重命名

~~~c
#include <stdio.h>
int rename(const char *oldpath, const char *newpath);
参数：
	oldpath：旧的文件名
	newpath：新的文件名

返回值：
	成功返回：0
	失败返回：-1，设置errno
~~~





 

#### 修改进程的工作目录

~~~c
#include <unistd.h>

int chdir(const char *path);
参数：
	path：切换到那个工作目录

返回值：
	成功返回：0
	失败返回：-1，设置errno
~~~





 

 

#### 获取进程的当前工作目录

~~~c
#include <unistd.h>
char *getcwd(char *buf, size_t size);
参数：
	buf：传入参数保存进程当前工作目录
	size：buf大小

返回值：
	成功返回：参数buf的首地址
	失败返回：NULL，设置errno  
~~~





 

#### 创建目录

~~~c
#include <sys/stat.h>
#include <sys/types.h>
int mkdir(const char *pathname, mode_t mode);
参数：
	pathname：目录名称
	mode：目录的权限，8进制，mod & ~umask

返回值：
	成功返回：0
	失败返回：-1
~~~





 

#### 删除目录

~~~c
#include <unistd.h>
int rmdir(const char *pathname);
参数：
	pathname：目录名

返回值：
	成功返回：0
	失败返回：-1

注意：
	​ 相当于 rm -f 只能删除空目录
~~~





 

#### 打开一个目录

~~~c
#include <sys/types.h>

#include <dirent.h>

DIR *opendir(const char *name);

参数: 
	name: 要打开的目录名

返回值: 
	成功返回：DIR
	失败返回：NULL 

注意：
	//关闭目录
	int closedir(DIR *dirp);
~~~





 

#### 读取目录中的数据

~~~c
#include <dirent.h>

struct dirent* readdir(DIR *dirp);

参数：
	dirp：打开的目录

返回值：
	成功返回：dirent结构体,里面保存了一个文件的属性
	失败返回：NULL，不设置errno
	读取到目录尾返回：NULL，设置errno

注意:
	目前想不到这个函数可以能会出现失败的情况，所以把NULL当做读到文件尾即可，传入错误参数只会段错误，不会函数调用失败

使用到的结构体：
	struct dirent
	{
  		ino_t d_ino;        // 此目录进入点的inode
  		ff_t d_off;        // 目录文件开头至此目录进入点的位移
  		signed short int d_reclen; // d_name 的长度, 不包含NULL 字符
  		unsigned char d_type;   // d_name 所指的文件类型 
  		har d_name[256];     // 文件名
	};

	d_type
		DT_BLK - 块设备
		DT_CHR - 字符设备
		DT_DIR - 目录
		DT_LNK - 软连接
		DT_FIFO - 管道
		DT_REG - 普通文件
		DT_SOCK - 套接字
		DT_UNKNOWN - 未知
~~~

~~~c
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
~~~



~~~c
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
~~~





### 文件属性函数

#### 判断文件权限、存在

~~~c
#include <unistd.h>
int access(const char *pathname, int mode);

函数功能：判断是否有操作某个文件的权限，或者判断文件是否存在

参数：
	pathname：文件名
	mode：
		R_OK：判断是否有读权限
		W_OK：判断是否有写权限
		X_OK：判断是否有执行权限
		F_OK：判断文件是否存在

返回值：
	有操作权限、文件存在返回：0
	没有有操作权限、文件存在返回：-1，设置erron
~~~





 

#### 修改文件权限

~~~c
#include <sys/stat.h>
int chmod(const char *filename, int mode);

参数：
	filename：文件名
	mode：八进制数设置文件权限 如：0777

返回值：
	成功返回：0
	失败返回：-1，设置errno
~~~





 

#### 修改文件所有者

~~~c
#include <unistd.h>
int chown(const char *path, uid_t owner, gid_t group);
参数：
	path：要修改的文件名
	owner：用户ID
	group：组ID

返回值：
	成功返回：0
	失败返回：-1，设置errno
~~~





 

#### 修改文件大小

~~~c
#include <unistd.h>
#include <sys/types.h>
int truncate(const char *path, off_t length);

参数：
	path：要操作的文件
	length：最终的文件大小

返回值：
	成功返回：0
	失败返回：-1，谁知errno
~~~





 

### 获取文件属性stat()

~~~c
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
int stat(const char *pathname, struct stat *statbuf);

功能：获取一个文件的详细属性

参数：
	pathname：文件名
	buf: 传出参数, 保存文件的属性信息

返回值:
	成功: 返回0
	失败: 返回-1, 并设置errno

注意：
	stat函数: 获取的是连接文件指向的文件的属性
	lstat函数: 获取的是连接文件本身的属性

 
使用到的结构体：
	struct stat {
		dev_t     st_dev;       //文件的设备编号
		ino_t      st_ino;       //节点
		mode_t   st_mode;        //文件的类型和存取的权限
		nlink_t    st_nlink;      //连到该文件的硬连接数目，刚建立的文件值为1
		uid_t      st_uid;    //用户ID
		gid_t      st_gid;    //组ID
		dev_t     st_rdev;    //(设备类型)若此文件为设备文件，则为其设备编号
		off_t      st_size;    //文件字节数(文件大小)
        blksize_t   st_blksize;   //块大小(文件系统的I/O 缓冲区大小)
        blkcnt_t   st_blocks;     //块数
        time_t     st_atime;    //最后一次访问时间
        time_t     st_mtime;    //最后一次修改时间
        time_t     st_ctime;    //最后一次改变时间(指属性)
	};

关于变量 st_mode: st_mode -- 16位整数	
	0-2 bit -- 其他人权限
		S_IROTH  00004 读权限
		S_IWOTH  00002 写权限
		S_IXOTH  00001 执行权限
		S_IRWXO  00007  掩码, 过滤 st_mode中除其他人权限以外的信息
	3-5 bit -- 所属组权限
		S_IRGRP  00040 读权限
		S_IWGRP  00020 写权限
		S_IXGRP  00010 执行权限
		S_IRWXG  00070 掩码, 过滤 st_mode中除所属组权限以外的信息
	6-8 bit -- 文件所有者权限
		S_IRUSR  00400  读权限
		S_IWUSR  00200  写权限
		S_IXUSR  00100  执行权限
		S_IRWXU  00700  掩码, 过滤 st_mode中除文件所有者权限以外的信息
	12-15 bit -- 文件类型
		S_IFSOCK  0140000 套接字
		S_IFLNK  0120000 符号链接（软链接）
		S_IFREG  0100000 普通文件
		S_IFBLK  0060000 块设备
		S_IFDIR  0040000 目录
		S_IFCHR  0020000 字符设备
		S_IFIFO  0010000 管道
		S_IFMT   0170000 掩码,过滤 st_mode中除文件类型以外的信息
	(st_mode & S_IFMT) == S_IFREG
~~~



### exec

~~~c
#include<unistd.h>
int execlp(const char *file,const char *arg)

功能：使用一个进程拉起另外一个进程，被拉起进程的用户区 替换 拉起进程的用户区，内核区不变，所以不会创建出新的进程 
    
参数：
	file:命令、程序名称，会在path里面找，一般用来执行命令
	arg:参数可用多个，用NULL结束

 

#include<unistd.h>
int execl (const char *file,const char *arg)
参数：
	file:命令、程序名称，不会在path里面找，所以一般用于执行程序
	arg:参数可用多个，用NULL结束

返回值：
	成功：无返回
	失败，返回-1，设置errno
~~~



##  进程

### 进程说明：

>- 每一个进程都拥有独立的虚拟地址空间，进程是linux系统资源的最小分配单位。
>
>- fork后，只有少量内核中的少量数据不会被拷贝，目前已知PID。
>
>- 孤儿进程，父进程在子进程之前推出了程序，子进程会认PID为1的进程做父进程，由PID为1的进程负责释放
>
>- 僵尸进程，子进程结束后父进程不负责回收，同时父进程还在运行中，子进程就会变成僵尸进程，僵尸进程需要杀死父进程才会死亡



 

### 创建进程：

~~~c
#include<unistd.h>
pid_t fork() (void)

成功返回：
	父进程返回：父进程pid
	子进程返回：0
失败返回：-1，设置errno
~~~





 

 

### 获取当前进程的PID和获取父进程的PID

~~~c
#include <sys/types.h>
#include <unistd.h>

//获取当前进程的PID:
pid_t getpid(void);

//获取父进程的PID:
pid_t getppid(void);
~~~



 

### 进程回收函数

~~~c
#include <sys/types.h>
#include <sys/wait.h>
pid_t wait(int *wstatus)

功能：回收一个结束的子进程
    
参数：wstatus: 传一个int数据进去就行
	WIFEXITED(wstatus)值为非0：进程正常结束
		WEXITSTATUS(wstatus):
			子进程的返回值，如果没有返回值，结果可能是随机数
			值是无符号1字节数据，如果返回值为负数，接收可能不正确
				-1 == 255，-2==254 ，以此类推
	WIFSIGNALED(wstatus): 值为为非0：进程异常终止
		WTERMSIG(wstatus):
			终止进程的信号编号(整型数据)

返回值:
	成功返回：子进程PID
	失败返回：-1

注意：
	函数带阻塞
	只回收一个进程
	通过4个宏函数，获取子进程退出状态

 

//可以设置非阻塞和回收指定进程的进程回收函数
pid_t waitpid(pid_t pid, int *wstatus, int options)

参数：
	pid:
		pid= -1: 所有子进程都可以回收
		pid> 0: 需要回收的子进程PID
		pid= 0: 没写，不知道
	wstatus：用法和上一个函数相同
	options：传WNOHANG不阻塞，传0阻塞

返回值:
	0: 回收掉的子进程PID
	-1: 无子进程
	=0: 参数options为非阻塞，且子进程未结束
~~~





 

 

### 进程通信pipe (管道)

~~~c
#include <unistd.h>
int pipe(int pipefd[2]);

功能：匿名管道返回两个文件描述符，父子进程之间通信

参数：
	pipefd：传出参数
		pipefd[0]:管道的读端
		pipefd[1]:管道的写端

返回值：
	成功返回：0
	失败返回：-1，设置errno

注意：
	管道是单向的，建议读端关闭写端，写端关闭读端
	管道的默认大小4k  查看ulimit -a
	管道可以的会动态扩展一部分空间
	管道文件描述符带阻塞
	管道的结构，环形队列
~~~





​     

 

 

### 进程通信mkfifo(管道)

~~~c
#include <sys/types.h>
#include <sys/stat.h>
int mkfifo(const char *pathname, mode_t mode);

功能：有名管道，创建一个管道文件，进行进程通信

参数：
	pathname：创建的文件名
	mode：文件的权限

返回值：
	成功返回：0
	失败返回：-1，设置ernno

注意：
	除了管道的大小其他的应该都和上面一样
~~~



 

 

### 进程通信mmap（内存映射区）

~~~c
#include <sys/mman.h>
void *mmap(void *addr, size_t length, int prot, int flags,int fd, off_t offset);

功能：把一个硬盘中的普通文件映射到 程序的内存中 相当于两个进程拥有同一块内存

参数：
	addr：传NULL即可，映射到具体的内存位置
	length：映射区的大小，一般用文件的大小即可
	prot：文件的读写权限
		PROT_READ读
		PROT_WRIFE 写
		PROT_READ | PROT_WRIFE 读写
	flags：映射区的内容是否同步到文件
		MAP_SHARED：同步到文件，进程通信必须设置同步
		MAP_PRIVATE：不同步到文件
	fd：被映射文件的文件描述符
	offset：映射文件的开始偏移量，必须是4k的整数倍，传0表示从文件其实位置开始映射

返回值：
	成功返回：映射区的首地址
	失败返回：MAP_FAILED #define MAP_FAILED ((void *) -1)

 

//内存映射区释放：
int munmap(void *addr, size_t length);
参数：
	addr：映射区的首地址
	length：映射区的大小

返回值：
	成功返回：0
	失败返回：-1，设置errno
~~~

**代码示例：**

~~~c
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
~~~



 

### 守护进程

#### 守护进程的特点

> 守护进程一般用d作为结尾
>
> 守护进程是脱离终端的
>
> 周期性的执行某些任务



#### 示例：

~~~c
//父进程fork子进程, 然后退出
pid_t pid = fork();
if(pid>0 || pid<0)
{
    exit(0);
}

//子进程调用setsid函数创建会话
setsid();

//修改当前的工作目录
chdir("/home/itcast/log");

//重设文件掩码
umask(0000);

//关闭文件描述符
close(STDIN_FILENO);
close(STDOUT_FILENO);
close(STDERR_FILENO);
~~~





##  信号

### 信号说明:

>- 一个进程执行过程中如果收到某个信号，会放下当前执行的代码，去处理这个信号，信号处理完成后在接着执行当前代码
>
>- 信号可以被阻塞、捕获 - SIGKILL, SIGSTOP 这两个信号 不允许被捕捉、阻塞



#### 信号的状态

>- 产生
>
>- 未决
>  - 信号没有被处理
>  - 阻塞是未决的一种状态
>
>- 递达
>  - 信号已经被递达，开始处理
>  - 递达不保证这个信号被正确处理结束，因为在处理一个信号的过程中可能收到另外一个信号，导致当前信号，处理中断。



#### 信号列表

>可以通过命令：kill -l 查询当前系统有哪些信号



 

#### 信号的产生和默认处理动作

>通过命令：man 7 signal 查看每个信号详细信息



 

 

### 信号处理函数：

~~~c
typedef void (*sighandler_t)(int);

示例：
	void 函数名(int 处理的信号编号)
	void lr_sig(int signo)

注意：
	1、信号处理函数执行中，默认会阻塞当前信号
	2、信号处理函数执行中，如果收到其他未被阻塞信号，会执行会去执行其他信号
~~~





 

 

### 注信号处理函数1：（捕获信号）

~~~c
#include <signal.h>
sighandler_t signal(int signum, sighandler_t handler); 

功能：捕获某个信号，交给自定义的信号处理函数处理

参数： 
	signum：信号的编号，需要捕捉的信号
    handler：自定义的函数名, 可以是SIG_IGN--表示忽略, SIG_DFL--表示行默认动作

返回值: 无

注意：
	​ 在不同的平台下，这个函数可能不同，所以建议使用sigaction函数
	​ SIGKILL, SIGSTOP 这两个信号 不允许被捕捉, 阻塞和忽略
	​ 处理当前信号过程中会自动阻塞当前信号
~~~





### 注册信号处理函数2（捕获信号）

~~~c
#include <signal.h>
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);

参数：
	signum:需要捕捉的信号
	act:传入参数,处理方式
	oldact:传出参数,旧的处理方式

返回值:
	成功返回：0
	失败返回：-1，设置errno

 

所需要结构体：  
	struct sigaction 
	{
		void (*sa_handler)(int);  // 信号处理函数
		void (*sa_sigaction)(int, siginfo_t *, void *); //如果需要通过信号传输某些数据，就需要使用这个信号处理函数，可以不给值
		sigset_t sa_mask; //当前信号处理函数执行期间需要阻塞的信号，不清楚绑定的信号还是还是信号处理函数，没事过，应该是绑定了信号，一般来说初始化一下这个集合即可
		int sa_flags;       //传0即可，表示使用默认标识
		void (*sa_restorer)(void); //已经废弃不在使用，不用给值
	};

 



注意：
	​ SIGKILL, SIGSTOP 这两个信号 不允许被捕捉, 阻塞和忽略
    ​ 处理当前信号过程中会自动阻塞当前信号
~~~





 

 

### 给指定进程发送信号：

~~~c
#include <sys/types.h>
#include <signal.h>
int kill(pid_t pid, int sig);    

参数： 
	pid：
		pid>0:发送信号给指定程序
		pid=0:发送信号给同一进程组的所有进程
		pid<-1:不会
		pid=-1:发送信号给，进程有权限发送的，系统中所有进程
	sig:信号编号，建议使用宏，不同操作系统可以编号不同

返回值:
	成功返回：0
	失败放回：-1，设置errno
~~~





 

 

### 指定时间后发送SIGALRM信号

~~~c
#include <unistd.h>
unsigned int alarm(unsigned int seconds);
参数:
	seconds:几秒后发送信号

返回值：
	若先前没有调用过alarm函数, 则返回0;
	若先前有alarm函数调用, 返回上一次剩余的秒数

注意：
	​ 使用自然定时法
	​ 一个进程只有一个时钟, 若多次调用, 最后一次是有效的
	​ alarm(0);就是取消时钟的意思
~~~





 

 

### 指定时间后发送信号

~~~c
//对比上个函数，这个可以重复发送信号
int setitimer(int which, const struct itimerval *new_value,struct itimerval *old_value);

参数：
	which:                  
		ITIMER_REAL：使用自然定时法，真实时间，建议使用，发送的是SIGALRM信号
		ITIMER_VIRTUAL：只计算进程占用cpu的时间，发送的是SIGVTALRM信号
		ITIMER_PROF：计算占用cpu及执行系统调用的时间，发送的是SIGPROF信号
	new_value: 传入参数, 设置时钟触发的时间
	old_value: 传出参数, 先前的设置, 通常传NULL.

返回值：
	成功返回：0
	失败返回：-1，设置errno

所需结构体：
	struct itimerval 
	{
    	struct timeval it_interval;   //以后每次触发的间隔 
   		struct timeval it_value;     //第一次触发的时间
	};
	struct timeval 
	{

   		time_t   tv_sec;   //秒
   		suseconds_t tv_usec;  //微秒
	};   
~~~





 

 

### 未决信号集和阻塞信号集

>- 信号被发送到程序内核的未决信号集，此时未决信号集会与阻塞信号集进行比较，如果信号未被阻塞，会被送达给程序
>
>- 因为每个信号只占用一个位的空间，只能表示 存在 或者 不存在 两种可能，所以程序收到多个相同未决信号，只有一个会被保存在未决信号集中
>
>- 子进程会拷贝父进程里面的阻塞信号集，未决信号集没测过
>
>- 子进程会拷贝父进程里面的信号捕捉和处理



 ~~~c
#include <signal.h>

所需结构体：
	typedef struct
 	{
   		nsigned long int __val[_SIGSET_NWORDS];
    }__sigset_t;

 

//信号集创建
sigset_t set;

//信号集置0,表示不阻塞(初始化操作)
int sigemptyset(sigset_t *set);

//信号集置1
int sigfillset(sigset_t *set);

//将某个信号加入信号集
int sigaddset(sigset_t *set, int signum);   

//将某个信号移出信号集
int sigdelset(sigset_t *set, int signum);       

//读取当前进程的未决信号集
int sigpending(sigset_t *set);

以上函数返回值：
	成功返回：0
	失败返回：-1，设置errno

 

//判断某个信号是否在信号集中
int sigismember(const sigset_t *set, int signum);
返回值：
	在返回：1
	不在返回：0
	错误返回：-1，设置errno

 

//设置阻塞信号集（生效）
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
参数：
	how:处理方式
		SIG_BLOCK: 将用户设置的阻塞信息添加到内核中, 内核中原来的数据不变 mask |= set
		SIG_UNBLOCK: 清除用户在内核中设置的数据, mask &= ~set
		SIG_SETMASK: 用户传入信号集，覆盖内核阻塞信号集
	set: 传入参数，是一个自定义信号集合。由参数how来指示如何修改当前信号屏蔽字。
    oldset：传出参数，保存旧的 内核阻塞信号集

返回值：
	成功返回:0
	失败返回:-1,设置erro
 ~~~



![image-20200831160525742](D:\zm\note\Readme.assets\image-20200831160525742.png)

​                                

 

 

 

 

 

 

 

 

 

### 子进程结束信号SIGCHLD

>产生SIGCHLD信号的条件
>
>​	子进程收到SIGSTOP信号
>
>​	子进程收到SIGCONT信号
>
>​	子进程终止



 >产生SIGCHLD信号的条件
 >
 >​	子进程死了, 自杀, 他杀
 >
 >​	子进程被暂停了
 >
 >​	子进程由暂停状态重新恢复运行



> 以上三种情况子进程都会给父进程发送信号: SIGCHLD, 父进程默认处理动作是忽略这个信号

 

~~~c
//通过SIGCHLD信号回收子进程小案例
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

int ii = 0;

//处理信号的函数
void my_waitpid(int signo)
{
	int wpid = -1;
    
  	//回收子进程
  	while (( wpid = waitpid(-1,NULL, WNOHANG)) !=0 && wpid != -1 )
		printf("回收了[%d]个进程，进程pid=[%d]\n", ++ii, wpid);
} 

 

int main()
{
	int i = 0;
	pid_t pid = 0;

	//捕捉子进程死亡信号
	signal(SIGCHLD, my_waitpid);

dzz:
    
  	//主进程创建1000个子进程
	pid = fork();
	if (pid > 0)
	{
		if (++i < 1000)
			goto dzz;
    
		while (ii<1000){}

 		printf("程序结束所有子进程已回收\n");
	}

 

	//子进程做一下打印
    if (pid == 0)
    	printf("第[%d]个子进程产生，PID=[%d]\n",i+1,getpid());

	
    return 0;
}
~~~



## 线程

### 线程说明

>- 线程没有独立的虚拟地址空间，多个子线程和主线程共享同一个地址空间
>
>- 线程是系统调度执行的最小单位，会抢CPU资源
>
>- 线程不是linux系统的库函数，是第三方库函数
>
>- 线程拥有的一些独立资源
>  - 线程id
>  - 处理器现场和栈指针（内核栈）
>  - 独立的栈空间（用户空间栈）
>  - errno 变量
>  - 阻塞信号集
>
>- 多个线程同一时间操作同一个数据时，需要加锁，防止数据异常
>
>- 多个线程间通信 “条件变量”、“信号量”
>
>- 子线程执行中发生异常会导致整个进程死亡



​    

### 创建线程

~~~c
#include <pthread.h>
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,void *(*start_routine) (void *), void *arg);

参数：
	thread：传出参数保存线程ID
	attr:线程属性，设置线程是否分离状态，传NULL默认就是不分离
		//定义线程属性变量
		pthread_attr_t attr;
		//初始化线程属性变量
		pthread_attr_init(&attr);
		//设置线程属性为分离状态
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		//释放线程属性
		ret = pthread_attr_destroy(&attr);
	void *(*start_routine) (void *)：线程执行的函数
	arg：需要传入线程的参数，给第三个参数传参

返回值:
	成功返回：0
	失败返回：错误号, 可以使用strerror函数打印错误原因 
~~~





 

### 使当前线程结束

~~~c
#include <pthread.h>
void pthread_exit(void *retval);

功能：线程退出时使用这个函数，不要使用return，主线程使用return 会使其他线程也结束

参数：
	retval：退出状态（需要使用全局或者申请的堆空间），不关心退出状态传NULL

注意：
	​ 是在需要退出的线程处调用
	​ 主线程退出必须使用这个函数否则其他线程也会死亡
~~~



 

### 获取当前的线程ID

~~~c
#include <pthread.h>
pthread_t pthread_self(void);

返回值：返回线程的ID
~~~





 

### 回收子线程资源

~~~c
#include <pthread.h>
int pthread_join(pthread_t thread, void **retval);

功能：子线程运行结束后，主线程负责回收子进程的资源，设置线程分离后，不需要主线程回收子资源

参数：
	thread：需要回收的子线程ID
	retval：退出状态 pthread_exit函数的参数，在这接收，不关心退出状态可以传NULL

返回值：
	成功返回：0
	失败: 返回错误号, 可以使用strerror函数打印错误原因

注意：
	分离后的线程无法阻塞等待退出
	子线程未结束函数会阻塞等待子线程退出
~~~





 

 

### 设置线程分离

~~~c
#include <pthread.h>
int pthread_detach(pthread_t thread);

功能：设置线程分离，子线程退出后不需要主线程回收资源

参数：
	thread：线程ID

返回值:
	成功: 返回0
	失败: 返回错误号, 可以使用strerror函数打印错误原因
~~~





 

 

### 取消线程

~~~c
#include <pthread.h>
int pthread_cancel(pthread_t thread);

功能：在主线程中，取消一个执行中的子线程

参数：
	thread：需要取消的线程ID

返回值:
	成功: 返回0
	失败: 返回错误号, 可以使用strerror函数打印错误原因

注意:
	线程取消不是立即执行的，需要等待线程到达某个取消点(检查点)，可粗略认为一个系统调用(进入内核)即为一个取消点，调用pthread_testcancel函数设置一个取消点。
~~~





 

 

### 比较两个线程是否相同

~~~c
#include <pthread.h>
int pthread_equal(pthread_t t1, pthread_t t2)；

参数：
	t1：需要比较的线程ID
	t2：需要比较的线程ID

返回值：
	两个线程相同返回：非0 值
	两个线程不同返回：0

注意：
	pthread_t线程ID，在不同的平台下可能封装不同，有些平台下pthread_t线程ID，是一个结构体，所以比较线程需要使用这个函数
~~~





 

 

### 线程同步

>多个线程同时访问一个数据，会出现数据异常所以需要线程同步



#### 互斥锁

>说明：多个线程同时对一个数据操作时，会导致异常，所以需要访问共享数据时，建议加锁，某个线程在访问时，其他线程阻塞等待

~~~c
//定义一把锁 
pthread_mutex_t mutex;

//初始化锁（第二个参数可以传NULL）
int pthread_mutex_init(pthread_mutex_t *restrict mutex,const pthread_mutexattr_t *restrict attr);

参数：
	mutex：传入一把锁
	attr：设置锁的属性，不设置可以传NULL

返回值：
	成功返回：0
	失败返回：错误号

 

//加锁
int pthread_mutex_lock(pthread_mutex_t *mutex);

参数：
	mutex：传入一把锁

返回值：
	成功返回：0
	失败返回：错误号

 

//解锁
int pthread_mutex_unlock(pthread_mutex_t *mutex); 

参数：
	mutex：传入一把锁

返回值：
	成功返回：0
	失败返回：错误号


//释放互斥锁

int pthread_mutex_destroy(pthread_mutex_t *mutex);

参数：
	mutex：传入一把锁

返回值：
	成功返回：0
	失败返回：错误号

注意：
	​ 线程对某个数据加锁前，最好释放当前的锁，防止死锁情况出现
	​ 线程对某个数据加锁前，请留意加锁代码阻塞情况，防止死锁情况出现
~~~



**代码示例**

 ~~~c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <pthread.h>
#define N 10000

int n = 0;

//定义一把锁
pthread_mutex_t  mutex;

//线程执行代码 数数
void* numadd(void* temp)
{
	//加锁
	pthread_mutex_lock(&mutex);

	for (int i = 0; i < 10000; i++)
		++n;

	//解锁
	pthread_mutex_unlock(&mutex);

	pthread_exit(NULL);
}

int main()
{
	//保存线程号
	pthread_t thread[N];
	//初始化锁
	pthread_mutex_init(&mutex,NULL);

	//创建线程
	for (int i = 0; i < N; i++)
	{
		if (pthread_create(&thread[i],
			NULL,numadd,NULL))
			return -1;//出错退出所有进程
	}

	//回收线程
	for (int i = 0; i < N; i++)
	{
		pthread_join(thread[i],NULL);
	}
	
	printf("%d\n",n);

	//释放锁
	pthread_mutex_destroy(&mutex);

	getchar();
	//退出主线程
	pthread_exit(NULL);
}
 ~~~

 

 

#### 读写锁

> 说明：读写锁时一种特别的锁，读时可以并行，写时独占

 ~~~c
//读写锁的使用步骤:

//定义一把读写锁:
pthread_rwlock_t wrlock;

//初始化读写锁:
int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock,const pthread_rwlockattr_t *restrict attr);

参数:
	rwlock: 读写锁
	attr: 读写锁属性, 使用默认属性, 设置为: NULL

返回值：
	成功返回：0
	失败返回：错误号

 

//添加读锁:
int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock); //

函数功能：加读锁

参数：
	rwlock：传入锁

返回值：
	成功返回：0
	失败返回错误号：错误号

 

//添加写锁:
int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);

函数功能：加写锁

参数：
	rwlock：传入锁

返回值：
	成功返回：0
	失败返回错误号：错误号

 

//解锁
int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);

参数：
	rwlock：传入锁

返回值：
	成功返回：0
	失败返回错误号：错误号

 

//释放读写锁:
int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);

参数：
	rwlock：传入锁

返回值：
	成功返回：0
	失败返回错误号：错误号

 

注意：
	​ 死锁：访问其他锁时建议解开自己的锁
	​ 读写锁：读并行，写独占，当读写同时等待锁的时候写的优先级高   
    ​ 适合于对数据结构读的次数远大于写的情况
 ~~~





 

#### 条件变量

>说明：生产者和消费者模型，使用条件变量，生产者生产出商品后通知消费者消费，没有商品时消费者阻塞等待，防止供不应求

 ~~~c
//条件变量相关函数和使用步骤:

//定义条件变量:
pthread_cond_t cond;

//初始化条件变量:
int pthread_cond_init(pthread_cond_t *restrict cond,const pthread_condattr_t *restrict attr);

参数: 
	cond: 条件变量的地址
	attr: 使用默认属性, 这个值设置为NULL

返回值：
	成功返回：0
	失败返回：错误号

 

//线程等待唤醒
int pthread_cond_wait(pthread_cond_t *restrict cond,pthread_mutex_t *restrict mutex);

函数功能：调用该函数阻塞，同时自动解锁，等待signal 和 broadcast函数唤醒后，自动加锁 并解除阻塞 运行

参数:
	cond: 条件变量
	mutex: 传入锁 

返回值：
	成功返回：0
	失败返回：错误号

注意：
	​ 醒后，执行前在使用while 循环判断一下，防止同时唤醒了多个线程

 

//唤醒被wait 函数阻塞的一个或多个线程
int pthread_cond_signal(pthread_cond_t *cond);
参数：
	cond: 条件变量

返回值：
	成功返回：0
	失败返回：错误号

注意：
	​ 可以使用 pthread_cond_broadcast 唤醒所有的阻塞在 pthread_cond_wait / pthread_cond_timedwait 函数上的线程。

 

//释放条件变量
int pthread_cond_destroy(pthread_cond_t *cond);
参数：
	cond：条件变量

返回值：
	成功返回：0
	失败返回：错误号
 ~~~



**代码示例**

~~~c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <pthread.h>
#include <unistd.h>


typedef struct DATA
{
	int num;
	struct DATA* next;
}DATA;

//初始化head为NULLL
DATA* head = NULL;
//定义锁
pthread_mutex_t  mutex;
//定义条件变量
pthread_cond_t cond;

//生产者线程代码
void* new_num(void* data)
{
	//静态变量，生产者线程共享，统计生产次数
	static int new_record=0;
	while (1)
	{
		//生产一个数据
		DATA* temp_data = (DATA*)malloc(sizeof(DATA));
		temp_data->num = rand() % 1000 + 1;

		//加锁
		pthread_mutex_lock(&mutex);

		//新的数据指向旧数据的地址
		temp_data->next = head;
		//head指向新的数据的地址
		head = temp_data;
		//打印生产成功
		printf("累计生产了[%d]个，目前生产的是[%d]\n", ++new_record,temp_data->num);

		//解锁
		pthread_mutex_unlock(&mutex);

		//通知消费线程
		pthread_cond_signal(&cond);
		usleep(100000);
	}

	return NULL;
}

//消费者线程代码
void* dele_num(void* data)
{
	//静态变量，消费者线程共享，统计消费次数
	static int dele_record=0;
	while (1)
	{
		//加锁
		pthread_mutex_lock(&mutex);
		
		//head为空，阻塞等待生产者线程通知
		while (head == NULL)
			//如果当前没有数据，阻塞线程 自动释放锁
			//如果收到 pthread_cond_signal 信号，解除阻塞 自动加锁 
			pthread_cond_wait(&cond, &mutex);

		//打印当前消费的数据,头查，多个数据的情况下，会先释放最后一个生产的，可以同过判断NULL，按生产顺序释放
		printf("累计使用了[%d]个，当前使用的是 [%d] \n", ++dele_record,head->num);
		//保存要释放的地址
		DATA* temp_data = head;
		//head指向下一个数据
		head = head->next;
		//释放当前数据
		free(temp_data);
		
		//解锁
		pthread_mutex_unlock(&mutex);
	}

	return NULL;
}

int main()
{
	//初始化锁
	pthread_mutex_init(&mutex,NULL);
	//初始化条件变量
	pthread_cond_init(&cond,NULL);

	//保存线程号
	pthread_t new_thread[3];
	pthread_t dele_thread[5];

	//创建线程
	for (int i = 0; i < 5; i++)
	{
		//创建消费者线程
		if (pthread_create(&dele_thread[i],NULL, dele_num, NULL))
			return -1;//出错退出所有线程

		//创建生产者线程
		if (i < 3)
		{
			if (pthread_create(&new_thread[i], NULL, new_num, NULL))
				return -1;//出错退出所有线程
		}
	}

	//回收线程
	for (int i = 0; i < 8; i++)
	{
		pthread_join(dele_thread[i],NULL);
		if (i < 4)
			pthread_join(new_thread[i], NULL);
	}


	getchar();
	pthread_exit(NULL);
}
~~~





#### 信号量

>说明：生产者和消费者模型，使用信号量，生产出商品后 ++ ，消费后 -- ，防止供不应求，++,--函数自带锁

~~~c
include <semaphore.h>

 

//信号量的使用

//1、定义信号量
sem_t sem;

 

//2、初始化信号量
int sem_init(sem_t *sem, int pshared, unsigned int value);
参数:
	sem: 信号量
	pshared: 传 0 表示用于多线程, 非 0 值表示用于多进程
	value: 信号量的初始值，小于等于 0 ，-- 函数阻塞

返回值:
	成功: 返回0
	失败: 返回-1, 并设置errno

 

//3、++ 函数
int sem_post(sem_t *sem);
功能: 调用该函数一次, 相当于sem++
参数:
	sem: 信号量

返回值: 
	成功: 返回0
	失败: 返回-1, 并设置errno

 

//4、-- 函数
int sem_wait(sem_t *sem);
功能: 调用该函数一次, 相当于sem--, 当sem为 0 的时候, 引起阻塞
参数:
	sem: 信号量

返回值: 
	成功: 返回0
	失败: 返回-1, 并设置errno
 

//5、释放信号量   
int sem_destroy(sem_t *sem);
功能: 释放信号量资源
参数:
	sem: 信号量

返回值: 
	成功: 返回0
	失败: 返回-1, 并设置errno
~~~



**代码示例**：

~~~c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include <semaphore.h>

typedef struct DATA
{
	int num;
	struct DATA* next;
}DATA;

//初始化head为NULLL
DATA* head = NULL;

//初始锁
pthread_mutex_t  mutex;

//定义信号量
sem_t dele_sem;

//生产者线程代码
void* new_num(void* data)
{
	//静态变量，生产者线程共享，统计生产次数
	static int new_record = 0;
	while (1)
	{
		//生产一个数据
		DATA* temp_data = (DATA*)malloc(sizeof(DATA));
		temp_data->num = rand() % 1000 + 1;

		//加锁
		pthread_mutex_lock(&mutex);

		//新的数据指向旧数据的地址
		temp_data->next = head;
		//head指向新的数据的地址
		head = temp_data;
		//打印生产成功
		printf("累计生产了[%d]个，目前生产的是[%d]\n", ++new_record, temp_data->num);

		//解锁
		pthread_mutex_unlock(&mutex);

		//调用 ++ 函数
		sem_post(&dele_sem);
	}

	return NULL;
}

//消费者线程代码
void* dele_num(void* data)
{
	//静态变量，消费者线程共享，统计消费次数
	static int dele_record = 0;
	while (1)
	{
		//调用 -- 函数
		sem_wait(&dele_sem);

		//加锁
		pthread_mutex_lock(&mutex);

		//打印当前消费的数据,头查，多个数据的情况下，会先释放最后一个生产的，可以同过判断NULL，按生产顺序释放
		printf("累计使用了[%d]个，当前使用的是 [%d] \n", ++dele_record, head->num);
		//保存要释放的地址
		DATA* temp_data = head;
		//head指向下一个数据
		head = head->next;
		//释放当前数据
		free(temp_data);

		//解锁
		pthread_mutex_unlock(&mutex);


	}

	return NULL;
}

int main()
{
	//初始化信号量和锁
	if (sem_init(&dele_sem, 0, 0) && pthread_mutex_init(&mutex,NULL))
		return -1;


	//保存线程号
	pthread_t new_thread[4],dele_thread[8];

	//创建线程
	for (int i = 0; i < 8; i++)
	{
		//创建消费者线程
		if (pthread_create(&dele_thread[i], NULL, dele_num, NULL))
			return -1;//出错退出所有线程

		//创建生产者线程
		if (i < 4)
		{
			if (pthread_create(&new_thread[i], NULL, new_num, NULL))
				return -1;//出错退出所有线程
		}
	}

	//回收线程
	for (int i = 0; i < 8; i++)
	{
		pthread_join(dele_thread[i], NULL);
		if (i < 4)
			pthread_join(new_thread[i], NULL);
	}

	//释放信号量
	if (sem_destroy(&dele_sem))
		return -1;


	pthread_exit(NULL);
}
~~~

 

#  Linux网络编程

## 辅助函数

### 大小端转换

~~~c
#include <arpa/inet.h>
uint16_t htons(uint16_t hostshort);
功能：小端数据转大端、短整型转换、小端也叫主机字节序，大端也叫网络字节序
参数：
	hostshort：传入端口号

返回值：
	成功返回：转换之后的数据
	失败返回：man没写失败情况

注意：
	​ 大端数据转小端 ntohs() 短整型，用于端口号
	​ atoi()：把字符串转换为整型，返回值没写失败情况
	​ uint32_t htonl(uint32_t hostlong); 长整型小端转大端，用于IPV4
    ​ uint32_t ntohl(uint32_t netlong); 长整型大端转小端，用于IPV4
	​ 这是整型转换，字符串配合其他函数使用 或者 使用其他函数
	​ 需要什么字节序直接转，如果字节序正确那结果不会改变
~~~





 

 

### 点分制十进制字符串转IP地址

~~~c
#include <arpa/inet.h>
int inet_pton(int af, const char *src, void *dst);
功能：把点分制十进制字符串，转为网络字节序的整型IP地址
参数：
	af：地址族协议
		AF_INET：IPV4
		AF_INET6：IPV6
	src：点分制十进制字符串
	dst：传出参数，保存转换后的IP地址

返回值：
	成功返回：1
	失败返回：-1
	传入的点分十进制字符串无效返回：0

 

#include <arpa/inet.h>
const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
功能：把网络字节序转为，点分十进制字符串
参数：
	af：地址族协议
		AF_INET：IPV4
		AF_INET6：IPV6
	src：网络字节序的整型IP地址
	dst：装换成功后的 点分十进制字符串 存储位置
	size：第三个参数dst 的内存大小

返回值：
	失败返回：NULL
	成功返回：非空指针，指向第三个参数的指针
~~~





 

 

### 利用域名获取IP地址

~~~c
#include <netdb.h>
extern int h_errno;
struct hostent *gethostbyname(const char *name);
参数：
	name：域名字符串

返回值：
	成功返回：保存ip信息的结构体
	失败返回：NULL

hostent结构体：
	struct hostent
	{
		char *h_name; 官方域名(Official domain name)。
		char **h_aliases;多个域名可以访问统一主页，同一IP也可以绑定多个域名，保存除了官方域名外的其他域名。
		int h_addrtype; Ipv4或者Ipv6保存在这里
		int h_length;保存IP地址的长度
		char **h_addr_list; 保存域名对应的IP地址
	}

 
//示例代码：
struct hostent * host = gethostbyname("www.naver.com");
printf("官方域名：%s\n",host->h_name);
for (int i = 0; host->h_aliases[i]; i++)
	printf("其他域名：%s\n",host->h_aliases[i]);
printf("ip类型：%s\n",(host->h_addrtype==AF_INET)?"AF_INET":"AF_INET6");
for (int i = 0; host->h_addr_list[i]; i++)
	printf("ip地址：%s\n", inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
~~~

​                               

 

### 利用IP获取域名

~~~c
#include <netdb.h>
#include <sys/socket.h>    /* for AF_INET */
extern int h_errno;

struct hostent *gethostbyaddr(const void *addr,socklen_t len, int type);
功能：传入IP地址，返回域名
参数：
	addr：含有IP地址的in_addr结构体指针。
	len：IPV4传4，IPV6传16
	type：IPV4传AF_INET,IPV6传AF_INET6

返回值：
	成功返回：保存ip信息的结构体
	失败返回：NULL

注意：
	这个函数好像需要某种服务才可以使用，一般情况下函数调用不成功，返回NULL,可以尝试IP地址为127.0.0.1

//示例代码：
struct sockaddr_in addr;
memset(&addr,0,sizeof(addr));
addr.sin_addr.s_addr = inet_addr("220.181.57.216");
struct hostent * host = gethostbyaddr((char*)&addr.sin_addr.s_addr,4,AF_INET);
printf("官方域名：%s\n", host->h_name);
for (int i = 0; host->h_aliases[i]; i++)
	printf("其他域名：%s\n", host->h_aliases[i]);
printf("ip类型：%s\n", (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");
for (int i = 0; host->h_addr_list[i]; i++)
	printf("ip地址：%s\n", inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
~~~





 

 

### 半关闭函数

~~~c
#include <sys/socket.h>
int shutdown(int sockfd, int how);
参数：
	sockfd：需要关闭的套接字
	how：关闭的方式
		SHTU_RD：断开输入流
		SHUT_WR：断开输出流
		SHUT_RDWR：同时断开I/O流

返回值：
    成功返回：0
	失败返回：-1
~~~





 

 

### 端口复用

~~~c
#include <sys/types.h> 
#include <sys/socket.h>
// 设置套接字属性
int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
功能：这里只介绍端口复用，端口复用需要在bind前调用。
参数：
	sockfd: 要操作的套接字
	optname: 端口复用的级别(二选一)
		SO_REUSEADDR
		SO_REUSEPORT
	optval: 端口复用-> 对应的是整形数
		1: 可以复用
		0: 不能复用
	optlen: optval参数的内存大小

返回值：
	成功返回：0
	失败返回：-1，设置errno

注意：
	​ TCP通信时主动关闭连接方，在四次挥手时，会等待2MSL时间，才会关闭程序，这个时间会占用端口
	​ 设置端口复用后，只有最后一个bind这个端口的程序能使用这个端口
~~~





## TCP通信

### tcp通信说明

- **TCP通信流程图**

​                    ![image-20200831170300600](D:\zm\note\Readme.assets\image-20200831170300600.png)           

 

- **TCP套接字缓冲区**
  - write写满时会阻塞
  - read 读时没有数据也会阻塞

 ![image-20200831170350585](D:\zm\note\Readme.assets\image-20200831170350585.png)

 

- **TCP通信是面向连接的，安全的，流式传输协议。TCP建立连接时会3次握手，断开连接时会4次挥手**

 

### 创建套接字

~~~c
#include <sys/types.h>     /* See NOTES */
#include <sys/socket.h>
int socket(int domain, int type, int protocol);
参数：
	domain：地址族协议
		PF_INET：ipv4互联网协议族
		PF_INET6：ipv6互联网协议族
		PF_LOCAL：本地通信的UNIX协议族
		PF_PACKET：底层套接字协议族
		PF_IPX：IPX Novell协议族
	type：通信过程中使用的协议
		SOCK_STREAM：流式协议（TCP）
		SOCK_DGRAM：报式协议（UDP）
	protocol：具体的通信过程中使用的协议
		0：一般写0，上个参数中的协议，默认就是TCP和UDP
		IPPROTO_TCP：满足ipv4和面向连接的协议
		IPPROTO_UDP：满足ipv4和面向消息的协议

返回值：
	成功返回：文件描述符
	失败返回：-1

 

注意：
	​ domain：套接字中的协议族信息，也可以把PF开头换成AF开头
~~~





 

 

### 给套接字分配（绑定）ip地址和端口

~~~c
#include <sys/types.h>     /* See NOTES */
#include <sys/socket.h>
int bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
参数：
	sockfd：用于监听的服务器端套接字
	addr：存有地址和端口的结构体
		一般使用sockaddr_in结构体，使用时强转
	addrlen：第二个结构体的长度
    
返回值：
	成功返回：0
	失败返回：-1

 
所需结构体：
头文件：
#include<arpa/inet.h>
struct sockaddr_in
{
	sa_family_t sin_family;  //地址族
		AF_INET：ipv4网络协议中使用的地址族
		AF_INET6：ipv6网络协议中使用的地址族
		AF_LOCAL：本地通信中采用的UNIX协议地址族         
	uint16_t sin_port; //TCP/UDP端口号
		htons()：转大端
		atoi()：把字符串转换为整型
	struct in_addr sin_addr;    //32位（四个字节）IP地址
	char sin_zero[8]; //不使用，但是必须置0，使用memset
}

struct in_addr
{
	In_addr_t s_addr;  //32位ip地址
		htonl()：转大端
			INADDR_ANY：自动获取服务器端的IP的地址（整型）
		点分制转大端：int inet_pton(int af, const char *src, void *dst);
			AF_INET：
			AF_INET6
			成功返回：1
			失败返回：-1
			传入的点分十进制字符串无效返回：0
}

注意：
	测试时使用IP地址不是自动获取的情况下，连接时也不能使用127.1
~~~





 

 

### 进入等待连接请求状态

~~~c
#include <sys/types.h>     /* See NOTES */
#include <sys/socket.h>
int listen(int sockfd, int backlog);
参数：
	sockfd：用于监听的服务器端套接字
	backlog：等待处理的连接队列的长度
		指定的数值不能大于 /proc/sys/net/core/somaxconn 中存储的数据, 默认为128

返回值：
	成功返回：0
	失败返回：-1
~~~





 

 

### 受理客服端连接请求

~~~c
#include <sys/types.h>     /* See NOTES */
#include <sys/socket.h>
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
参数：
	sockfd：服务器端套接字
	addr：传入一个sockaddr_in结构体用于存储客服端的地址信息
	addrlen：第二个结构体的内存大小，需要socklen_t类型的指针，所以请在外面定义，直接sizeof不行

返回值：
	成功返回：用于和客服端通信的套接字
	失败返回：-1

注意：
	函数带阻塞
~~~



### 客服端请求连接

~~~c
#include <sys/types.h>     /* See NOTES */
#include <sys/socket.h>
int connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
参数：
	sockfd：创建好的客服端套接字
	addr：传入保存有服务器IP和端口sockaddr_in结构体， 
	addrlen：第二个结构体的长度

返回值：
	成功返回：0
    失败返回：-1,      
~~~





##  UDP通信

### UDP通信说明

- **通信流程图**

​                          ![image-20200831171254363](D:\zm\note\Readme.assets\image-20200831171254363.png)     

 

- **UDP通信时面向无连接的，不安全的，报式传输协议，效率高。**

 

### UDP发送数据

~~~c
#include <sys/types.h>
#include <sys/socket.h>
ssize_t sendto(int sockfd, const void *buf,size_t len,int flags,const struct sockaddr *dest_addr,socklen_t addrlen);

参数：
	sockfd：用于传输数据的UDP套接字
	buf：需要传输的数据
	len：传输多少字节的数据
	flags：可选项参数，传0即可
	dest_addr：存有目标地址信息的结构体
	addrlen：dest_addr的大小

返回值：
	成功返回：传输的字节数
	失败返回：-1
~~~





 

 

### UDP接收数据

~~~c
#include <sys/types.h>
#include <sys/socket.h>
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,struct sockaddr *src_addr, socklen_t *addrlen);
功能：阻塞等待接收数据
参数：
	sockfd：用于接收数据的UDP套接字
	buf：接收到的数据
	len：接收多少字节的数据
	flags：可选项参数，若没有则传递0
	src_addr：保存发送端地址信息的结构体，不需要可以传NULL
	addrlen：结构体src_addr的大小

返回值：
	成功返回：接收到的字节数
	失败返回：-1
~~~





 

### UDP通信示例

**服务器**

~~~c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include<arpa/inet.h>

/*
	UDP,接收数据，并回发
*/
int main()
{
	//创建UDP通信套接字
	int cfd;
	if ((cfd=socket(PF_INET,SOCK_DGRAM,0))== -1)
	{
		perror("socket()");
		exit(-1);
	}

	//bind端口和ip
	struct sockaddr_in s_addr;
	memset(&s_addr,0,sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(8000);
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(cfd,(struct sockaddr*)&s_addr, sizeof(s_addr)) == -1)
	{
		perror("bind()");
		exit(-1);
	}


	//阻塞等待数据接收
	char buf[1024] = {0};
	struct sockaddr_in c_addr;
	int c_addr_siz = sizeof(c_addr);
	memset(&c_addr, 0, c_addr_siz);
	int ret_recvfrom;
goto_recvfro:
	ret_recvfrom = recvfrom(cfd, buf, sizeof(buf), 0, &c_addr, &c_addr_siz);
	if (ret_recvfrom == -1 )
	{
		perror("recvfrom()");
		exit(-1);
	}

	//发送数据
	if (ret_recvfrom > 0)
	{
		printf("接收到[%d]个字节的数据：[%s]\n",ret_recvfrom,buf);
		int ret_sendto;
		ret_sendto = sendto(cfd,buf,sizeof(buf),0,&c_addr,c_addr_siz);
		if (ret_sendto == -1)
		{
			perror("sendto()");
			exit(-1);
		}

		if (ret_sendto > 0)
			printf("发了[%d]个字节的数据：[%s]\n", ret_sendto, buf);
	}

	goto goto_recvfro;

	exit(-1);
}
~~~



**客服端**

~~~c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include<arpa/inet.h>
#include <unistd.h>

/*
	UDP,发送数据，在接收回发的数据
*/
int main(int argc, char** argv)
{
	if (argc != 3 )
	{
		perror("argc");
		exit(-1);
	}

	//创建UDP通信套接字
	int cfd;
	if ((cfd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("socket()");
		exit(-1);
	}

	//初始化目标信息
	struct sockaddr_in s_addr;
	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(atoi(argv[2]));
	if (inet_pton(AF_INET, argv[1], &s_addr.sin_addr.s_addr) != 1)
	{
		perror("inet_pton()");
		exit(-1);
	}
	int s_addr_siz = sizeof(s_addr);
	
	//读取键盘输入
	char buf[1024] = {0};
	read(0,buf,sizeof(buf));

	//发送数据
	int ret_sendto;
	ret_sendto = sendto(cfd, buf, sizeof(buf), 0, &s_addr, s_addr_siz);
	if (ret_sendto == -1)
	{
		perror("sendto()");
		exit(-1);
	}
	if (ret_sendto > 0)
		printf("发了[%d]个字节的数据：[%s]\n", ret_sendto, buf);

	
	//阻塞接收数据
	memset(buf,0,sizeof(buf));
	int ret_recvfrom;
	ret_recvfrom= recvfrom(cfd,buf,sizeof(buf),0,NULL,NULL);
	if (ret_recvfrom == -1)
	{
		perror("recvfrom()");
		exit(-1);
	}
	if (ret_recvfrom > 0 )
		printf("接收到了[%d]个字节的数据：[%s]\n",ret_recvfrom,buf);


	exit(0);
}
~~~





## 案例：

### 多进程服务器

> 说明：使用多进程实现并发访问

~~~c
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
~~~



 

### 多线程服务器

> 说明：使用多线程实现并发访问

~~~c
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
~~~



 

### select IO多路转接

>- 使用select函数监视套接字的变化，实现并发访问。
>
>- select可以跨平台在windows使用。
>
>- fd_set集合限制，最多监视1024个套接字



~~~c 
#include <sys/select.h>
/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int select(int nfds, fd_set *readfds, fd_set *writefds,fd_set *exceptfds,struct timeval *timeout);
参数：
	nfds：需要监听的文件描述符数量，当前创建出来的最大套接字ID+1，或者直接1024
	readfds：监听读缓冲区是否有数据，调用时传入需要监听的套接字集合，结束时返回读缓冲区有数据的集合，不关心可以传NULL
	writefds：监听写缓冲区是否可以写入数据(缓冲区没满就可以写)，调用时传入需要监听套接字集合，结束时返回可以写入数据的套接字集合，不关心可以传NULL
	exceptfds：监听套接字是否有异常，使用参照上面两个参数，不关心可以传NULL
	timeval：设置单次监听最长阻塞时间，传NULL一直阻塞监听

返回值：
	失败返回：-1
	成功返回：发生变化的文件描述符数
	返回0：表示本次监听结束没变化的文件描述符

注意：
下列函数可以用于用于操作fd_set类型
void FD_CLR(int fd, fd_set *set); 把某个文件描述符移除
int FD_ISSET(int fd, fd_set *set);判断某个套接字是否存在
	存在返回：1
	不存在返回：0
void FD_SET(int fd, fd_set *set);把某个文件描述符加入
void FD_ZERO(fd_set *set);初始化，置0
~~~



**示例代码：**

~~~c
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
~~~



~~~c
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
~~~



 

### poll

>- poll是linux下select 的简单改进版
>
>- 使用数组管理需要监听的套接字，可以监听超过1024个套接字



 ~~~c
#include <poll.h>
int poll(struct pollfd *fds, nfds_t nfds, int timeout);
功能：和select一样，监听套接字
参数：
	fds：需要监听的数组
	nfds：需要监听多少个套接字
	timeout：阻塞时长
		0：不阻塞
		-1：永久阻塞
		>0：阻塞时长，单位毫秒

返回值：
	失败返回：-1
	成功返回：检测的数组中有多少个文件描述符变化

所需结构体：
struct pollfd
{
	int  fd;     /* 委托内核检测的文件描述符 */
	short events;   /* 委托内核检测文件描述符的什么事件 */
		POLLIN //读缓冲区是否有数据
		POLLOUT//写缓冲区是否能写入
	short revents;  /* 文件描述符实际发生的事件 */
};
 ~~~





**代码示例：**

~~~c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <arpa/inet.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include<unistd.h>
#include<poll.h>

int main(int argc, char** argv)
{

	if (argc != 2)
	{
		printf("argc\n");
		exit(-1);
	}

	//创建负责监听的套接字
	int lfd = socket(AF_INET,SOCK_STREAM,0);
	if (lfd == -1)
	{
		perror("socket()");
		exit(-1);
	}

	//创建结构体保存客服端和服务器的IP、端口
	struct sockaddr_in s_addr;
	struct sockaddr_in c_addr;
	int s_addr_siz = sizeof(s_addr);
	int c_addr_siz = sizeof(c_addr);
	memset(&s_addr,0,s_addr_siz);
	memset(&c_addr,0,c_addr_siz);

	//初始化服务器IP和端口
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(atoi(argv[1]));

	//给监视套接字绑定IP和端口
	if (bind(lfd,(struct sockaddr*)&s_addr,s_addr_siz)==-1)
	{
		perror("bind()");
		exit(-1);
	}

	//开始监听
	if (listen(lfd,128)==-1)
	{
		perror("listen()");
		exit(-1);
	}

	//初始化 pollfd 数组，里面保存需要监听的套接字
	struct pollfd fds[1024];
	for (int i = 0; i < 1024; i++)
	{
		//需要监听的事件
		fds[i].events = POLLIN;
		//需要监听的套接字
		fds[i].fd = -1;
	
	}
	fds[0].fd = lfd;
	//目前需要监听多少套接字
	int max_fd = 1;

	
	while (1)
	{
		//使用poll 监听套接字
		int ret_poll = poll(fds,max_fd,-1);
		if (ret_poll == -1)
		{
			perror("ret_poll()");
			exit(-1);
		}

		//判断是否是 监听连接的套接字 读缓冲区有数据
		if (fds[0].revents & POLLIN)
		{
			//创建和客服端通信的套接字
			int cfd = accept(lfd,(struct sockaddr*)&c_addr,&c_addr_siz);
			if (cfd == -1)
			{
				perror("accept()");
				exit(-1);
			}

			//通信套接字加入监听
			for (int i = 0; i < 1024; i++)
			{
				if (fds[i].fd == -1)
				{
					fds[i].fd = cfd;
					//最大需要监听的套接字数量
					max_fd = (i + 1) > max_fd ? (i + 1) : max_fd;
					break;
				}
			}

			//如果只有一个 监听套接字 缓冲区有数据，就不用遍历下的通信套接字了
			if (--ret_poll <= 0)
				continue;

		}

		//遍历 数组 找出缓冲区准备就绪的套接字
		for (int i = 1; i < max_fd; i++)
		{
			if (fds[i].revents & POLLIN)
			{

				//读取客服端发送数据
					int read_ret;
					char buf[1024] = { 0 };
					read_ret = read(fds[i].fd, buf, sizeof(buf));
					//读取失败
					if (read_ret == -1)
					{
						perror("read()");
						continue;
						//exit(-1);
					}

					//对方断开连接
					else if (read_ret == 0)
					{
						printf("连接已关闭\n");
						close(fds[i].fd);
						
						//移除 监听数组 
						fds[i].fd == -1;
						break;
					}

					//数据写回客服端
					if (!write(fds[i].fd, buf, sizeof(buf)))
					{
						perror("writes()");
						exit(-1);
					}
			}
		}
	}

	close(lfd);
	exit(0);
}
~~~





### epoll 

#### 说明

>- 使用epoll 实现高性能并发服务器
>
>- epoll有两种工作模式，默认使用LT模式：读缓冲区有数据就会通知
>
>- ET模式高性能模式：读缓冲区发生改变才会通知
>
>  - ET模式下一般需要使用while保证数据被读完，所以建议设置套接字为非阻塞模式
>
>  - ET模式只需要在上树时加个宏即可使用
>
>    >#include <unistd.h>
>    >
>    >#include <fcntl.h>
>    >
>    >// 设置套接字为非阻塞
>    >
>    >// 获取套接字flag属性
>    >
>    >int flag = fcntl(cfd, F_GETFL);
>    >
>    >// 添加非阻塞属性
>    >
>    >flag = flag | O_NONBLOCK;
>    >
>    >// 将新的flag属性设置给cfd
>    >
>    >fcntl(cfd, F_SETFL, flag);



#### 函数

 ~~~c
#include <sys/epoll.h>
int epoll_create(int size);
功能：创建一颗红黑树，维护需要监视的套接字
参数: 
	size: 没意义, 随便写个数就行，别写0

返回值;
	成功返回: 文件描述符, 操作epoll树的根节点
	失败返回：-1

 

int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
功能：对epoll 树进行管理：添加、删除节点，修改节点属性
参数:
	epfd: 树的根节点，上一个函数的返回值
	op: 要进行什么样的操作
		EPOLL_CTL_ADD: 注册新节点, 添加到红黑树上
		EPOLL_CTL_MOD: 修改套接字的属性
		EPOLL_CTL_DEL: 从红黑树上删除节点
	fd: 要操作的套接字
	event: 要监视什么操作，删除节点传NULL

返回值：
	成功返回：0
	失败返回：-1，设置ernno

所需结构体：
struct epoll_event {
	uint32_t   events; 要监视的属性,可以位运算同时使用多个宏
		EPOLLIN 监视读
		EPOLLOUT 监视写
		EPOLLERR 监视错误 
		EPOLLET ET触发模式
	epoll_data_t     data;    联合体，在下面
};

typedef union epoll_data {
	void    *ptr; // 可以使用结构体，给每一个节点一些自定义属性和方法（回调方法），但是第一个属性必须是 fd 保证能正确赋值
	int     fd;  //套接字
	uint32_t   u32;
	uint64_t   u64;
} epoll_data_t;

 

int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
功能：阻塞等待系统通知，处理有变化的套接字
参数：
	epfd：树的根节点，第一个函数的返回值
	epoll_event：传出参数，保存发生变化的套接字，使用数组保存多个
	maxevents：第二个参数的数组个数
	timeout：阻塞时间
		0，不阻塞
		-1，一直阻塞
		>0，阻塞时长，毫秒  

返回值：
	成功返回：变化的套接字个数
	失败返回：-1
 ~~~



####  默认LT模式示例

~~~c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include<errno.h>
#include <sys/epoll.h>

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
	epoll_ctl(epfd, EPOLL_CTL_ADD,lfd, &temp_event);

	

	while (1)
	{
		//epoll_wait函数调用
		memset(&events,0,sizeof(events));
		int ret = epoll_wait(epfd, events,sizeof(events)/sizeof(events[0]),-1);

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
			for (int i = 0; i < ret; i++)
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
					temp_event.events = EPOLLIN;
					epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &temp_event);

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
					//读取客服端发送数据
					int read_ret;
					char read_buf[1024] = { 0 };
					read_ret = read(events[i].data.fd, read_buf, sizeof(read_buf));
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
~~~



#### ET模式代码示例

 ~~~c
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
 ~~~



#### ET模式、prt指针的使用

~~~c
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
~~~







### UDP广播

>- 广播强制发送数据，组播可以选择是否接收数据
>
>- 广播就是给局域网内同一网段多个设备发送数据
>
>- 只能在局域网中使用
>
>- 客服端只要绑定了服务器广播使用的端口，就可以收到广播数据

 

**服务器流程：**

>1、创建通信UDP的套接字
>
>2、设置套接字属性为广播，setsockopt();
>
>3、通信，发送广播的数据，(IP地址自动获取)
>
>4、关闭通信的套接字



**客服端：**

>1、创建通信的UDP套接字
>
>2、绑定广播的端口(IP地址自动获取)
>
>3、通信，接收广播数据
>
>4、关闭套接字



 

**代码示例：**

**服务器**

~~~c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include<arpa/inet.h>
#include <unistd.h>
/*
	UDP广播服务端
*/
int main()
{
	

	//创建UDP通信套接字
	int cfd;
	if ((cfd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("socket()");
		exit(-1);
	}

	//设置套接字属性为广播
	int op = 1;
	if (setsockopt(cfd, SOL_SOCKET, SO_BROADCAST, &op, sizeof(op)) ==-1)
	{
		perror("setsockopt()");
		exit(-1);
	}


	//广播发送数据
	struct sockaddr_in s_addr;
	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(8000);
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	char buf[1024] = { 0 };
	int ret_sendto;

	//发送广播数据
	srand(time(NULL));
	while (1)
	{
		sprintf(buf, "广播数据:[%d]", rand()%1000+1);
		ret_sendto = sendto(cfd, buf, sizeof(buf), 0,(struct sockaddr*)&s_addr, sizeof(s_addr));
		if (ret_sendto == -1)
		{
			perror("sendto()");
			exit(-1);
		}

		if (ret_sendto > 0)
			printf("发了[%d]个字节的数据：[%s]\n", ret_sendto, buf);

		sleep(1);
	}
	
	exit(0);
}

~~~



**客服端**

~~~c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include<arpa/inet.h>
#include <unistd.h>

/*
	UDP,广播客服端
*/
int main(int argc, char** argv)
{


	//创建UDP通信套接字
	int cfd;
	if ((cfd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("socket()");
		exit(-1);
	}

	//绑定广播的端口
	struct sockaddr_in s_addr;
	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(8000);
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(cfd, (struct sockaddr_in*) & s_addr, sizeof(s_addr)) == -1)
	{
		perror("bind()");
		exit(-1);
	}


	//读取广播的数据
	char buf[1024] = { 0 };
	int ret_recvfrom;
	while (1)
	{
		ret_recvfrom = recvfrom(cfd, buf, sizeof(buf), 0, NULL, NULL);
		if (ret_recvfrom == -1)
		{
			perror("recvfrom()");
			exit(-1);
		}
		if (ret_recvfrom > 0)
			printf("接收到了[%d]个字节的数据：[%s]\n", ret_recvfrom, buf);
	}



	exit(0);
}
~~~

 

**网上UDP广播资料**

https://blog.csdn.net/weixin_36750623/article/details/83575412

 

 

### UDP组播

**说明：**

​          ![image-20200831180144591](D:\zm\note\Readme.assets\image-20200831180144591.png)                 

 

**通信流程：**

 ![image-20200831180205773](D:\zm\note\Readme.assets\image-20200831180205773.png)

 

**代码示例：**

**服务器**

~~~c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
    // 1. 创建通信的套接字
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd == -1)
    {
        perror("socket");
        exit(0);
    }

#if 0
    // 2. 通信的fd绑定本地的IP和端口
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = INADDR_ANY;  // 0 -> 0.0.0.0
    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret == -1)
    {
        perror("bind");
        exit(0);
    }
#endif

    // 设置组播属性
    struct in_addr imr_multiaddr; 
    // 初始化组播地址
    inet_pton(AF_INET, "239.0.0.10", &imr_multiaddr.s_addr);
    setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, &imr_multiaddr, sizeof(imr_multiaddr));

    // 将数据发送给客户端, 使用广播地址和固定端口
    // 初始化客户端的地址信息
    struct sockaddr_in cliaddr;
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(8989); // 客户端也需要绑定这端口
    inet_pton(AF_INET, "239.0.0.10", &cliaddr.sin_addr.s_addr);

    int num = 0;
    // 3. 通信
    while(1)
    {
        // 接收数据
        char buf[128];
#if 0
        char ipbuf[64];
        struct sockaddr_in cliaddr;
        int len = sizeof(cliaddr);
        recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr*)&cliaddr, &len);
        printf("client IP: %s, Port: %d\n", 
               inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ipbuf, sizeof(ipbuf)),
               ntohs(cliaddr.sin_port));
        printf("client say: %s\n", buf);
#endif

        // 发送数据
        sprintf(buf, "hello, client...%d\n ", num++);
        sendto(fd, buf, strlen(buf)+1, 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
        printf("组播的数据: %s\n", buf);
        sleep(1);
    }

    close(fd);

    return 0;
}
~~~



**客服端**

~~~c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include<arpa/inet.h>
#include <unistd.h>

/*
	UDP,广播客服端
*/
int main(int argc, char** argv)
{


	//创建UDP通信套接字
	int cfd;
	if ((cfd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("socket()");
		exit(-1);
	}

	//绑定广播的端口
	struct sockaddr_in s_addr;
	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(8000);
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(cfd, (struct sockaddr_in*) & s_addr, sizeof(s_addr)) == -1)
	{
		perror("bind()");
		exit(-1);
	}


	//读取广播的数据
	char buf[1024] = { 0 };
	int ret_recvfrom;
	while (1)
	{
		ret_recvfrom = recvfrom(cfd, buf, sizeof(buf), 0, NULL, NULL);
		if (ret_recvfrom == -1)
		{
			perror("recvfrom()");
			exit(-1);
		}
		if (ret_recvfrom > 0)
			printf("接收到了[%d]个字节的数据：[%s]\n", ret_recvfrom, buf);
	}

	exit(0);
}
~~~



[src\linux网络编程\UDP\UDP广播\client.c](src/linux网络编程/UDP/UDP广播/client.c)

 

 

### TCP本地套接字

>- 用于本机进程间通信
>
>- 一般使用TCP通信
>
>- 使用 套接字文件 替代端口和IP



**代码示例：**

**服务器**

~~~c
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include<sys/un.h>

/*
	本地套接字通信，回声服务器
*/

int main()
{
	//如果套接字文件存在删除
	unlink("./servers.s");
	
	//创建监听的套接字
	int lfd=  socket(PF_LOCAL,SOCK_STREAM,0);
	if (lfd == -1)
	{
		perror("socket()");
		exit(-1);
	}

	//监听套接字绑定套接字文件
	struct sockaddr_un s_addr_un;
	memset(&s_addr_un,0,sizeof(s_addr_un));
	s_addr_un.sun_family = AF_LOCAL;
	strcpy(s_addr_un.sun_path,"./servers.s");
	if (bind(lfd, (struct sockaddr*) & s_addr_un, sizeof(s_addr_un))==-1)
	{
		perror("bind()");
		exit(-1);
	}

	//开始监听
	if (listen(lfd, 128)==-1)
	{
		perror("listen()");
		exit(-1);
	}
	
	//处理连接
	struct sockaddr_un c_addr_un;
	int c_addr_un_siz = sizeof(c_addr_un);
	memset(&c_addr_un,0,c_addr_un_siz);
	int cfd;
goto_accept:
	if ((cfd = accept(lfd, (struct sockaddr*)&c_addr_un, &c_addr_un_siz)) == -1 )
	{
		perror("accept()");
		exit(-1);
	}

	printf("连接成功:对方套接字文件[%s]\n",c_addr_un.sun_path);
	
	//通信
	char buf[1024] = { 0 };
	while (1)
	{
		int ret = read(cfd, buf, sizeof(buf));
		//读取客服端发送的数据
		if (ret == -1)
		{
			perror("read()");
			exit(-1);
		}
		//客服端已关闭，且读缓冲区所有数据已经读完
		else if (ret == 0)
		{
			printf("写端已关闭\n");
			close(cfd);
			goto goto_accept;
		}

		//把读取到的数据，发回给客服端
		if (write(cfd, buf, sizeof(buf)) == -1)
		{
			perror("write()");
			exit(-1);
		}
	}


	exit(0);
}
~~~



**客服端**

~~~c
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include<sys/un.h>

/*
	本地套接字通信，客服端
*/

int main()
{
	//如果套接字文件存在删除
	unlink("./client.s");

	//创建通信的套接字
	int cfd = socket(AF_LOCAL, SOCK_STREAM, 0);
	if (cfd == -1)
	{
		perror("socket()");
		exit(-1);
	}

	//绑定
	//struct sockaddr_un c_addr_un;
	//int c_addr_un_size = sizeof(c_addr_un);
	//memset(&c_addr_un, 0, c_addr_un_size);
	//c_addr_un.sun_family = AF_LOCAL;
	//strcpy(c_addr_un.sun_path, "client.s");
	//if (bind(cfd, (struct sockaddr*) & c_addr_un, c_addr_un_size) == -1)
	//{
	//	perror("bind()");
	//	exit(-1);
	//}

	//连接
	struct sockaddr_un s_addr_un;
	int s_addr_un_size = sizeof(s_addr_un);
	memset(&s_addr_un, 0, s_addr_un_size);
	s_addr_un.sun_family = AF_LOCAL;
	strcpy(s_addr_un.sun_path, "servers.s");
	if (connect(cfd, (struct sockaddr*) & s_addr_un, s_addr_un_size)==-1)
	{
		perror("connect()");
		exit(-1);
	}
	
	while (1)
	{
		char buf[1024] = { 0 };
		//memset(buf,'a',1024);

		//读取键盘输入
		if ((read(1, buf, sizeof(buf))) == -1)
		{
			perror("read()");
			return -1;
		}

		//把键盘输入发送给服务器
		if (write(cfd, buf, sizeof(buf)) == -1)
		{
			perror("write()");
			return -1;
		}

		//读取服务器发送的数据
		memset(&buf, 0, sizeof(buf));
		if ((read(cfd, buf, sizeof(buf))) == -1)
		{
			perror("read2()");
			return -1;
		}

		//打印服务器发送的数据
		if (write(1, buf, sizeof(buf)) == -1)
		{
			perror("write()2");
			return -1;
		}

	}
	eixt(-1);
}
~~~

 

 





# C

## 阅读说明

>学C的时候没有记录笔记的习惯，后期部分整理而来
>



## c基础



### 指针

>- 指针可以获取3个值
>  - &p：自己的地址
>  - p：指向的地址
>  - *p：指向地址的值
>
>- 步长
>  - 根据指针类型的不同，步长也不同。步长相当于数据类型大小，比如char一个字节
>- 取值
>  - 内存中保存着二进制数据，根据指针类型不同，会解析出不同的值





### const(常量修饰符)

>- 放在全局区的const变量，无法通过指针修改
>- 一般函数参数使用const修饰的话表示这是一个传入参数，方便阅读





### sizeof(获取数据占用的空间)  

>- 返回的是unsingned int(无符号整型)
>- 用于指针时只能获取指针的大小，取*也只能获取指针类型的大小





### static(静态修饰符)

>- 运行前分配内存
>
>- 程序运行期间一直存活
>- 只有第一次初始化有效
>- 只能在当前文件使用
>
>



### typedef(取别名)

>**格式：**
>
>​	typedef  数据类型  需要的别名
>
>**优点：**
>
>- 简化结构体变量创建 可以省略struct关键字
>
>- 区分数据类型：char* p1,p2，此种情况p1是指针，p2不是，使用typedef char* Pchar 即可解决。
>
>-  提高可移植性，同一数据类型使用别名，需要改的时候，直接改一下别名就行



### #define宏

>- 预处理阶段展开
>
>- 宏函数
>  - #define MYADD(x,y) ((x)+(y))
>  - 多用括号防止运算优先级原因出错
>- 用的多的东西可以用宏，方便修改，维护



### 位运算

>- **~  按位取反**
>  -  一个数取反结果：0变1，1变0,
>
>- **& 按位与**
>  - 一个数与另外一个数，结果：同1为1，其他为0
>  - 将某几位置0
>
>- **| 按位或**
>  - 一个数或另外一个数，结果：同0为0，其他为1
>  - 将某几位置1
>
>- **^ 按位 异或**
>  -  一个数异或另外一个数，结果，相同为0，不为1
>  - 和1异或，将某几位翻转



###  左右移

>**左移**
>
>- 格式：数据 << N
>  - 数据：表示需要左移的数据
>  - <<：位移运算符
>  - N：表示左移几位
>
>- 注意
>  - 左移后，左侧头部数据被挤出，后面数据补0
>  - 位运算符，不会改变数据原有的值







### 库(WindowsVS)

#### 动态库

>- **动态库特点**
>  - 运行阶段加入程序，所以程序运行需要动态库文件
>  - 多个程序可以使用一个动态库，节省空间
>  - 运行阶段加入程序，所以库文件可以单独更新不需要重新编译程序
>
> 
>
>- **vs动态库的生成**
>
>  1. 编写库文件
>
>  2. **.h**文件，每个函数声明前面需要添加__declspec(dllexport)
>
>  3. 项目设置为动态库
>
>  4. 生成动态库文件
>
>  5. 找到.lib文件和.dll文件放到当前目录下使用，.h文件可以不使用
>
> 
>
>- **动态库的使用**
>
>  - 关联库文件 
>
>    ~~~c 
>    #pragma comment(lib,"./动态库.lib")
>    ~~~
>
>  - 如果要使用头文件的话，也包含一下头文件



#### 静态库

>- **静态库的特点**
>  - 编译阶段加入程序，所以程序可以单独使用，不需要库文件
>  - 编译阶段加入程序，所以每个程序使用单独静态库文件，浪费资源
>  - 编译阶段加入程序，所以更新库需要重新编译文件
>
>- **VS生成静态库**
>
>  1. 编写库文件
>  2. 项目设置为静态库
>  3. 生成静态库
>  4. 找到**.lib**文件使用，**.h**可以不用
>
>
>
>
>- **静态库的使用**
>
>  1. 关联库文件
>
>     ~~~c
>     #pragma comment(lib,"./静态库.lib")
>     ~~~
>
>  2. 如果要使用头文件的话，也包含以下头文件
>
>





  ### 编译步骤

- **预处理**

  ~~~ shell
  # 获取到头文件展开、宏替换、条件编译等之后的文件
  
  gcc -E hello.c -o hello.i
  ~~~



- **编译**

  ~~~shell
  # 获取到汇编代码，同时也会检查语法
  gcc -S hello.i -o hello.s
  ~~~



- **汇编**

  ~~~shell
  # 获取到机器码
  gcc -c hello.s -o hello.o
  ~~~



- **链接**

  ~~~shell
  # 链接为可执行文件
  gcc hello.o -o hello
  ~~~

  



### 内存对齐

>**内存对齐的大小，看结构体里面的“最大的数据类型”和“对齐模数”，谁小用谁**
>
>- #pragma pack(show) #pragma pack(1)，查看默认对齐模数、设置对齐模数 
>
>- 每一次对齐都是看“最大的数据类型”和“对齐模数”
>
>- 即使结构体结束，末尾的地方也要对齐
>
>- 数组，结构体，展开单个数据来看，不做整体看



## 内存、字符串操作函数

### 说明

>头文件#include <string.h>
>
>一般来说字符串操作函数遇到’\0’都会结束



### memset()设置

~~~c
void *memset(void *s, int c, size_t n);
参数：
	s：需要替换的起始地址
	c：需要替换的内容，直接给字符或者ascii都行
    n：替换多少个字节

返回值:
    指向s的指针 (基本不用不太清楚)

注意：
	完全使用内存操作系列函数组包时，如果写入txt可能导致乱码，用其他软件打开解决，如notepad++,vs,qt,写字板等等，原因不明，听说记事本前几个字符有默认东西
~~~





 

 

### memcmy()比较

~~~ c
int memcmp(const void *s1, const void *s2, size_t n);

参数：
    s1：比较数据1
	s2：比较数据2
    n：比较长度（字节）

返回值：
	相同返回：0
	不同返回：非0值

注意：
	不同情况下返回的非0值根据字符串大小决定，字符串大小根据ASCII表决定
~~~





 

### memcpy()拷贝

~~~c
void *memcpy(void *dest, const void *src, size_t n);

参数：
	dest：拷贝目标地址
	src：存有数据的地址
	n：拷贝几个字节

返回值：
    指向dest的指针

注意：
    src的dest所指的内存区域不能重叠 
~~~





 

### 分割字符串strtok()

~~~c
char *strtok(char *str, const char *delim);

参数：
	str：需要分割的字符串
    delim：分割符，可以是字符串，只要匹配到其中任何一个字符都会分割

返回值：
	还能继续分割返回：分割出来的字符串
	分割完成返回：NULL

注意：
	- 函数内部会自动保存剩下需要分割的字符串，所以第一次以后传NULL即可
	- 会改变参数str字符串，字符串中每个找到的分割符，都会被替换成’\0’

示例代码：

void lrStrtok()
{
    char p[] = "192.1,6,8.1.1";
    char pp[] = ".,";
    char *ppp;
    printf("%s\n", strtok(p, pp));
	while ((ppp = strtok(NULL, pp))!=NULL)
	{
        printf("%s\n", ppp);
	}

}
~~~

运行结果：

![image-20200826174824995](D:\zm\note\Readme.assets\image-20200826174824995.png)                               

 

 

### 查找指定字符串strstr()

~~~c
char *strstr(const char *haystack, const char *needle)
参数:
	haystack：被查找字符串(数据)
	needle：需要查找的字符（串）（务必使用字符串，不要使用字符，\0关系）

返回值：
	找到返回：needle第一次出现的位置
	没找到返回：NULL

注意：
	​ 查找指定字符第一次出现的位置：char *strchr(const char *s, int c);
	​ 查找指定字符最后一次出现的位置：char *strrchr(const char *s, int c);
~~~



### 拼接字符串strcat()

~~~c
char *strcat(char *dest, const char *src);
参数：
	dest：被追加字符串
	src：追加字符串

返回值：
    dest字符串的首地址

注意：
	​ dest需要有足够的空间来存储src的数据
	​ 可以指定src的长度
    	char *strncat(char *dest, const char *src, size_t n);
~~~





 

 

### 拷贝字符串strcpy()

~~~c 
char *strcpy(char *dest, const char *src);
参数：
	dest：目标字符串
    src：有数据的字符串

返回值：dest字符串起始地址

注意:
	​如果dest所指空间内存不够，会造成缓冲溢出(buffer Overflow)错误。
	​ 可以指定src的拷贝长度
        char *strncpy(char *dest, const char *src, size_t n);
~~~





 

 

### 字符串比较strcmp()

~~~c
int strcmp(const char *s1, const char *s2);
参数:
	s1：比较参数1
	s2：比较参数2

返回值：
	相同返回：0
	不同返回：非0值

注意：
	​ 不同情况下返回的非0值根据字符串大小决定，字符串大小根据ASCII表决定     
	​  可以指定比较前几个字节
        int strncmp(const char *s1, const char *s2, size_t n);
	​ 函数比较时可以忽略大小写
		int strcasecmp(const char *s1, const char *s2);
		int strncasecmp(const char *s1, const char *s2, size_t n);
~~~





## 文件操作函数

#### 说明

>C库的文件操作，带缓冲区



#### 打开文件

~~~c
#include <stdio.h>
FILE * fopen(const char * filename, const char * mode);
功能：打开文件

参数：
	filename：需要打开的文件名，根据需要加上路径
	mode：打开文件的模式设置
		“r”： 以只读方式打开一个文本文件（不创建文件，若文件不存在则报错）
    	.“w”：以写方式打开文件(如果文件存在则清空文件，文件不存在则创建一个文件)
    	“a”：以追加方式打开文件，在末尾添加内容，若文件不存在则创建文件
		“r+”：以可读、可写的方式打开文件(不创建新文件)
		“w+”：以可读、可写的方式打开文件(如果文件存在则清空文件，文件不存在则创建一个文件)
    	“a+”：以添加方式打开文件，打开文件并在末尾更改文件,若文件不存在则创建文件

返回值：
	成功：文件指针
	失败：NULL

注意：
	​ 第二个参数每种打开模式都可以加b表示二进制方式打开，只在windows下有效
	​ windows的磁盘文本文件使用\r\n（占用两个字节）的方式换行，不加b读写时都会自动转换
~~~





 

####  关闭文件

~~~c
#include <stdio.h>
int fclose(FILE * stream);
功能：关闭先前fopen()打开的文件。此动作让缓冲区的数据写入文件中，并且释放系统所提供的文件资源。 

参数：
	stream：文件指针

返回值：
	成功：0
	失败：-1 
~~~



#### fputc()单个字符写

~~~c
#include <stdio.h>

int fputc(int ch, FILE * stream);

功能：将ch转换为unsigned char后写入stream指定的文件中

参数：
	ch：需要写入文件的字符
	stream：文件指针

返回值：
	成功：成功写入文件的字符
	失败：返回-1
~~~



 

#### fgetc()单个字符读

~~~c
#include <stdio.h>

int fgetc(FILE * stream);

功能：从stream指定的文件中读取一个字符

参数：
	stream：文件指针

返回值：
	成功：返回读取到的字符
	失败：-1
~~~





#### fputs()写一个字符串

~~~c
#include <stdio.h>

int fputs(const char * str, FILE * stream);

功能：将str所指定的字符串写入到stream指定的文件中。

参数：
	str：需要写的数据
	stream：文件指针

返回值：
	成功：0
	失败：-1

注意：
	写字符串，遇\0结束，遇到\n不会结束
~~~





 

#### fgets()读一行字符串

~~~c
#include <stdio.h>

char * fgets(char * str, int size, FILE * stream);

功能：从stream指定的文件内读入字符，保存到str所指定的内存空间，直到出现换行字符、读到文件结尾或是已读了size - 1个字符为止，最后会自动加上字符 '\0' 作为字符串结束。

参数：
	str：读取到的数据存放的地方
	size：指定最大读取字符串的长度（size - 1）
	stream：文件指针

返回值：
	成功：成功读取的字符串
	读到文件尾或出错： NULL

 

注意：
	遇到'\n'结束

 
~~~





 

#### fwrite()块写

~~~c
#include <stdio.h>
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

功能：以数据块的方式给文件写入内容

参数：
	ptr：准备写入文件数据的地址
	size： size_t 为 unsigned int类型，此参数指定写入文件内容的块数据大小
	nmemb：写入文件的块数，写入文件数据总大小为：size * nmemb
	stream：已经打开的文件指针

返回值：
	成功：实际成功写入文件数据的块数目，此值和 nmemb 相等
	失败：0
~~~



 

#### fread()块读

~~~c
#include <stdio.h>

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);

功能：以数据块的方式从文件中读取内容

参数：
	ptr：存放读取出来数据的内存空间
	size： size_t 为 unsigned int类型，此参数指定读取文件内容的块数据大小
	nmemb：读取文件的块数，读取文件数据总大小为：size * nmemb
	stream：已经打开的文件指针

返回值：
	成功：实际成功读取到内容的块数，如果此值比nmemb小，但大于0，说明读到文件的结尾。
    失败：0
~~~



 

 

### fprintf()格式化写

~~~c
#include <stdio.h>

int fprintf(FILE * stream, const char * format, ...);

功能：根据参数format字符串来转换并格式化数据，然后将结果输出到stream指定的文件中，指定出现字符串结束符 '\0' 为止。

参数：
	stream：已经打开的文件
	format：字符串格式，用法和printf()一样

返回值：
	成功：实际写入文件的字符个数
	失败：-1

注意：
	示例 fprintf(fp, "%d %d %d\n", 1, 2, 3);
~~~





 

 

### fscanf()格式化读

~~~c
#include <stdio.h>

int fscanf(FILE * stream, const char * format, ...);

功能：从stream指定的文件读取字符串，并根据参数format字符串来转换并格式化数据。

参数：

	stream：已经打开的文
	?format：字符串格式，用法和scanf()一样

返回值：
	成功：参数数目，成功转换的值的个数
	失败： - 1

注意：
//示例
int a = 0;
int b = 0;
int c = 0;
fscanf(fp, "%d %d %d\n", &a, &b, &c);
printf("a = %d, b = %d, c = %d\n", a, b, c);

~~~





 

### feof()是否读到文件尾

~~~c
#include <stdio.h>
int feof(FILE * stream);

功能：检测是否读取到了文件结尾。判断的是最后一次“读操作的内容”，不是当前位置内容(上一个内容)。

参数：
	stream：文件指针

返回值：
	非0值：已经到文件结尾
	0：没有到文件结尾
~~~





 

 

### stat()获取文件状态

~~~c
#include <sys/types.h>
#include <sys/stat.h>
int stat(const char *path, struct stat *buf);

功能：获取文件状态信息

参数：
	path：文件名
	buf：传入参数，保存文件信息的结构体

返回值：
	成功：0
	失败-1

注意：
    
//使用到的结构体
struct stat {

​    dev_t     st_dev;     //文件的设备编号

​    ino_t     st_ino;     //节点

​    mode_t    st_mode;  //文件的类型和存取的权限

​    nlink_t    st_nlink;   //连到该文件的硬连接数目，刚建立的文件值为1

​    uid_t     st_uid;     //用户ID

​    gid_t     st_gid;     //组ID

​    dev_t     st_rdev;   //(设备类型)若此文件为设备文件，则为其设备编号

​    off_t     st_size;    //文件字节数(文件大小)

​    unsigned long st_blksize;  //块大小(文件系统的I/O 缓冲区大小)

​    unsigned long st_blocks;  //块数

​    time_t    st_atime;   //最后一次访问时间

​    time_t    st_mtime;  //最后一次修改时间

​    time_t    st_ctime;   //最后一次改变时间(指属性)

};
~~~





 

 

 

### 删除、重命名文件

~~~c
#include <stdio.h>
int remove(const char *pathname);

功能：删除文件

参数：
	pathname：文件名

返回值：
	成功：0
	失败：-1

 

#include <stdio.h>
int rename(const char *oldpath, const char *newpath);

功能：把oldpath的文件名改为newpath

参数：
	oldpath：旧文件名
	newpath：新文件名

返回值：
	成功：0
	失败： - 1
~~~





 

 

### fflush()强制刷新缓冲区

~~~c
#include <stdio.h>
int fflush(FILE *stream);

功能：更新缓冲区，让缓冲区的数据立马写到文件中。

参数：
	stream：文件指针

返回值：
	成功：0
	失败：-1
~~~





 

 

### fopen()缓冲区的刷新机制

>- 满刷新
>
>- 关闭刷新
>
>- 强制刷新
>
>注意：
>
> 没有行刷新
>
>程序崩溃，没有正常结束不会触发关闭刷新





#  CPP



## 基础



### namespace命名空间

#### 说明

- 用于解决名称冲突
- 命名空间必须声明在全局作用域下



#### 示例

~~~c++
//命名空间可以存放变量、函数、结构体、类
namespace A
{
	int m_A = 0;
	void func() {};
	struct Person {};
	class Animal {};
}

//命名空间可以嵌套命名空间
namespace B
{
	int m_A = 100;
	namespace C
	{
		int m_A = 200;
	}
}

//名空间是开放的，可以随时向命名空间下添加成员
namespace B
{
	int m_A = 100;
}
namespace B
{
	int m_B = 1000;
}

//名空间可以是匿名的
namespace
{	
	int m_C = 10000; // 如果命名空间 是 匿名  变量前隐式加了关键字  static
	int m_D = 20000;
}

//名空间可以起别名
namespace C
{
	int m_E = 100000;
}
namespace D = C;
void test()
{
	namespace E =C;
	cout << E::m_E << endl;
	cout << D::m_E << endl;
	cout << C::m_E << endl;
}
~~~





### const

>- 全局区域const变量无法修改
>
>- 局部区域const变量如果使用常量赋值，也无法修改，但是：
>  - 编译能成功
>  - 运行也能成功
>  - 结果不会改变，因为当前变量指向的是符号表
>  - 修改的只是一个临时空间， 
>
>- c语言的const默认是外部链接属性，c++的默认是内部链接属性
>
>- c++尽量使用const代替#define





### using

>- using声明
>  - using namespace std;
>
>- 提前告诉编译器 sunWuKongId用 KingGlory下的
>  - using KingGlory::sunWuKongId;



### 引用

>- 语法：
>   - 数据类型 & 引用名 = 原名
>  - 引用必须初始化
>  - 引用本质上是一个指向不可修改的指针，不同的引用操作，编译器会做不同的“优化”

~~~c++
//对数组引用

//1、直接建立引用
//pArr就是arr的引用名
int arr[5] = { 1, 2, 3, 4, 5 };
int(&pArr)[5] = arr;	

//2、先定义数组类型，再通过类型定义数组的引用
typedef int(ARRAY_TYPE)[5];
// 数据类型  & 别名 = 原名
ARRAY_TYPE & pArr2 = arr;
~~~





### 内联函数

>- 函数的声明和实现都加关键字inline
>
>- 用于替换宏函数
>
>- 类的成员函数都隐藏的加了关键字inline
>
>- inline只是建议编译器把该函数当做内联函数处理，具体是否是内联函数不一定
>
>- 不加inline关键字，编译器也可能把一些函数当做内联函数
>- 示例：
>  - ![image-20200827190117736](D:\zm\note\Readme.assets\image-20200827190117736.png)





### 函数的默认参数

~~~c++
//如果某个位置已经有了默认参数，那么从这个位置起，后面的参数都必须有默认参数
int func(int a,int b = 20,int c = 20)
{
    return a+b+c;
}

//函数的声明和实现只能有一个默认参数
int func(int a = 10,int b = 20);
int func(int a,int b)
{
    return a+b;
}

//占位参数
int func(int a,int = 10)
{
}
int func(int a,int)
{
}
~~~



### 函数重载

>- 在同一作用域下，函数的名称相同，函数的**参数个数、类型、顺序不同**，代表了不同的函数
>
>- 实现原理是，编译器会改变函数真正的名称，所以这些函数的名称也是不同的



### exterm C

~~~c++
//单个使用
extern "C" void func()
{
    
}

//批量使用、头文件使用，通过宏 __cplusplus 在编译时自动判断当前编译器是C还是C++
#ifdef __cplusplus
extern "C" {
#endif

#pragma once
#include<stdio.h>
void show();
void show1();
void show2();
    
#ifdef __cplusplus
}
#endif
~~~



### 异常处理

>- 关键字
>  - **throw** 抛出异常
>  - **try** 捕获异常 相当于if
>  - **catch** 捕获异常 相当于else



**示例：**

~~~c++
#include<iostream>
using namespace std;

//异常处理基类
class ERR
{
public:
	virtual void show()
	{
		cout << "ERR异常处理" << endl;
	}
};

//异常处理派生类
class NullERR : public ERR
{
public:
	virtual void show()
	{
		cout << "NullERR异常处理" << endl;
	}
};
//异常处理派生类
class HHERR : public ERR
{
public:
	virtual void show()
	{
		cout << "呵呵异常处理" << endl;
	}
};



//只允许函数抛出指定异常
//int myDivision(int a, int b) throw(string, int, ERR);

//不允许函数抛出异常
//int myDivision(int a, int b) throw();


//会引发异常的函数
int myDivision(int a ,int b)
{
	if (b==0)
	{
	#ifdef false
		throw 1;//抛出int类型异常
		throw ERR();//抛出自定义类型异常
		throw 1.1;//抛出double类型异常
		throw 'a';//抛出char类型异常
		throw "asd";//抛出const char[] (const char*)异常 
		throw HHERR();//抛出自定义类型异常
		throw string("asd");//抛出string类型异常
	#endif 
		//抛出异常，如果没有捕获处理程序会中断
		//抛出异常相当于return，会结束该函数
		throw NullERR();//抛出自定义类型异常
	}

	return a / b;
}


void test()
{
	try	//捕获异常
	{
		myDivision(10, 0);
	}
	catch (const char *) //捕获const char *类型异常
	{
		cout << "char *异常" << endl;
	}
	catch (ERR &e) //捕获ERR类型异常，这种语法可以接收到抛出的匿名对象
				  //该语法还可以接收到到子类的匿名对象
	{
		e.show();
	}
	catch (...)	//捕获其他的异常
	{
		throw;//暂时不处理，继续抛出当前异常，异常类型不变
		//cout << "其他异常" << endl;
	}
	
}
void test01()
{
	//捕获test()函数抛出的异常
	try
	{
		test();
	}
	catch(string)
	{
		cout<<"string异常捕获"<<endl;
	}
	catch (...)
	{
		cout << "其他异常捕获" << endl;
	}
}

int main()
{
	test01();
	system("pause");
	return 0;
}
~~~



**多态继承的示例：**

~~~c++
#include<iostream>
#include<string>
using namespace std;

//继承c++错误父类
class MyErr :public exception
{
public:
	MyErr()
	{
		err = "没有错误信息！";
	}

	//构造创建对象
	MyErr(const char * err)
	{
		this->err = err;
	}
	//构造创建对象
	MyErr(string err)
	{
		this->err = err;
	}
	//重写父类虚拟函数打印返回错误信息
	virtual const char * what() const
	{
		return this->err.c_str();
	}

	virtual ~MyErr()
	{
	}

private:
	string err;

};

//抛出错误的函数
void myErr()
{
	//抛出错误对象
	throw MyErr();
}

int main()
{
	try
	{
		myErr();

	}
	catch (exception & e)//通过多态，父类接收子类错误对象
	{
		cout << e.what() << endl;
	}
	catch (...)
	{
		cout << "其他错误信息" << endl;
	}

	system("pause");
	return 0;
}
~~~



### 标准输入cin

#### 从标准输入中读取一个字符

~~~c++
char ch = cin.get();
~~~



#### 从标准输入中读取一个字符串

~~~c++
cin.get( buf , sizeof(buf) );//不会读取换行符，换行符遗留在缓冲区中

cin.getline( buf , sizeof(buf) )//不会读取换行符，换行符扔掉，不会留在缓冲区
~~~



#### 忽略字符

~~~c++
cin.ignore();//忽略一个字符，括号中可以有参数，填几就忽略几个字符
~~~



#### 偷窥一个字符

~~~c++
cin.peek();//偷窥就是查看缓冲区中的下一个字符，但是不会取走
~~~



#### 放回字符

~~~c++
char ch =cin.get();

cin.putback(ch);//放回字符，原来样放回
~~~



#### 代码示例

~~~c++
#include<iostream>
using namespace std;

//读取一个字符cin.get()
void test()
{
	char ch = cin.get();
	cout << ch << endl;
}

//读取一个字符串cin.get(buf,sizeof(buf))
void test01()
{
	char buf[1024];
	//这种方式读取遇到换行符结束，同时换行符会留在缓冲区
	cin.get(buf,sizeof(buf));
	cout << buf << endl;
	cin.get();
}

//读取一个字符串cin.getline()
void test02()
{
	char buf[1024] = { 0 };
	//这种方式读取遇到换行符结束，换行符会被抛弃，不在缓冲区，也不在字符串
	cin.getline(buf, sizeof(buf));
	cout << buf << endl;
}

//忽略一个或多个字符cin.ignore()
void test03()
{
	char buf[1024] = { 0 };
	//忽略字符，小括号填多少就忽略多少个字符
	cin.ignore();
	cin.get(buf, sizeof(buf));
	cout << buf << endl;
}

//偷窥一个字符 cin.peek()
void test04()
{
	char buf[1024] = {0};
	//窥探
	char ch = cin.peek();
	cout << ch << endl;
	cin.get(buf, sizeof(buf));
	cout << buf << endl;

}

//放回一个字符，只能放回一个 cin.putback()
void test05()
{
	//读取出来后在放回去
	char ch= cin.get();
	cout << ch << endl;
	cin.putback(ch);

	char buf[1024] = { 0 };
	cin.get(buf, sizeof(buf));
	cout << buf << endl;
}

int main()
{
	//test();
	//test01();
	//test02();
	//test03();
	test05();
	system("pause");
	return 0;
}


//案例2  让用户输入 1 ~ 10之间的数字，如果输入有误，重新输入
void test08()
{
	int num;

	while (true)
	{
		cout << "请输入1~10之间的数字： " << endl;
		//cin>>num;读取折标会设置标志位为1  然后向下走
		cin >> num;
		if (num > 0 && num < 10)
		{
			cout << "输入正确-值为：" << num << endl;
			break; 
		}

		//重置标志位
		cin.clear();
		//清空缓冲区   2013可用 2015以上 cin.ignore()/ cin.getline();
		cin.sync();

		//缓冲区中的标志位  0正常   1异常
		cout << "标志位： " << cin.fail() << endl;

		//cout << "输入有误请重新输入" << endl;
	}
}
~~~





### 标准输出 cout

**代码示例：**

~~~c++
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include <iomanip> // io  man  ip

/*
cout.put() //向缓冲区写字符
cout.write() //从buffer中写num个字节到当前输出流中。
*/

void test01()
{
	//cout.put('h').put('e').put('l');

	//char buf[] = "hello world";
	//cout.write(buf, sizeof(buf));

	cout << "hello world" << endl;
}


//通过流成员函数
void test02(){

	int number = 99;
	cout.width(10); //设置宽度
	cout.fill('*'); //填充
	cout.setf(ios::left);  //设置左对齐格式
	cout.unsetf(ios::dec); //卸载十进制
	cout.setf(ios::hex);  //设置十六进制   63
	cout.setf(ios::showbase); //显示基数 0x (十六进制)  0 （八进制）
	cout.unsetf(ios::hex); //卸载十六进制
	cout.setf(ios::oct);  //设置八进制  
	cout << number << endl;
}


void test03()
{
	//使用控制符
	int number = 99;
	cout << setw(20)   //设置宽度
		 << setfill('~') //填充
		 << setiosflags(ios::showbase) //显示基数
		<< setiosflags(ios::left)  //左对齐
		<< hex  //设置十六进制
		<< number
		<< endl;
}


int main(){

	//test01();

	//test02();

	test03();

	system("pause");
	return EXIT_SUCCESS;
}
~~~





### 文件IO

**代码示例：**

~~~c++
#include<iostream>
using namespace std;
#include<fstream>
#include<string>

//C++文件打开方式
//ios::in	以输入方式打开文件
//ios::out	以输出方式打开文件（这是默认方式），如果已有此名字的文件，则将其原有内容全部清除
//ios::app	以输出方式打开文件，写入的数据添加在文件末尾
//ios::ate	打开一个已有的文件，文件指针指向文件末尾
//ios::trunc	打开一个文件，如果文件已存在，则删除其中全部数据，如文件不存在，则建立新文件。如已指定了 ios::out 方式，而未指定ios : : app，ios::ate，ios : : in，则同时默认此方式
//ios::binary	以二进制方式打开一个文件，如不指定此方式则默认为ASCII方式
//ios::nocreate	打开一个已有的文件，如文件不存在，则打开失败。nocrcate的意思是不建立新文件
//ios::noreplace	如果文件不存在则建立新文件，如果文件已存在则操作失败，replace 的意思是不更新原有文件
//ios::in | ios::out	以输入和输出方式打开文件，文件可读可写
//ios::out | ios::binary	以二进制方式打开一个输出文件
//ios::in | ios::binar	以二进制方式打开一个输入文件

//写文件
void test01()
{
	ofstream ofs("./test.txt", ios::in | ios::trunc);
	if (!ofs.is_open())
	{
		cout << "文件打开失败" << endl;
		return;
	}

	//写入内容
	ofs << "姓名:diaomao\n";
	ofs << "年龄:12" << endl;
	ofs << "性别:男" << endl;

	//关闭对象
	ofs.close();
}

//读文件
void test02()
{
	ifstream ifs;
	ifs.open("./test.txt",ios::in);
	if (!ifs.is_open())
	{
		cout << "文件打开失败" << endl;
		return;
	}

	char buf[1024] = {0};
	//读取文件内容,第一种方式
	//while (ifs>>buf)
	//{
	//	cout << buf << endl;
	//}

	//第二种方式
	//while (ifs.getline(buf,sizeof(buf)))
	//{
	//	cout << buf << endl;
	//}

	//第三种方式
	//string str;
	//while (getline(ifs,str))
	//{
	//	cout << str << endl;
	//}

	//第四种方式,单个字符读取
	char c = 0;
	while ( (c=ifs.get()) != EOF)
	{
		cout << c;
	}

	ifs.close();
}


int main()
{
	test01();
	test02();
	system("pause");
	return 0;
}
~~~



## **面向对象**

### 封装

 

 

### 继承

#### 继承的权限 

> pulic，protected，private继承后权限不变，如果权限不够自动降级,注意父类私有属性子类无法访问
>
> - 公共继承
>   - 父类中公共权限成员，到子类中变为公共权限
>   - 父类中保护权限成员，到子类中变为保护权限
>   - 父类中私有权限成员，到子类中不可访问
>
> - 保护继承
>   - 父类中公共权限成员，到子类中变为保护权限
>   - 父类中保护权限成员，到子类中变为保护权限
>   - 父类中私有权限成员，到子类中不可访问
>
> - 私有继承
>   - 父类中公共权限成员，到子类中变为私有权限
>   - 父类中保护权限成员，到子类中变为私有权限
>   - 父类中私有权限成员，到子类中不可访问





 

#### 查看对象模型

>a.   找到vs开发人员命令提示符
>
>b.  切换到文件所在目录，输入下面命令
>
>c.   cl /d1  reportSingleClassLayout类名 文件名



 

#### 不会被继承的函数

>默认构造  拷贝构造  析构  operator=



 

#### 函数重定义

>子类中只要有和父类相同名称的函数，父类中的所有该名称函数将被隐藏，如需访问，可以同过加作用域访问



 

#### 创建子类对象时需要创建父类对象

>a.   一般来说父类对象必须要有默认构造
>
>b.  如果父类对象没有默认构造，子类对象的构造，需要初始化列表法给父类构造传参



 

 

### 多态

> 多态就是父类 指针 或 引用 调用子类对象

 

#### 动态多态的满足条件

> 继承关系，父类中写了虚函数，子类重写了父类中的虚函数

 

#### 多态的原理

>vfptr虚函数指针， vftable虚函数表，有兴趣在深入了解

 

#### 虚函数

>多态使用： 父类的指针或者引用 指向子类的对象



 

#### 纯虚函数

>a.   关键字 virtual
>
>b.  纯虚函数可以不写实现，virtual void fun()=0;
>
>c.   类中如果有纯虚函数，这个类就是抽象类，不可以创建对象，
>
>d.  子类继承抽象类，如果想要创建对象就必须重写父类中的纯虚函数



 

#### 虚析构和纯虚析构

>- 纯虚析构需要在类外自己写实现
>- 存在的意义，释放父类时可以自己找到子类的析构调用，避免内存泄漏
>- 可以自己释放真实对象，不用虚析构
>- 使用虚析构后delete运作模式会改变，别乱delete会出错



#### 函数重写

>- 子类重写父类中的虚函数就是重写
>
>- 重写和重载一样需要返回值、函数名、形参列表一致，才是重写



### 构造函数

>- 创建对象需要调用构造方法
>
>
>
>- 构造函数的语法
>
>![image-20200827193314587](D:\zm\note\Readme.assets\image-20200827193314587.png)
>
>- 拷贝构造，初始化对象时调用，默认是值拷贝，所以如果有堆区数据就是拷贝地址（浅拷贝）
>
>
>
>- 默认构造的提供规则
>  - 默认会有无参构造和拷贝构造
>  - 只要提供了任何构造方法，就不会再提供无参构造
>  - 只要不自己提供拷贝构造，都会有默认的拷贝构造
>
>
>
>- 构造的调用
>
>![image-20200827193435189](D:\zm\note\Readme.assets\image-20200827193435189.png)
>
>
>
>- 初始化列表，其中参数有对象也可以使用
>
>![image-20200827193507981](D:\zm\note\Readme.assets\image-20200827193507981.png)



### new

>- 创建存放在堆区的对象，用完需要delete释放；
>
>- 如果利用new在堆区开辟 自定义类型数据的数组，必须有默认构造函数
>  - Person * personArray = new Person[10];
>  - 释放堆区数组 delete [] personArray;



### struct和class

>- 在c++下struct和class是一样的，只有默认权限不同
>
>- struct默认权限是public   class是private



### 类的静态成员

>- 独立于对象存在，所以只有一份所有对象共用，也可以不使用对象单独使用
>
>- 独立于对象所以，静态属性函数外部赋值
>
>- 独立于对象所以，静态方法无法使用非对象静态成员



### 单例模式

>- 只能创建一个对象就叫单例模式
>
>- 私有化所有构造
>
>- 创建一个私有的静态成员存储唯一对象的指针，该成员在类的外部赋值
>
>- 可以有一个公共方法返回这个唯一对象指针
>
>- 分文件使用时类外创建对象要放在CPP里面

![image-20200827193801570](D:\zm\note\Readme.assets\image-20200827193801570.png)

​                               

### 类的常函数和常对象

- 常函数表示this的值无法修改，(声明该函数不会改变成员属性的值)

![image-20200827193841383](D:\zm\note\Readme.assets\image-20200827193841383.png)



- 常对象成员属性(this)的值无法修改，常对象只能调用常函数，这是为了防止常对象的成员属性被修改

![image-20200827193914083](D:\zm\note\Readme.assets\image-20200827193914083.png)

- 可以有特殊成员，用 mutable 关键字声明，声明后该值可以在常函数中修改

![image-20200827193945084](D:\zm\note\Readme.assets\image-20200827193945084.png)







### 友元

- 友元就是允许对方访问该类的私有成员

  - 全局函数作为友元

    ![image-20200827194032643](D:\zm\note\Readme.assets\image-20200827194032643.png)

  - 成员函数为友元

    ![image-20200827194105874](D:\zm\note\Readme.assets\image-20200827194105874.png)

  - 类作为友元

    ​										![image-20200827194134871](D:\zm\note\Readme.assets\image-20200827194134871.png)



### 运算符重载

- 主要是让对象能使用运算符，方便操作，减少代码量

- operator关键字



#### **代码示例**

~~~C++
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include<string>

class AA
{
public:
	//加号运算符重载
	AA operator+(AA &aa)
	{
		AA temp; 
		temp.a =  this->a + aa.a;
		strcat(temp.b,this->b);
		strcat(temp.b,aa.b);
		temp.c = this->c + aa.c;

		return temp;
	}

	//减号运算符重载
	AA operator-(AA &aa)
	{
		AA temp;
		temp.a = this->a = aa.a;
		return temp;
	}

	//比较运算符重载
	bool operator==(AA &aa)
	{
		if (this->a==aa.a && this->c==aa.c && (strcmp(this->b,aa.b)==0))
		{
			return true;
		}

		return false;
	}

	//前置++重载
	AA& operator++()
	{
		this->a++;
		return *this;
	}

	//后置++重载
	//返回值加const 是为了防止连续++ 后置++ 无法连续++
	//但是这样返回值就是一个常对象，常对象只能调用常函数
	const AA operator++(int)
	{
		AA temp(*this);
		//temp.a = this->a;
		this->a++;
		return temp;
	}

	//赋值运算符重载
	AA& operator=(const AA & aa)
	{
		cout << "operator=()调用" << endl;
		this->a = aa.a;
		strcpy(this->b,aa.b);
		this->c = aa.c;
		return *this;
	}

	//void abc(AA &a,AA &b)
	//{
	//	AA temp;
	//	temp.a = a.a + b.a;
	//	
	//}

	AA()
	{
		cout << "无参构造调用" << endl;
		this->a = 0;
		this->b = (char*)malloc(128);
		memset(this->b,0x0,128);
	}

	AA(int a,const char *b, const string c)
	{
		cout << "有参构造调用" << endl;
		this->b = (char*)malloc(128);
		memset(this->b, 0x0, 128);
		this->a = a;
		strcpy(this->b,b);
		this->c = c;
	}

	AA(AA &aa)
	{
		cout << "拷贝参构造调用" << endl;
		this->b = (char*)malloc(128);
		memset(this->b, 0x0, 128);
		this->a = aa.a;	
		strcpy(this->b,aa.b);
		this->c = aa.c;
	}

	~AA()
	{
		cout << "析构调用" << endl;
		this->a = 0;
		free(this->b);
		c = "\0";
	}
	
	 int getA()const
	{
		return this->a;
	}

	char* getB()
	{
		return this->b;
	}

	string getC()
	{
		return this->c;
	}
private:
	int a;
	char *b;
	string c;
};



int main()
{
	
	AA a1(12,"char","string");
	(a1++);
	cout << a1.getA() << endl;
	cout << (a1++).getA() << endl;
	cout << (++a1).getA() << endl;
	++(++(++a1));
	cout << a1.getA() << endl;
	//a1.getA();
	AA a2 = a1;//拷贝构造的调用
	AA a3 (a2);
	AA a4(13,"char1","char2");
	a3 = a2 + a1 + a1; 
	a1 = a2 = a3 = a4;
	cout << a3.getA() <<"  "<<"char="<< a3.getB() << "  "<<"string=="<<a3.getC() << endl;

	if (a1==a2)
	{
		cout << "a1==a2" << endl;
		
	}
	else
	{
		cout << "a1!=a2" << endl;
	}
	system("pause");
	return 0;
}
~~~



#### **MyString示例**

~~~c++
#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include<iostream>
#include<string>
using namespace std;


class MyString
{
	//friend istream& operator<<(istream & cin, MyString & str);
	friend ostream& operator<<(ostream & cout, MyString & str);
public:
	MyString();
	MyString(const char* str);
	MyString(MyString & str);
	~MyString();

	//获取字符串大小
	unsigned int getSize();

	//重载=
	MyString& operator=(const MyString & str);
	MyString& operator=(const char* str);

	//重载+
	//这里不能返回引用  
	//值返回会编译器会隐藏的调用拷贝函数，拷贝一份返回
	//返回引用的话，已经调用析构了，会访问到非法内存
	MyString operator+( MyString & str);
	MyString operator+(const char * str);


	//重载==
	bool operator==(const char *str);
	bool operator==(MyString & str);
	
	//重载[]
	char& operator[](int i);



private:
	char * pString;
 	unsigned int size;
};

//重载<<运算符
ostream& operator<<(ostream & cout,MyString & str);


~~~

~~~c++
#include "MyString.h"

MyString::MyString()
{	
	this->pString = (char*)malloc(8);
	memset(this->pString,0x0,8);
	this->size = 8;
}

MyString::MyString(const char * buf)
{
	if (this->pString!=NULL )
	{
		free(this->pString);
		this->pString = NULL;
		this->size = 0;
	}

	this->pString = (char*)malloc(strlen(buf)+1);
	memset(pString,0x0,strlen(buf)+1);
	strcpy(this->pString,buf);
	this->size = (strlen(this->pString)+1);

}

MyString::MyString(MyString & str)
{
	this->pString = (char*)malloc(str.size);
	memcpy(this->pString,str.pString,str.size);
	this->size = str.size;
}


MyString::~MyString()
{
	if (this->pString!=NULL)
	{
		free(this->pString);
		this->size = 0;
	}

}







MyString & MyString::operator=(const MyString & str)
{
	if (this->size < str.size)
	{
		free(this->pString);
		this->pString = (char*)malloc(str.size);
	}
	memset(this->pString,0x0,this->size);
	memcpy(this->pString, str.pString, str.size);
	this->size = str.size;
	return *this;

}

MyString & MyString::operator=(const char * str)
{
	if (this->size < strlen(str)+1)
	{
		free(this->pString);
		this->pString = (char*)malloc(strlen(str) + 1);
	}
	memset(this->pString, 0x0, this->size);
	strcpy(this->pString, str);
	this->size = strlen(str) + 1;
	return *this;
}

MyString MyString::operator+(MyString & str)
{
	char * temp = (char*)malloc(str.size+this->size+1);
	memset(temp, 0x0, str.size + this->size + 1);
	strcpy(temp,this->pString);
	strcat(temp,str.pString);
	MyString ms(temp);
	free(temp);
	return ms;	
}

MyString MyString::operator+(const char * str)
{
	char * temp = (char*)malloc((strlen(str) + 1) + this->size );
	memset(temp,0x0, (strlen(str) + 1) + this->size );
	strcpy(temp, this->pString);
	strcat(temp, str);
	MyString ms(temp);
	free(temp);
	return ms;
}

bool MyString::operator==(const char * str)
{
	if (strcmp(this->pString,str)==0)
	{
		return true;
	}
	return false;
}

bool MyString::operator==(MyString & str)
{
	if (strcmp(this->pString, str.pString) == 0)
	{
		return true;
	}
	return false;
}

char & MyString::operator[](int i)
{
	if (i < this->size)
	{
		return this->pString[i];
	}
}

unsigned int MyString::getSize()
{
	return this->size;
}

ostream & operator<<(ostream & cout, MyString & str)
{
	cout << str.pString;
	return cout;
	
}

~~~





## 模板

>模板也叫泛形编程 关键字  template<>



### 模板的应用

~~~c++
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include"Person.hpp"
using namespace std;

//模板函数的使用
template<class T,class T1>
void jh(T& a,T1& b)
{
	T temp;
	temp = a;
	a = b;
	b = temp;

}
//模板函数的使用
class JH
{
public:
	JH()
	{}
	JH(string a,int b)
	{
		this->age = b;
		this->name = a;
	}

	JH& operator=(JH& j)
	{
		this->age = j.age;
		this->name = j.name;
		return *this;
	}

	int age;
	string name;
};
//模板函数的使用
void test()
{
	int a = 0;
	int b = 5;

	JH j1("diaomao",1);
	JH j2("maodiao",2);
	//泛形函数的使用
	jh<JH, JH>(j1, j2);
	cout << "j1.age=" << j1.age <<"  j1.name=" << j1.name<< endl;
	cout << "j2.age=" << j2.age <<"  j2.name=" << j2.name<< endl;


	//泛形函数的使用
	jh<int,int>(a,b);
	//如果是简单的基础数据，模板函数会自动推导，可以不指定数据类型
	jh(a,b);
	cout << "a=" << a << endl;
	cout << "b=" << b << endl;
}


//以下为模板类的使用
template<class HEIGHT, class NAME, class AGE>
class Person_1 : public Person<NAME ,AGE>
{
public:
	Person_1()
	{}
	//初始化列表方法，在Person<NAME, AGE>类没有无参构造的情况下创建对象
	//1.Person<NAME, AGE>(ps1.name, ps1.age) 这个初始化的是继承到的Person对象
	//2.hi(hi1)这个初始化的是成员属性 hi
	//3.ps(ps1.name,ps1.age) 这个初始化的是成员ps对象
	Person_1(Person<NAME, AGE> ps1, HEIGHT hi1) :
		Person<NAME, AGE>(ps1.name, ps1.age), hi(hi1), ps(ps1.name,ps1.age)
	{
	}
	//成员函数使用模板
	void show()
	{
		cout << "姓名：" << this->ps.name << endl << "年龄" << this->ps.age << endl
			<< "身高：" << this->hi << endl;
		cout << endl << endl;
		cout << "姓名：" << this->Person<NAME, AGE>::name << endl << "年龄" << this->Person<NAME,AGE>::age << endl
			<< "身高：" << this->hi << endl;
	}

	//成员函数使用模板
	void show(Person_1<HEIGHT, NAME, AGE> & p);

	Person<NAME, AGE> ps;
	HEIGHT hi;
};

//类外实现成员模板函数的实现
template<class HEIGHT, class NAME, class AGE>
void Person_1<HEIGHT, NAME, AGE>::show(Person_1<HEIGHT, NAME, AGE> & p)
{
	cout << p.age << p.hi << p.name << p.ps.age << p.ps.name << endl;
}


//void show;

void test01()
{
	//注意这行代码需要走4个构造，否则无法创建p3对象
	//1.走的构造是，传入的默认对象 Person<string,int>("das",12)
	//2.走Person_1继承来的Person类构造
	//3.走的是Person_1的成员属性Person的构造
	//4.走的是Person_1的构造
	Person_1<int,string, int> p3(Person<string,int>("das",12), 15);
	p3.show();
	p3.show(p3);

}

int main()
{
	test();
	//test01();

	system("pause");
	return 0;
}
~~~





### 自定义数组案例

~~~c++
#include<iostream>
using namespace std;
#include<string>
template<class LR>
class ARR
{
public:
	ARR()
	{
		this->data = NULL;
		this->len = 0;
		this->size = 0;
	}

	//传入参数为数组的大小
	ARR(int a)
	{
		this->len = 0;
		this->size = a;
		this->data = new LR[a];
	}

	//拷贝构造
	ARR(ARR & arr )
	{
		this->len = arr->len;
		this->size = arr->size;
		//delete this[];
		this->data = new LR[arr->size];
		for (int i = 0; i < arr->size; i++)
		{
			this->data[i] = arr->data[i];
		}
	}

	//重载等号
	ARR& operator=(ARR & arr )
	{
		if (this->data!=NULL)
		{
			delete[] this->data;
		}

		this->size = arr.size;
		this->len = arr.len;
		this->data = new ARR[arr.size];
		for (int i = 0; i < arr.len; i++)
		{
			this->data[i] = arr.data[i];
		}

		return *this;
	}

	//重载中括号
	LR& operator[](int pos)
	{
		return this->data[pos];
	}

	//尾插
	void pushBack(const LR &lr)
	{
		if (this->len>=(this->size-1))
		{
			return;
		}

		this->data[len] = lr;
		len++;
	}

	//获取数组大小
	int getSize()
	{
		return this->size;
	}


	//获取数组当前有效长度
	int getLen()
	{
		return this->len;
	}


	void weiShan()
	{
	//	delete this->data[len];
		this->len--;
	}

	//析构
	~ARR()
	{
		delete[] this->data;
	}

private:
	//数据
	LR* data;
	//当前长度
	int len;
	//数组大小
	int size;
};

~~~



~~~c++
#include"ARR.hpp"

int a = 0;
int b=0;

//用于测试的Person类
class Person
{
public:
	Person()
	{
		cout<< "构造"<<++a<<endl;
	}
	Person(int age,string name)
	{
		this->age = age;
		this->name = name;
	}

	~Person()
	{
		cout << "析构"<<++b<<endl;
	}
	int age;
	string name;


};

void test()
{
	Person p1(1, "a");
	Person p2(2, "b");
	Person p3(3, "c");
	Person p4(4, "d");
	Person p5(5, "e");
	//创建ARR数组对象
	ARR<Person>a1(10);
	//尾插
	a1.pushBack(p1);
	a1.pushBack(p2);
	a1.pushBack(p3);
	a1.pushBack(p4);
	a1.pushBack(p5);

	//尾删
	a1.weiShan();


	for (int i = 0; i < a1.getLen(); i++)
	{
		cout << a1[i].age << a1[i].name << endl;
	}
}

int main()
{
	
	test();
	system("pause");
	return 0;

}
~~~





## string类

### 构造函数：

~~~c++
string();//创建一个空的字符串 例如: string str;

string(const string& str);//使用一个string对象初始化另一个string对象

string(const char* s);//使用字符串s初始化

string(int n, char c);//使用n个字符c初始化
~~~

 

### 基本赋值操作：

~~~c++
string& operator=(const char* s);//char*类型字符串 赋值给当前的字符串

string& operator=(const string &s);//把字符串s赋给当前的字符串

string& operator=(char c);//字符赋值给当前的字符串

string& assign(const char *s);//把字符串s赋给当前的字符串

string& assign(const char *s, int n);//把字符串s的前n个字符赋给当前的字符串

string& assign(const string &s);//把字符串s赋给当前字符串

string& assign(int n, char c);//用n个字符c赋给当前字符串

string& assign(const string &s, int start, int n);//将s从start开始n个字符赋值给字符串
~~~





 

### 基本字符操作：

~~~c++
char& operator[](int n);//通过[]方式取字符

char& at(int n);//通过at方法获取字符

~~~





 

### string拼接操作：

~~~c++
string& operator+=(const string& str);//重载+=操作符

string& operator+=(const char* str);//重载+=操作符

string& operator+=(const char c);//重载+=操作符

string& append(const char *s);//把字符串s连接到当前字符串结尾

string& append(const char *s, int n);//把字符串s的前n个字符连接到当前字符串结尾

string& append(const string &s);//同operator+=()

string& append(const string &s, int pos, int n);//把字符串s中从pos开始的n个字符连接到当前字符串结尾

string& append(int n, char c);//在当前字符串结尾添加n个字符c
~~~



 

### string比较操作：

~~~c++
//compare函数在>时返回 1，<时返回 -1，==时返回 0。
//比较区分大小写，比较时参考字典顺序，排越前面的越小。
//大写的A比小写的a小。
int compare(const string &s) const;//与字符串s比较
int compare(const char *s) const;//与字符串s比较

~~~





 

### string查找和替换：没找到返回-1

~~~c++
int find(const string& str, int pos = 0) const; //查找str第一次出现位置,从pos开始查找

int find(const char* s, int pos = 0) const; //查找s第一次出现位置,从pos开始查找

int find(const char* s, int pos, int n) const; //从pos位置查找s的前n个字符第一次位置

int find(const char c, int pos = 0) const; //查找字符c第一次出现位置

int rfind(const string& str, int pos = npos) const;//查找str最后一次位置,从pos开始查找

int rfind(const char* s, int pos = npos) const;//查找s最后一次出现位置,从pos开始查找

int rfind(const char* s, int pos, int n) const;//从pos查找s的前n个字符最后一次位置

int rfind(const char c, int pos = 0) const; //查找字符c最后一次出现位置

string& replace(int pos, int n, const string& str); //替换从pos开始n个字符为字符串str

string& replace(int pos, int n, const char* s); //替换从pos开始的n个字符为字符串s
~~~





 

### string子串：

~~~c++
string substr(int pos = 0, int n = npos) const;//返回由pos开始的n个字符组成的字符串
~~~



​    

 

### string插入和删除操作：

~~~c++
string& insert(int pos, const char* s); //插入字符串

string& insert(int pos, const string& str); //插入字符串

string& insert(int pos, int n, char c);//在指定位置插入n个字符c

string& erase(int pos, int n = npos);//删除从Pos开始的n个字符
~~~





 

### string和c-style字符串转换：

~~~c++
//string 转 char*
string str = "itcast";
const char* cstr = str.c_str();

//char* 转 string
char* s = "itcast";
string str(s);
~~~





 

### 代码示例：

~~~c++
#include<iostream>
#include<string>
#include<vector>
using namespace std;

/*
构造函数：
	string();//创建一个空的字符串 例如: string str;
	string(const string& str);//使用一个string对象初始化另一个string对象
	string(const char* s);//使用字符串s初始化
	string(int n, char c);//使用n个字符c初始化
*/

/*
基本赋值操作：
	string& operator=(const char* s);//char*类型字符串 赋值给当前的字符串
	string& operator=(const string &s);//把字符串s赋给当前的字符串
	string& operator=(char c);//字符赋值给当前的字符串
	string& assign(const char *s);//把字符串s赋给当前的字符串
	string& assign(const char *s, int n);//把字符串s的前n个字符赋给当前的字符串
	string& assign(const string &s);//把字符串s赋给当前字符串
	string& assign(int n, char c);//用n个字符c赋给当前字符串
	string& assign(const string &s, int start, int n);//将s从start开始n个字符赋值给字符串
*/

/*
基本字符操作：
	char& operator[](int n);//通过[]方式取字符
	char& at(int n);//通过at方法获取字符
*/


/*
 string拼接操作：
	string& operator+=(const string& str);//重载+=操作符
	string& operator+=(const char* str);//重载+=操作符
	string& operator+=(const char c);//重载+=操作符
	string& append(const char *s);//把字符串s连接到当前字符串结尾
	string& append(const char *s, int n);//把字符串s的前n个字符连接到当前字符串结尾
	string& append(const string &s);//同operator+=()
	string& append(const string &s, int pos, int n);//把字符串s中从pos开始的n个字符连接到当前字符串结尾
	string& append(int n, char c);//在当前字符串结尾添加n个字符c
*/

/*
string比较操作：
	compare函数在>时返回 1，<时返回 -1，==时返回 0。
	比较区分大小写，比较时参考字典顺序，排越前面的越小。
	大写的A比小写的a小。
	int compare(const string &s) const;//与字符串s比较
	int compare(const char *s) const;//与字符串s比较
*/

/*
string查找和替换：没找到返回-1
	int find(const string& str, int pos = 0) const; //查找str第一次出现位置,从pos开始查找
	int find(const char* s, int pos = 0) const;  //查找s第一次出现位置,从pos开始查找
	int find(const char* s, int pos, int n) const;  //从pos位置查找s的前n个字符第一次位置
	int find(const char c, int pos = 0) const;  //查找字符c第一次出现位置
	int rfind(const string& str, int pos = npos) const;//查找str最后一次位置,从pos开始查找
	int rfind(const char* s, int pos = npos) const;//查找s最后一次出现位置,从pos开始查找
	int rfind(const char* s, int pos, int n) const;//从pos查找s的前n个字符最后一次位置
	int rfind(const char c, int pos = 0) const; //查找字符c最后一次出现位置
	string& replace(int pos, int n, const string& str); //替换从pos开始n个字符为字符串str
	string& replace(int pos, int n, const char* s); //替换从pos开始的n个字符为字符串s
*/

/*
string子串：
	string substr(int pos = 0, int n = npos) const;//返回由pos开始的n个字符组成的字符串
*/

/*
string插入和删除操作：
	string& insert(int pos, const char* s); //插入字符串
	string& insert(int pos, const string& str); //插入字符串
	string& insert(int pos, int n, char c);//在指定位置插入n个字符c
	string& erase(int pos, int n = npos);//删除从Pos开始的n个字符
*/

/*
string和c-style字符串转换：
	string 转 char*
	string str = "itcast";
	const char* cstr = str.c_str();
	char* 转 string
	char* s = "itcast";
	string str(s);
*/

//查找
void test01()
{
	//查找子字符串第一次出现的位置，没找到返回-1
	string str1 = "hello world";
	int ret= str1.find("loo");
	cout <<ret<< endl;

	//查找所有子字符串出现的位置
	ret = 0;
	while ((ret = str1.find("l",ret)) != -1)
	{
		cout << ret << endl;
		ret++;
	}
}

//返回一个子字符串
void test02()
{
	string str1 = "hello world";
	cout << str1.substr(0, 5) << endl;
}

//string插入和删除字符
void test03()
{
	//插入字符
	string str1 = "hello world";
	str1.insert(5,"hehe");
	cout << str1 << endl;

	//删除字符
	str1.erase(5,4);
	cout << str1 << endl;
}

//大写转小写
void test04()
{
	string str1 = "ASFS425ADasdS//\\!DFSD1";
	for (int i = 0; i < str1.length(); i++)
	{
		if (str1[i] >='A' && str1[i] <= 'Z')
		{
			str1[i] += 32;
		}
	}
	cout << str1 << endl;
}

//提取子串
void test05()
{
	string str1 = "hehe:ha!ha:xix!ixi:la!lala:heihei:w!uwu";
	string str2[20];
	//提取到str2数组中
	for (int i = 0,ii=0; i < str1.length(); i++)
	{
		if (str1[i]==':'||str1[i]=='!')
		{
			ii++;
			continue;
		}
		str2[ii] += str1[i];
	}


	//遍历提取到字符串
	for (int i = 0; ; i++)
	{
		if (str2[i][0] == NULL)
			break;
		
		cout << str2[i] << endl;
	}
}

int main()
{
	test05();
	system("pause");
	return 0;
}
~~~







## STL标准模板库

### 迭代器

~~~c++
//迭代器的定义
//容器::iterator 迭代器名 = 返回迭代器的方法;
list<Person>::iterator it= p.begin();
~~~

| 输入迭代器     | 提供对数据的只读访问                                         | 只读，支持++、==、！=                   |
| -------------- | ------------------------------------------------------------ | --------------------------------------- |
| 输出迭代器     | 提供对数据的只写访问                                         | 只写，支持++                            |
| 前向迭代器     | 提供读写操作，并能向前推进迭代器                             | 读写，支持++、==、！=                   |
| 双向迭代器     | 提供读写操作，并能向前和向后操作                             | 读写，支持++、--，                      |
| 随机访问迭代器 | 提供读写操作，并能以跳跃的方式访问容器的任意数据，是功能最强的迭代器 | 读写，支持++、--、[n]、-n、<、<=、>、>= |





### 容器



#### vector容器



##### 说明

>单端动态数组，连续空间，有未雨绸缪机制。可预留空间，提高效率。头部操作效率低。使用随机迭代器

![image-20200830144306682](D:\zm\note\Readme.assets\image-20200830144306682.png)



##### 函数

~~~c++
//拷贝构造:
	vector<T> v; //采用模板实现类实现，默认构造函数
	vector(v.begin(), v.end());//将v[begin(), end())区间中的元素拷贝给本身。
	vector(n, elem);//构造函数将n个elem拷贝给本身。
	vector(const vector &vec);//拷贝构造函数。

//赋值操作
	assign(beg, end);//将[beg, end)区间中的数据拷贝赋值给本身。
	assign(n, elem);//将n个elem拷贝赋值给本身。
	vector& opera2tor=(const vector  &vec);//重载等号操作符
	swap(vec);// 将vec与本身的元素互换。

//vector的大小操作：
	size();//返回容器中元素的个数
	empty();//判断容器是否为空，判断size
	resize(int num);//重新指定容器的长度为num，若容器变长，则以默认值填充新位置。如果容器变短，则末尾超出容器长度的元素被删除。
	resize(int num, elem);//重新指定容器的长度为num，若容器变长，则以elem值填充新位置。如果容器变短，则末尾超出容器长>度的元素被删除。
	capacity();//容器的容量
	reserve(int len);//给容器预留容量，避免重复开辟、释放，提高效率

//vector数据存取操作：
	at(int idx); //返回索引idx所指的数据，如果idx越界，抛出out_of_range异常。
	operator[];//返回索引idx所指的数据，越界时，运行直接报错
	front();//返回容器中第一个数据元素
	back();//返回容器中最后一个数据元素

//vector插入和删除操作：
	insert(const_iterator pos, int count, ele);//迭代器指向位置pos插入count个元素ele.
	push_back(ele); //尾部插入元素ele
	pop_back();//删除最后一个元素
	erase(const_iterator start, const_iterator end);//删除迭代器从start到end之间的元素
	erase(const_iterator pos);//删除迭代器指向的元素
	clear();//删除容器中所有元素，删除的是元素个数，空间容量不变

~~~



##### 代码示例

~~~c++
#include<iostream>
using namespace std;
#include<vector>

/*
拷贝构造:
	vector<T> v; //采用模板实现类实现，默认构造函数
	vector(v.begin(), v.end());//将v[begin(), end())区间中的元素拷贝给本身。
	vector(n, elem);//构造函数将n个elem拷贝给本身。
	vector(const vector &vec);//拷贝构造函数。
*/

void myVectotPrintf(vector<int> &v)
{
	for (int i = 0; i < v.size(); i++)
	{
		cout << v[i]<<" ";
		/*int a = v[i];
		cout << a << endl;*/
	}
	cout <<endl;
}

void test01()
{
	//第一个构造
	//vector<T> v;
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);
	v1.push_back(4);
	myVectotPrintf(v1);

	//第二个构造
	//vector(v.begin(), v.end());
	int arr[] = {1,4,7,2,5,8};
	vector<int> v2(arr, arr+sizeof(arr)/sizeof(int));
	myVectotPrintf(v2);

	//第三个构造
	//vector(n, elem);
	vector<int> v3(10,1);
	myVectotPrintf(v3);

	//第四个构造
	//vector(const vector &vec);
	vector<int> v4(v1);
	myVectotPrintf(v4);
}

/*
赋值操作
	assign(beg, end);//将[beg, end)区间中的数据拷贝赋值给本身。
	assign(n, elem);//将n个elem拷贝赋值给本身。
	vector& operator=(const vector  &vec);//重载等号操作符
	swap(vec);// 将vec与本身的元素互换。
*/
void test02()
{
	int arr[] = { 1,4,7,2,5,8,3,6,9};
	vector<int> v1(arr, arr + sizeof(arr) / sizeof(int));

	//assign(beg, end);//将[beg, end)区间中的数据拷贝赋值给本身
	vector<int> v2;
	v2.assign(v1.begin()+3,v1.begin()+6);
	myVectotPrintf(v2);

	//assign(n, elem);//将n个elem拷贝赋值给本身
	vector<int> v3;
	v3.assign(5,1);
	myVectotPrintf(v3);

	//vector& operator=(const vector  &vec);//重载等号操作符
	vector<int> v4;
	v4 = v2;
	myVectotPrintf(v4);

	//swap(vec);// 将vec与本身的元素互换。
	v1.swap(v3);
	myVectotPrintf(v1);
	myVectotPrintf(v3);
}

/*
vector的大小操作：
	size();//返回容器中元素的个数
	empty();//判断容器是否为空，判断size
	resize(int num);//重新指定容器的长度为num，若容器变长，则以默认值填充新位置。如果容器变短，则末尾超出容器长度的元素被删除。
	resize(int num, elem);//重新指定容器的长度为num，若容器变长，则以elem值填充新位置。如果容器变短，则末尾超出容器长>度的元素被删除。
	capacity();//容器的容量
	reserve(int len);//给容器预留容量，避免重复开辟、释放，提高效率
*/
void test03()
{
	int arr[] = { 1,4,7,2,5,8,3,6,9 };
	vector<int> v1(arr, arr + sizeof(arr) / sizeof(int));

	//resize(int num);重新指定容器的长度
	//指定的是元素的个数。与容量无关
	v1.resize(0);
	myVectotPrintf(v1);

	//empty();//判断容器是否为空
	//判断容器是否为空根据元素的个数决定，与容量无关
	if (v1.empty())
	{
		cout<< "容器为空"<<endl;
	}
	
	//reserve(int len); 给容器预留容量，避免重复开辟、释放，提高效率
	vector<int> v2;
	v2.reserve(10000);
	cout<<v2.capacity()<<endl;
}

/*
vector数据存取操作：
	at(int idx); //返回索引idx所指的数据，如果idx越界，抛出out_of_range异常。
	operator[];//返回索引idx所指的数据，越界时，运行直接报错
	front();//返回容器中第一个数据元素
	back();//返回容器中最后一个数据元素
*/
void test04()
{
	//数据存取用中括号就行
	int arr[] = { 1,4,7,2,5,8,3,6,9 };
	vector<int> v1(arr, arr + sizeof(arr) / sizeof(int));
}

/*
vector插入和删除操作：
	insert(const_iterator pos, int count, ele);//迭代器指向位置pos插入count个元素ele.
	push_back(ele); //尾部插入元素ele
	pop_back();//删除最后一个元素
	erase(const_iterator start, const_iterator end);//删除迭代器从start到end之间的元素
	erase(const_iterator pos);//删除迭代器指向的元素
	clear();//删除容器中所有元素，删除的是元素个数，空间容量不变
*/
void test05()
{
	int arr[] = { 1,4,7,2,5,8,3,6,9 };
	vector<int> v1(arr, arr + sizeof(arr) / sizeof(int));

	//insert(const_iterator pos, int count, ele);
	v1.insert(v1.begin()+5,2,444);
	myVectotPrintf(v1);

	//push_back(ele); //尾部插入元素ele
	v1.push_back(123);
	myVectotPrintf(v1);

	//pop_back();//删除最后一个元素
	v1.pop_back();
	myVectotPrintf(v1);

	//erase(const_iterator start, const_iterator end);
	//删除迭代器从start到end之间的元素
	v1.erase(v1.begin()+5,v1.begin()+7);
	myVectotPrintf(v1);

	//erase(const_iterator pos);//删除迭代器指向的元素
	v1.erase(v1.begin()+4);
	myVectotPrintf(v1);

	//clear();//删除容器中所有元素,删除的是元素个数，空间容量不变
	v1.clear();
	cout << v1.capacity() << endl;
}

//巧用swap，收缩内存空间
void test06()
{
	vector<int> v;
	for (int i = 0; i < 100000; i++) {
		v.push_back(i);
	}

	cout << "capacity:" << v.capacity() << endl;
	cout << "size:" << v.size() << endl;

	//此时 通过resize改变容器的元素个数
	v.resize(10);

	cout << "capacity:" << v.capacity() << endl;
	cout << "size:" << v.size() << endl;

	//收缩内存空间
	//vector<int>(v)  相当于匿名对象，调用了拷贝
	//.swap(v)  把匿名对象和v容器交换
	vector<int>(v).swap(v);

	cout << "capacity:" << v.capacity() << endl;
	cout << "size:" << v.size() << endl;


}

int main()
{
	test06();
	system("pause");
	return 0;
}
~~~



#### deque容器

##### 说明

>双端队列，非连续空间，使用随机迭代器



![image-20200830150645872](D:\zm\note\Readme.assets\image-20200830150645872.png)



##### 函数

~~~c++
//构造方法：
	deque<T> deqT;//默认构造形式
	deque(beg, end);//构造函数将[beg, end)区间中的元素拷贝给本身。
	deque(n, elem);//构造函数将n个elem拷贝给本身。
	deque(const deque &deq);//拷贝构造函数。

//deque赋值操作：
	assign(beg, end);//将[beg, end)区间中的数据拷贝赋值给本身。
	assign(n, elem);//将n个elem拷贝赋值给本身。
	deque& operator=(const deque &deq); //重载等号操作符 
	swap(deq);// 将deq与本身的元素互换

//deque大小操作：
	deque.size();//返回容器中元素的个数
	deque.empty();//判断容器是否为空
	deque.resize(num);//重新指定容器的长度为num,若容器变长，则以默认值填充新位置。如果容器变短，则末尾超出容器长度的元素被删除。
	deque.resize(num, elem); //重新指定容器的长度为num,若容器变长，则以elem值填充新位置,如果容器变短，则末尾超出容器长度的元素被删除。

//deque双端插入和删除操作：
	push_back(elem);//在容器尾部添加一个数据
	push_front(elem);//在容器头部插入一个数据
	pop_back();//删除容器最后一个数据
	pop_front();//删除容器第一个数据

//deque数据存取：
	at(idx);//返回索引idx所指的数据，如果idx越界，抛出out_of_range。
	operator[];//返回索引idx所指的数据，如果idx越界，不抛出异常，直接出错。
	front();//返回第一个数据。
	back();//返回最后一个数据

//deque插入操作：
	insert(pos, elem);//在pos位置插入一个elem元素的拷贝，返回新数据的位置。
	insert(pos, n, elem);//在pos位置插入n个elem数据，无返回值。
	insert(pos, beg, end);//在pos位置插入[beg,end)区间的数据，无返回值。

//deque删除操作：
	clear();//移除容器的所有数据
	erase(beg, end);//删除[beg,end)区间的数据，返回下一个数据的位置。
	erase(pos);//删除pos位置的数据，返回下一个数据的位置。

~~~



#### stack容器

##### 说明

>栈容器，没有迭代器

![image-20200830151008898](D:\zm\note\Readme.assets\image-20200830151008898.png)



##### 函数

~~~c++
stack构造函数：
	stack<T> stkT;//stack采用模板类实现， stack对象的默认构造形式： 
	stack(const stack &stk);//拷贝构造函数

stack赋值操作：
	stack& operator=(const stack &stk);//重载等号操作符

stack数据存取操作：
	push(elem);//向栈顶添加元素
	pop();//从栈顶移除第一个元素
	top();//返回栈顶元素

stack大小操作：
	empty();//判断堆栈是否为空
	size();//返回堆栈的大小
~~~



##### 代码示例

~~~c++
#include<iostream>
using namespace std;
#include<stack>
#include<string>

/*
stack构造函数：
	stack<T> stkT;//stack采用模板类实现， stack对象的默认构造形式： 
	stack(const stack &stk);//拷贝构造函数

stack赋值操作：
	stack& operator=(const stack &stk);//重载等号操作符

stack数据存取操作：
	push(elem);//向栈顶添加元素
	pop();//从栈顶移除第一个元素
	top();//返回栈顶元素

stack大小操作：
	empty();//判断堆栈是否为空
	size();//返回堆栈的大小
*/

class Person
{
public:
	Person(const char * name,int age)
	{
		this->name = name;
		this->age = age;
	}

	string name;
	int age;
};

void test01()
{
	//栈容器
	stack<Person> p;
	//入栈
	p.push(Person("AA",10));
	p.push(Person("BB",20));
	p.push(Person("CC",30));
	p.push(Person("DD",40));
	p.push(Person("EE",50));

	//查看栈中元素个数
	cout<< p.size()<<endl;

	//判断栈是否为空
	while (!p.empty())
	{
		/*Person temp = p.top();
		cout << temp.name<<" " 
			<<temp.age << endl;*/
		//输出栈顶元素
		cout << p.top().age << " " <<
			p.top().name << endl;
		//出栈
		p.pop();
	}
}

int main()
{
	test01();

	system("pause");
	return 0;
}
~~~





#### queue容器

##### 说明

>队列容器，没有迭代器

![image-20200830151121370](D:\zm\note\Readme.assets\image-20200830151121370.png)



##### 函数

~~~c++
//queue构造函数：
	queue<T> queT;//queue采用模板类实现，queue对象的默认构造形式：
	queue(const queue &que);//拷贝构造函数

//queue存取、插入和删除操作：
	push(elem);//往队尾添加元素
	pop();//从队头移除第一个元素
	back();//返回最后一个元素
	front();//返回第一个元素

//queue赋值操作：
	queue& operator=(const queue &que);//重载等号操作符

//queue大小操作：
	empty();//判断队列是否为空
	size();//返回队列的大小
~~~



##### 代码示例

~~~c++
#include<iostream>
using namespace std;
#include<queue>
#include<string>

/*
queue构造函数：
	queue<T> queT;//queue采用模板类实现，queue对象的默认构造形式：
	queue(const queue &que);//拷贝构造函数

queue存取、插入和删除操作：
	push(elem);//往队尾添加元素
	pop();//从队头移除第一个元素
	back();//返回最后一个元素
	front();//返回第一个元素

queue赋值操作：
	queue& operator=(const queue &que);//重载等号操作符

queue大小操作：
	empty();//判断队列是否为空
	size();//返回队列的大小
*/

class Person
{
public:
	Person(const char * name, int age)
	{
		this->name = name;
		this->age = age;
	}

	string name;
	int age;
};

void test01()
{
	//队列容器
	queue<Person> p;
	//入队
	p.push(Person("AA", 10));
	p.push(Person("BB", 20));
	p.push(Person("CC", 30));
	p.push(Person("DD", 40));
	p.push(Person("EE", 50));

	//查看队列中元素个数
	cout << p.size() << endl;

	//判断队列是否为空
	while (!p.empty())
	{
		//输出队头
		cout<<"队头 ="
			<< p.front().age << " " <<
			p.front().name << endl;
		// 输出队尾
		cout << "队尾 ="
			<< p.back().age << " " <<
			p.back().name << endl;

		cout << endl;
		//出队
		p.pop();
	}
}

int main()
{
	test01();
	system("pause");
	return 0;
}
~~~





#### list容器

##### 说明

>双向链表，使用双向迭代器

![image-20200830151223209](D:\zm\note\Readme.assets\image-20200830151223209.png)



##### 函数

~~~c++
//list构造函数：
	list<T> lstT;//list采用采用模板类实现,对象的默认构造形式：
	list(beg, end);//构造函数将[beg, end)区间中的元素拷贝给本身。
	list(n, elem);//构造函数将n个elem拷贝给本身。
	list(const list &lst);//拷贝构造函数。
	
//list数据元素插入和删除操作：
	push_back(elem);//在容器尾部加入一个元素
	pop_back();//删除容器中最后一个元素
	push_front(elem);//在容器开头插入一个元素
	pop_front();//从容器开头移除第一个元素
	insert(pos, elem);//在pos位置插elem元素的拷贝，返回新数据的位置。
	insert(pos, n, elem);//在pos位置插入n个elem数据，无返回值。
	insert(pos, beg, end);//在pos位置插入[beg,end)区间的数据，无返回值。
	clear();//移除容器的所有数据
	erase(beg, end);//删除[beg,end)区间的数据，返回下一个数据的位置。
	erase(pos);//删除pos位置的数据，返回下一个数据的位置。
	remove(elem);//删除容器中所有与elem值匹配的元素。

//list大小操作：
	size();//返回容器中元素的个数
	empty();//判断容器是否为空
	resize(num);//重新指定容器的长度为num，
	若容器变长，则以默认值填充新位置。
	如果容器变短，则末尾超出容器长度的元素被删除。
	resize(num, elem);//重新指定容器的长度为num，
	若容器变长，则以elem值填充新位置。
	如果容器变短，则末尾超出容器长度的元素被删除。

//list赋值操作：
	assign(beg, end);//将[beg, end)区间中的数据拷贝赋值给本身。
	assign(n, elem);//将n个elem拷贝赋值给本身。
	list& operator=(const list &lst);//重载等号操作符
	swap(lst);//将lst与本身的元素互换。

//list数据的存取：
	front();//返回第一个元素。
	back();//返回最后一个元素。

//list反转排序：
	reverse();//反转链表，比如lst包含1,3,5元素，运行此方法后，lst就包含5,3,1元素。
	sort(); //list排序

~~~



##### 代码示例

~~~c++
#include<iostream>
using namespace std;
#include <string>
#include<list>

/*
list构造函数：
	list<T> lstT;//list采用采用模板类实现,对象的默认构造形式：
	list(beg, end);//构造函数将[beg, end)区间中的元素拷贝给本身。
	list(n, elem);//构造函数将n个elem拷贝给本身。
	list(const list &lst);//拷贝构造函数。
	
list数据元素插入和删除操作：
	push_back(elem);//在容器尾部加入一个元素
	pop_back();//删除容器中最后一个元素
	push_front(elem);//在容器开头插入一个元素
	pop_front();//从容器开头移除第一个元素
	insert(pos, elem);//在pos位置插elem元素的拷贝，返回新数据的位置。
	insert(pos, n, elem);//在pos位置插入n个elem数据，无返回值。
	insert(pos, beg, end);//在pos位置插入[beg,end)区间的数据，无返回值。
	clear();//移除容器的所有数据
	erase(beg, end);//删除[beg,end)区间的数据，返回下一个数据的位置。
	erase(pos);//删除pos位置的数据，返回下一个数据的位置。
	remove(elem);//删除容器中所有与elem值匹配的元素。

list大小操作：
	size();//返回容器中元素的个数
	empty();//判断容器是否为空
	resize(num);//重新指定容器的长度为num，
	若容器变长，则以默认值填充新位置。
	如果容器变短，则末尾超出容器长度的元素被删除。
	resize(num, elem);//重新指定容器的长度为num，
	若容器变长，则以elem值填充新位置。
	如果容器变短，则末尾超出容器长度的元素被删除。

list赋值操作：
	assign(beg, end);//将[beg, end)区间中的数据拷贝赋值给本身。
	assign(n, elem);//将n个elem拷贝赋值给本身。
	list& operator=(const list &lst);//重载等号操作符
	swap(lst);//将lst与本身的元素互换。

list数据的存取：
	front();//返回第一个元素。
	back();//返回最后一个元素。

list反转排序：
	reverse();//反转链表，比如lst包含1,3,5元素，运行此方法后，lst就包含5,3,1元素。
	sort(); //list排序
*/


class Person
{
public:
	Person(const char * name, int age)
	{
		this->name = name;
		this->age = age;
	}

	Person()
	{
	}

	//重载等号
	bool operator<(Person &p)
	{
		//age相同的情况下使用name排序
		if (this->age == p.age)
			return this->name < p.name;

		//age排序
		return this->age < p.age;
	}

	string name;
	int age;
};

void test01()
{
	list<Person> p;
	p.insert(p.end(), Person("AA", 12));
	p.insert(p.begin(), Person("BB", 13));
	p.insert(p.end(), Person("CC", 14));
	p.insert(p.begin(), Person("DD", 15));
	p.insert(p.end(), Person("EE", 16));
	p.insert(p.end(), Person("FF", 16));
	p.insert(p.end(), Person("DD", 16));

	//迭代器的定义
	//容器::iterator 迭代器名 = 返回迭代器的方法;
	//list<Person>::iterator it= p.begin();

	//遍历
	for (list<Person>::iterator it = p.begin();it!=p.end();)
		cout <<"姓名："<< (*(it++)).name<<"  年龄："<<it->age<<endl;
	cout << endl << endl;

	//逆序
	p.reverse();

	//遍历
	for (list<Person>::iterator it = p.begin(); it != p.end();)
		cout << "姓名：" << (*(it++)).name << "  年龄：" << it->age << endl;
	cout << endl << endl;

	//排序
	//自定义类型排序需要重载 < 号,升序
	//想逆序可以直接改重载 < 号的的代码
	//或者写个回调函数
	p.sort();
	//遍历
	for (list<Person>::iterator it = p.begin(); it != p.end();)
		cout << "姓名：" << (*(it++)).name << "  年龄：" << it->age << endl;

	
}

int main()
{
	test01();

	system("pause");
	return 0;
}
~~~



#### set/multiset容器

##### 说明

>set容器，插入数据会自动排序的容器，不允许元素重复
>
>multiset和set一样但是允许元素重复
>
>使用什么迭代器暂时不清楚待补充



##### 函数

~~~c++
//构造函数：
	set<T> st;//set默认构造函数：
	mulitset<T> mst; //multiset默认构造函数: 
	set(const set &st);//拷贝构造函数

//set赋值操作：
	set& operator=(const set &st);//重载等号操作符
	swap(st);//交换两个集合容器

//set大小操作：
	size();//返回容器中元素的数目
	empty();//判断容器是否为空

//set插入和删除操作：
	insert(elem);//在容器中插入元素。
	clear();//清除所有元素
	erase(pos);//删除pos迭代器所指的元素，返回下一个元素的迭代器。
	erase(beg, end);//删除区间[beg,end)的所有元素 ，返回下一个元素的迭代器。
	erase(elem);//删除容器中值为elem的元素。

//set查找操作：
	find(key);//查找键key是否存在,若存在，返回该键的元素的迭代器；若不存在，返回set.end();
	count(key);//查找键key的元素个数
	lower_bound(keyElem);//返回第一个key>=keyElem元素的迭代器。
	upper_bound(keyElem);//返回第一个key>keyElem元素的迭代器。
	equal_range(keyElem);//返回容器中key与keyElem相等的上下限的两个迭代器。
~~~



##### 代码示例

~~~c++
#include<iostream>
using namespace std;
#include<set>
#include<string>

class MySet
{
public:
	bool operator()(const int & mySet1, const int & mySet2)
	{
		return mySet1 > mySet2;
	}
};

/*
构造函数：
	set<T> st;//set默认构造函数：
	mulitset<T> mst; //multiset默认构造函数: 
	set(const set &st);//拷贝构造函数

set赋值操作：
	set& operator=(const set &st);//重载等号操作符
	swap(st);//交换两个集合容器

set大小操作：
	size();//返回容器中元素的数目
	empty();//判断容器是否为空

set插入和删除操作：
	insert(elem);//在容器中插入元素。
	clear();//清除所有元素
	erase(pos);//删除pos迭代器所指的元素，返回下一个元素的迭代器。
	erase(beg, end);//删除区间[beg,end)的所有元素 ，返回下一个元素的迭代器。
	erase(elem);//删除容器中值为elem的元素。

set查找操作：
	find(key);//查找键key是否存在,若存在，返回该键的元素的迭代器；若不存在，返回set.end();
	count(key);//查找键key的元素个数
	lower_bound(keyElem);//返回第一个key>=keyElem元素的迭代器。
	upper_bound(keyElem);//返回第一个key>keyElem元素的迭代器。
	equal_range(keyElem);//返回容器中key与keyElem相等的上下限的两个迭代器。
*/

void mySetPrintf(set<int> &s)
{
	int i = 0;
	for (set<int>::iterator it = s.begin();it != s.end();)
	{
		cout << *it << endl;
		it++;
		i++;
	}
	if (i == 0)
		cout << "NULL"<<endl;
	cout << endl << endl;
}

void mySetPrintf2(set<int,MySet> &s)
{
	int i = 0;
	for (set<int>::iterator it = s.begin(); it != s.end();)
	{
		cout << *it << endl;
		it++;
		i++;
	}
	if (i == 0)
		cout << "NULL" << endl;
	cout << endl << endl;
}

/*
set插入和删除操作：
	insert(elem);//在容器中插入元素。
	clear();//清除所有元素
	erase(pos);//删除pos迭代器所指的元素，返回下一个元素的迭代器。
	erase(beg, end);//删除区间[beg,end)的所有元素 ，返回下一个元素的迭代器。
	erase(elem);//删除容器中值为elem的元素。
*/
void test01()
{
	//初始化
	set<int> s1;

	//insert(elem);//在容器中插入元素。
	s1.insert(1);
	s1.insert(0);
	s1.insert(99);
	s1.insert(5);
	s1.insert(50);
	s1.insert(324);
	s1.insert(3453);
	mySetPrintf(s1);

	//删除指定元素
	//erase(pos);//删除pos迭代器所指的元素，返回下一个元素的迭代器
	for (set<int>::iterator it = s1.begin();it!=s1.end();)
	{	
		if ((*it) == 324)
		//it必须接收返回值，否则当前匀速被删迭代器失效
			it = s1.erase(it);
		it++;
	}
	mySetPrintf(s1);

	//erase(elem);//删除容器中值为elem的元素
	s1.erase(99);
	mySetPrintf(s1);

	//clear();//清除所有元素
	s1.clear();
	mySetPrintf(s1);

}

/*
set查找操作：
	find(key);//查找键key是否存在,若存在，返回该键的元素的迭代器；若不存在，返回set.end();
	count(key);//查找键key的元素个数
	lower_bound(keyElem);//返回第一个key>=keyElem元素的迭代器。
	upper_bound(keyElem);//返回第一个key>keyElem元素的迭代器。
	equal_range(keyElem);//返回容器中key与keyElem相等的上下限的两个迭代器。
*/
void test02()
{
	//初始化
	set<int> s1;
	//insert(elem);//在容器中插入元素。
	s1.insert(1);
	s1.insert(4);
	s1.insert(7);
	s1.insert(2);
	s1.insert(5);
	s1.insert(8);
	s1.insert(3);
	s1.insert(6);
	s1.insert(9);
	mySetPrintf(s1);

	//find(key);//查找键key是否存在,若存在，返回该键的元素的迭代器；若不存在，返回set.end();
	cout << *(s1.find(1)) << endl;

	//count(key);//查找键key的元素个数
	cout << s1.count(5) << endl;

	//lower_bound(keyElem);//返回第一个key >= keyElem元素的迭代器。
	cout << *s1.lower_bound(2) << endl;

	//upper_bound(keyElem);//返回第一个key > keyElem元素的迭代器。
	cout << *s1.upper_bound(2)<< endl;

	//equal_range(keyElem);//返回容器中key与keyElem相等的上下限的两个迭代器,
	//对组接收
	pair<set<int>::iterator, set<int>::iterator> pit = s1.equal_range(5);
	cout << "下一个元素的迭代器*(pid.second)="<<*(pit.second) << endl;//下一个元素的迭代器
	cout << "当前元素的迭代器*(pid.first)="<<*(pit.first) << endl;;//当前元素的迭代器
	
	//自定义排序规则降序
	set<int,MySet> s2;
	s2.insert(1);
	s2.insert(4);
	s2.insert(7);
	s2.insert(2);
	s2.insert(5);
	s2.insert(8);
	s2.insert(3);
	s2.insert(6);
	s2.insert(9);
	mySetPrintf2(s2);
}

int main()
{
	test02();

	system("pause");
	return 0;
}
~~~





#### map/multimap容器

##### 说明

>- map容器可以有一个键，一个值
>
>- 键和set容器是一样的



##### 函数

~~~c++
//map构造函数：
	map<T1, T2> mapTT;//map默认构造函数: 
	map(const map &mp);//拷贝构造函数
		
//map赋值操作：
	map& operator=(const map &mp);//重载等号操作符
	swap(mp);//交换两个集合容器
	
//map大小操作：
	size();//返回容器中元素的数目
	empty();//判断容器是否为空
	
//map插入数据元素操作：
	map.insert(...); //往容器插入元素，返回pair<iterator,bool>
	map<int, string> mapStu;
	// 第一种 通过pair的方式插入对象
	mapStu.insert(pair<int, string>(3, "小张"));
	// 第二种 通过pair的方式插入对象
	mapStu.inset(make_pair(-1, "校长"));
	// 第三种 通过value_type的方式插入对象
	mapStu.insert(map<int, string>::value_type(1, "小李"));
	// 第四种 通过数组的方式插入值
	mapStu[3] = "小刘";
	mapStu[5] = "小王";
	
//map删除操作：
	clear();//删除所有元素
	erase(pos);//删除pos迭代器所指的元素，返回下一个元素的迭代器。
	erase(beg, end);//删除区间[beg,end)的所有元素 ，返回下一个元素的迭代器。
	erase(keyElem);//删除容器中key为keyElem的对组。

//map查找操作：
	find(key);//查找键key是否存在,若存在，返回该键的元素的迭代器；/若不存在，返回map.end();
	count(keyElem);//返回容器中key为keyElem的对组个数。对map来说，要么是0，要么是1。对multimap来说，值可能大于1。
	lower_bound(keyElem);//返回第一个key>=keyElem元素的迭代器。
	upper_bound(keyElem);//返回第一个key>keyElem元素的迭代器。
	equal_range(keyElem);//返回容器中key与keyElem相等的上下限的两个迭代器。

~~~



##### 代码示例

~~~c++
#include<iostream>
using namespace std;
#include<map>
#include<string>
/*
map构造函数：
	map<T1, T2> mapTT;//map默认构造函数: 
	map(const map &mp);//拷贝构造函数
	
	
map赋值操作：
	map& operator=(const map &mp);//重载等号操作符
	swap(mp);//交换两个集合容器
	
map大小操作：
	size();//返回容器中元素的数目
	empty();//判断容器是否为空
	
map插入数据元素操作：
	map.insert(...); //往容器插入元素，返回pair<iterator,bool>
	map<int, string> mapStu;
	// 第一种 通过pair的方式插入对象
	mapStu.insert(pair<int, string>(3, "小张"));
	// 第二种 通过pair的方式插入对象
	mapStu.inset(make_pair(-1, "校长"));
	// 第三种 通过value_type的方式插入对象
	mapStu.insert(map<int, string>::value_type(1, "小李"));
	// 第四种 通过数组的方式插入值
	mapStu[3] = "小刘";
	mapStu[5] = "小王";
	
map删除操作：
	clear();//删除所有元素
	erase(pos);//删除pos迭代器所指的元素，返回下一个元素的迭代器。
	erase(beg, end);//删除区间[beg,end)的所有元素 ，返回下一个元素的迭代器。
	erase(keyElem);//删除容器中key为keyElem的对组。

map查找操作：
	find(key);//查找键key是否存在,若存在，返回该键的元素的迭代器；/若不存在，返回map.end();
	count(keyElem);//返回容器中key为keyElem的对组个数。对map来说，要么是0，要么是1。对multimap来说，值可能大于1。
	lower_bound(keyElem);//返回第一个key>=keyElem元素的迭代器。
	upper_bound(keyElem);//返回第一个key>keyElem元素的迭代器。
	equal_range(keyElem);//返回容器中key与keyElem相等的上下限的两个迭代器。
*/

class Person
{
public:
	Person()
	{}

	Person(string name,int age)
	{
		this->name = name;
		this->age = age;
	}

	string name;
	int age;
};

void myPerinfMap(map<int, Person> &p)
{
	for (map<int, Person>::iterator it = p.begin();
		it!=p.end();++it)
	{
		cout<< "ID=" <<(*it).first<<"  name="<<(*it).second.name 
			<<"  age="<<(*it).second.age <<endl;
	}
	cout << endl<<endl;
}

/*
map插入数据元素操作：
	map.insert(...); //往容器插入元素，返回pair<iterator,bool>
	map<int, string> mapStu;
	// 第一种 通过pair的方式插入对象
	mapStu.insert(pair<int, string>(3, "小张"));
	// 第二种 通过pair的方式插入对象
	mapStu.inset(make_pair(-1, "校长"));
	// 第三种 通过value_type的方式插入对象
	mapStu.insert(map<int, string>::value_type(1, "小李"));
	// 第四种 通过数组的方式插入值
	mapStu[3] = "小刘";
	mapStu[5] = "小王";
*/
void test01()
{
	map<int, Person> p;

	//第一种方式
	p.insert(pair<int,Person>(1,Person("BB",13)));

	//第二种方式
	p.insert(make_pair(0, Person("AA",12)));

	//第三种方式
	p.insert(map<int,Person>::value_type(-1,Person("CC",14)));

	//第四种方式
	//下标为键，ID
	//下标重复的情况下，修改ID
	p[-1] = Person("DD",15);
	p[4] = Person("EE", 16);
	p[3] = Person("FF", 17);
	myPerinfMap(p);
}

/*
map删除操作：
	clear();//删除所有元素
	erase(pos);//删除pos迭代器所指的元素，返回下一个元素的迭代器。
	erase(beg, end);//删除区间[beg,end)的所有元素 ，返回下一个元素的迭代器。
	erase(keyElem);//删除容器中key为keyElem的对组。
*/
void test02()
{
	//初始化
	map<int, Person> p;
	p[0] = Person("DD", 15);
	p[4] = Person("EE", 16);
	p[3] = Person("FF", 17);
	p[1] = Person("AA", 18);
	p[2] = Person("BB", 18);
	p[5] = Person("CC", 19);
	myPerinfMap(p);

	//erase(keyElem);//删除容器中key为keyElem的对组。
	//重复删除，没反应
	//p.erase(1);
	p.erase(1);
	myPerinfMap(p);

	//其方法同set容器不演示了
}

/*
map查找操作：
	find(key);//查找键key是否存在,若存在，返回该键的元素的迭代器；/若不存在，返回map.end();
	count(keyElem);//返回容器中key为keyElem的对组个数。对map来说，要么是0，要么是1。对multimap来说，值可能大于1。
	lower_bound(keyElem);//返回第一个key>=keyElem元素的迭代器。
	upper_bound(keyElem);//返回第一个key>keyElem元素的迭代器。
	equal_range(keyElem);//返回容器中key与keyElem相等的上下限的两个迭代器。
*/
void test03()
{
	//初始化
	map<int, Person> p;
	p[0] = Person("DD", 15);
	p[4] = Person("EE", 16);
	p[3] = Person("FF", 17);
	p[1] = Person("AA", 18);
	p[2] = Person("BB", 18);
	p[5] = Person("CC", 19);
	myPerinfMap(p);

	//find(key);//查找键key是否存在,若存在，返回该键的元素的迭代器；/若不存在，返回map.end();
	cout << "ID=" << (*p.find(1)).first << "  name="
		<< (*p.find(1)).second.name<<"  age="
		<< (*p.find(1)).second.age << endl;

	//equal_range(keyElem);//返回容器中key与keyElem相等的上下限的两个迭代器。
	pair<map<int, Person>::iterator, map<int, Person>::iterator> dz = p.equal_range(2);
	cout << "ID=\t" << dz.first->first << endl;
	cout << "age=\t" << dz.first->second.age << endl;
	cout << "name=\t"<<dz.first->second.name << endl;
	cout<<endl << endl;
	cout << "ID=\t" << dz.second->first << endl;
	cout << "age=\t" << dz.second->second.age << endl;
	cout << "name=\t" << dz.second->second.name << endl;
}

int main()
{
	test03();
	system("pause");
	return 0;
}
~~~





#### pair<>对组

##### 示例 1

~~~c++
pair<set<int>::iterator, set<int>::iterator> pit = s1.equal_range(5);
cout << "下一个元素的迭代器*(pid.second)="<<*(pit.second) << endl;//下一个元素的迭代器
cout << "当前元素的迭代器*(pid.first)="<<*(pit.first) << endl;;//当前元素的迭代器
~~~



##### 示例 2

~~~c++
pair<map<int, Person>::iterator, map<int, Person>::iterator> dz = p.equal_range(2);
cout << "ID=\t" << dz.first->first << endl;
cout << "age=\t" << dz.first->second.age << endl;
cout << "name=\t"<<dz.first->second.name << endl;
cout<<endl << endl;
cout << "ID=\t" << dz.second->first << endl;
cout << "age=\t" << dz.second->second.age << endl;
cout << "name=\t" << dz.second->second.name << endl;
~~~





### 算法

#### 内建函数

~~~c++
template<class T> bool less<T>//小于 降序排列
template<class T> bool greater<T>//大于 升序排列
~~~



#### 算法头文件

>- 算法主要是由头文件<algorithm> <functional> <numeric>组成。
>
>- <algorithm>是所有STL头文件中最大的一个,其中常用的功能涉及到比较，交换，查找,遍历，复制，修改，反转，排序，合并等...
>
>- <numeric>体积很小，只包括在几个序列容器上进行的简单运算的模板函数.
>
>- <functional> 定义了一些模板类,用以声明函数对象。



#### 遍历算法

##### 函数

~~~c++
遍历算法 遍历容器元素
	@param beg 开始迭代器
	@param end 结束迭代器
	@param _callback  函数回调或者函数对象
	@return 函数对象
	for_each(iterator beg, iterator end, _callback);

transform算法 将指定容器区间元素搬运到另一容器中
	注意 :目标不知道为什么要用resize指定长度，不然报错
	@param beg1 源容器开始迭代器
	@param end1 源容器结束迭代器
	@param beg2 目标容器开始迭代器
	@param _cakkback 回调函数或者函数对象
	@return 返回目标容器迭代器
	for_each(iterator beg1, iterator end1, iterator beg2, _callbakc)
~~~



##### 代码示例

~~~c++
#include<iostream>
using namespace std;
#include<string>
#include<vector>
#include<algorithm>

/*
遍历算法 遍历容器元素
	@param beg 开始迭代器
	@param end 结束迭代器
	@param _callback  函数回调或者函数对象
	@return 函数对象
	for_each(iterator beg, iterator end, _callback);
*/
class MyPrint
{
public:
	void operator()(int v)
	{
		//for_each对单个元素的操作写在这里
		cout<< v <<endl;
	}
};

void test01()
{
	vector<int> v;
	for (int i = 0; i < 10; i++)
		v.push_back(i);

	for_each(v.begin(),v.end(), MyPrint());
}


/*
transform算法 将指定容器区间元素搬运到另一容器中
注意 : transform 不会给目标容器分配内存，所以需要我们提前分配好内存
	@param beg1 源容器开始迭代器
	@param end1 源容器结束迭代器
	@param beg2 目标容器开始迭代器
	@param _cakkback 回调函数或者函数对象
	@return 返回目标容器迭代器
	for_each(iterator beg1, iterator end1, iterator beg2, _callbakc)
*/
class MyTransForm
{
public:
	int operator()(int v)
	{
		//for_each对单个元素的自定义赋值操作
		return v +100;
	}

};
void test02()
{
	vector<int> v;
	for (int i = 0; i < 10; i++)
		v.push_back(i);

	vector<int> v1;
	//这里不知道为什么要指定长度，不可以指定大小
	v1.resize(v.size());
	//transform算法 将指定容器区间元素搬运到另一容器中
	transform(v.begin(),v.end(),v1.begin(),MyTransForm());
	//遍历输出
	for_each(v1.begin(), v1.end(), MyPrint());
}


int main()
{
	test02();
	system("pause");
	return 0;
}
~~~





#### 查找算法

##### 函数

~~~c++
find算法 查找元素
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param value 查找的元素
	@return 返回第一个找到元素的迭代器
	find(iterator beg, iterator end, value)

find_if算法 条件查找
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param  callback 回调函数或者谓词(返回bool类型的函数对象)
	@return 返回第一个找的元素的迭代器
	find_if(iterator beg, iterator end, _callback);

adjacent_find算法 查找相邻重复元素
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@return 返回相邻元素的第一个位置的迭代器
	adjacent_find(iterator beg, iterator end);

binary_search算法 二分查找法
	注意: 在无序序列中不可用
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param value 查找的元素
	@return bool 查找返回true 否则false
	bool binary_search(iterator beg, iterator end, value);

count算法 统计元素出现次数
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@return int返回元素个数

count_if算法 统计元素出现次数
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param  callback 回调函数或者谓词(返回bool类型的函数对象)
	@return int返回元素个数
	count_if(iterator beg, iterator end, _callback);

~~~



##### 代码示例

~~~c++
#include<iostream>
using namespace std;
#include<vector>
#include<algorithm>

/*
find算法 查找元素
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param value 查找的元素
	@return 返回第一个找到元素的迭代器
	find(iterator beg, iterator end, value)
*/
void test01()
{
	//初始化容器
	vector<int> v;
	for (int i = 0; i < 10; i++)
		v.push_back(i);
	v.push_back(4);
	v.push_back(4);
	for_each(v.begin(), v.end(), [](int a) {cout << a << " "; });
	cout << endl;

	//find算法的使用
	//cout << *(find(v.begin(), v.end(), 4)) << endl;
	for (vector<int>::iterator ret = find(v.begin(), v.end(), 4);ret!=v.end();)
	{
		cout << *ret << endl;
		ret++;
		ret = find(ret, v.end(), 4);
	}
}

/*
find_if算法 条件查找
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param  callback 回调函数或者谓词(返回bool类型的函数对象)
	@return 返回第一个找的元素的迭代器
	find_if(iterator beg, iterator end, _callback);
*/
bool myFindIf(int a)
{
	//find_if写条件的地方
	return a > 1;
}
void test02()
{
	//初始化容器
	vector<int> v;
	for (int i = 0; i < 10; i++)
		v.push_back(i);
	for_each(v.begin(), v.end(), [](int a) {cout << a << " "; });
	cout << endl;

	//find_if条件查找
	cout << *(find_if(v.begin(), v.end(), myFindIf)) << endl;;
}

/*
	adjacent_find算法 查找相邻重复元素
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@return 返回相邻元素的第一个位置的迭代器
	adjacent_find(iterator beg, iterator end);
*/
void test03()
{
	//初始化容器
	vector<int> v;
	for (int i = 0; i < 10; i++)
		v.push_back(i);
	v.push_back(9);
	v.push_back(2);
	for_each(v.begin(), v.end(), [](int a) {cout << a << " "; });
	cout << endl;

	//adjacent_find算法，查找相邻的重复元素
	cout << *(adjacent_find(v.begin(),v.end())++) << endl;
}

/*
	binary_search算法 二分查找法
	注意: 在无序序列中不可用
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param value 查找的元素
	@return bool 查找返回true 否则false
	bool binary_search(iterator beg, iterator end, value);
*/
void test04()
{
	//初始化容器
	vector<int> v;
	for (int i = 0; i < 10; i++)
		v.push_back(i);
	for_each(v.begin(), v.end(), [](int a) {cout << a << " "; });
	cout << endl;

	//二分查找，返回bool，只可以用于有序容器
	cout << binary_search(v.begin(), v.end(), 9) << endl;
	cout << binary_search(v.begin(), v.end(), 10) << endl;;
}

/*
	count算法 统计元素出现次数
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@return int返回元素个数
*/
void test05()
{
	//初始化容器
	vector<int> v;
	for (int i = 0; i < 10; i++)
		v.push_back(i);
	v.push_back(4);
	v.push_back(4);
	v.push_back(4);
	for_each(v.begin(), v.end(), [](int a) {cout << a << " "; });
	cout << endl;

	//cout算法查找某个元素出现的次数，返回int
	cout << count(v.begin(),v.end(),4) << endl;
}

/*
count_if算法 统计元素出现次数
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param  callback 回调函数或者谓词(返回bool类型的函数对象)
	@return int返回元素个数
	count_if(iterator beg, iterator end, _callback);
*/
class MyCountIf
{
public:
	MyCountIf() 
	{
		this->temp = -1;
	}
	MyCountIf(int a)
	{
		this->temp = a;
	}
	
	//可以有多个参数
	int temp;

	bool operator()(int value)
	{
		//写条件的地方
		return value > this->temp;
	}

};
void test06()
{
	//初始化容器
	vector<int> v;
	for (int i = 0; i < 10; i++)
		v.push_back(i);

	for_each(v.begin(), v.end(), [](int a) {cout << a << " "; });
	cout << endl;

	//count_if算法，条件统计
	cout<<count_if(v.begin(),v.end(), MyCountIf(1))<<endl;

}





int main()
{
	test06();
	system("pause");
	return 0;
}
~~~



#### 排序算法

##### 函数

~~~c++
merge算法 容器元素合并，并存储到另一容器中
注意 : 两个容器必须是有序的
	@param beg1 容器1开始迭代器
	@param end1 容器1结束迭代器
	@param beg2 容器2开始迭代器
	@param end2 容器2结束迭代器
	@param dest  目标容器开始迭代器

sort算法 容器元素排序
	@param beg 容器1开始迭代器
	@param end 容器1结束迭代器
	@param _callback 回调函数或者谓词(返回bool类型的函数对象)
	sort(iterator beg, iterator end, _callback)

random_shuffle算法 对指定范围内的元素随机调整次序
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	random_shuffle(iterator beg, iterator end);

reverse算法 反转指定范围的元素
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	reverse(iterator beg, iterator end)
~~~



##### 代码示例

~~~c++
#include<iostream>
using namespace std;
#include<algorithm>
#include<vector>
/*
merge算法 容器元素合并，并存储到另一容器中
注意 : 两个容器必须是有序的
	@param beg1 容器1开始迭代器
	@param end1 容器1结束迭代器
	@param beg2 容器2开始迭代器
	@param end2 容器2结束迭代器
	@param dest  目标容器开始迭代器
*/
void test01()
{
	//初始化容器
	vector<int> v1;
	for (int i = 0; i < 10; i++)
		v1.push_back(i);
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; });
	cout << endl;
	vector<int> v2;
	for (int i = 5; i < 15; i++)
		v2.push_back(i);
	for_each(v2.begin(), v2.end(), [](int a) {cout << a << " "; });
	cout << endl;

	vector<int> v3;
	v3.resize(v1.size()+v2.size());
	//merge算法 
	merge(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin());
	for_each(v3.begin(), v3.end(), [](int a) {cout << a << " "; });
	cout << endl;
}


/*
sort算法 容器元素排序
	@param beg 容器1开始迭代器
	@param end 容器1结束迭代器
	@param _callback 回调函数或者谓词(返回bool类型的函数对象)
	sort(iterator beg, iterator end, _callback)
*/
void test02()
{
	//初始化容器
	vector<int> v1;
	for (int i = 0; i < 10; i++)
		v1.push_back(rand()%10);
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; });
	cout << endl;

	//排序算法默认升序
	sort(v1.begin(), v1.end());
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; }); cout << endl;
	//用内建函数对象，实现降序
	//排序需求复杂，可以自己写仿函数或者回调函数
	sort(v1.begin(), v1.end(),greater<int>());
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; }); cout << endl;
}

/*
random_shuffle算法 对指定范围内的元素随机调整次序
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	random_shuffle(iterator beg, iterator end);
*/
void test03()
{
	//初始化容器
	vector<int> v1;
	for (int i = 0; i < 10; i++)
		v1.push_back(i);
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; }); cout << endl;

	//random_shuffle算法，洗牌、打乱
	random_shuffle(v1.begin(),v1.end());
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; }); cout << endl;
}

/*
reverse算法 反转指定范围的元素
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	reverse(iterator beg, iterator end)
*/
void test04()
{
	//初始化容器
	vector<int> v1;
	for (int i = 0; i < 10; i++)
		v1.push_back(i);
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; }); cout << endl;

	//reverse算法，反转指定范围的元素
	reverse(v1.begin(), v1.end());
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; }); cout << endl;
}


int main()
{
	test04();
	system("pause");
	return 0;
}
~~~





#### 拷贝和替换算法

##### 函数

~~~c++
copy算法 将容器内指定范围的元素拷贝到另一容器中
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param dest 目标起始迭代器
	copy(iterator beg, iterator end, iterator dest)

replace算法 将容器内指定范围的旧元素修改为新元素
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param oldvalue 旧元素
	@param newvalue 新元素
	replace(iterator beg, iterator end, oldvalue, newvalue)

replace_if算法 将容器内指定范围满足条件的元素替换为新元素
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param callback函数回调或者谓词(返回Bool类型的函数对象)
	@param oldvalue 新元素
	replace_if(iterator beg, iterator end, _callback, newvalue);

swap算法 互换两个容器的元素
	@param c1容器1
	@param c2容器2
	swap(container c1, container c2);
~~~



##### 代码示例

~~~c++
#include<iostream>
using namespace std;
#include<vector>
#include<algorithm>

/*
copy算法 将容器内指定范围的元素拷贝到另一容器中
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param dest 目标起始迭代器
	copy(iterator beg, iterator end, iterator dest) 
*/
void test01()
{
	//初始化容器
	vector<int> v1;
	for (int i = 0; i < 10; i++)
		v1.push_back(i);
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; }); cout << endl;

	vector<int> v2;
	v2.resize(v1.size());
	//copy算法 将容器内指定范围的元素拷贝到另一容器中
	copy(v1.begin(), v1.end(),v2.begin());
	for_each(v2.begin(), v2.end(), [](int a) {cout << a << " "; }); cout << endl;
}

/*
replace算法 将容器内指定范围的旧元素修改为新元素
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param oldvalue 旧元素
	@param newvalue 新元素
	replace(iterator beg, iterator end, oldvalue, newvalue)
*/
void test02()
{
	//初始化容器
	vector<int> v1;
	for (int i = 0; i < 10; i++)
		v1.push_back(i);
	v1.push_back(4);
	v1.push_back(4);
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; }); cout << endl;

	//replace算法 将容器内指定范围的旧元素修改为新元素
	replace(v1.begin(),v1.end(),4,8);
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; }); cout << endl;
}

/*
replace_if算法 将容器内指定范围满足条件的元素替换为新元素
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param callback函数回调或者谓词(返回Bool类型的函数对象)
	@param oldvalue 新元素
	replace_if(iterator beg, iterator end, _callback, newvalue);
*/
class MyReplaceIf
{
public:
	bool operator()(int a)
	{
		//自定义规则处
		return a < 5;
	}

};
void test03()
{
	//初始化容器
	vector<int> v1;
	for (int i = 0; i < 10; i++)
		v1.push_back(i);
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; }); cout << endl;

	//replace_if算法 将容器内指定范围满足条件的元素替换为新元素
	replace_if(v1.begin(),v1.end(),MyReplaceIf(),5);
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; }); cout << endl;
}

/*
swap算法 互换两个容器的元素
	@param c1容器1
	@param c2容器2
	swap(container c1, container c2);
*/
void test04()
{
	//初始化容器
	vector<int> v1;
	vector<int> v2;
	for (int i = 0; i < 10; i++)
		v1.push_back(i);
	for (int i = 9; i>-1; i--)
		v2.push_back(i);
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; }); cout << endl;
	for_each(v2.begin(), v2.end(), [](int a) {cout << a << " "; }); cout << endl;

	//replace_if算法 将容器内指定范围满足条件的元素替换为新元素
	swap(v1, v2);
	cout << "交换后" << endl;
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; }); cout << endl;
	for_each(v2.begin(), v2.end(), [](int a) {cout << a << " "; }); cout << endl;
}



int main()
{
	test04();
	system("pause");
	return 0;
}
~~~





#### 常用算数算法

##### 函数

~~~c++
accumulate算法 计算容器元素累计总和
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param value累加值
	accumulate(iterator beg, iterator end, value)

fill算法 向容器中添加元素
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param value t填充元素
	fill(iterator beg, iterator end, value)
~~~



##### 代码示例

~~~c++
#include<iostream>
using namespace std;
#include<vector>
#include<algorithm>
#include<numeric>
/*
accumulate算法 计算容器元素累计总和
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param value累加值
	accumulate(iterator beg, iterator end, value)
*/
void test01()
{
	//初始化容器
	vector<int> v1;
	for (int i = 0; i < 10; i++)
		v1.push_back(i);
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; }); cout << endl;

	//accumulate算法 计算容器元素累计总和
	cout << accumulate(v1.begin(), v1.end(), 1000) << endl;;
	//int sum = accumulate(v1.begin(), v1.end(), 100);
	//cout << sum << endl;
}

/*
fill算法 向容器中添加元素
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param value t填充元素
	fill(iterator beg, iterator end, value)
*/
void test02()
{
	//初始化容器
	vector<int> v1;
	for (int i = 0; i < 10; i++)
		v1.push_back(i);
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; });
	cout << endl;

	//fill算法 向容器中添加元素
	fill(v1.begin(),v1.end(),12);
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; });
	cout << endl;
}

int main()
{
	test02();

	system("pause");
	return 0;
}
~~~





#### 常用集合算法

##### 函数

~~~c++
set_intersection算法 求两个set集合的交集
	注意:两个集合必须是有序序列
	@param beg1 容器1开始迭代器
	@param end1 容器1结束迭代器
	@param beg2 容器2开始迭代器
	@param end2 容器2结束迭代器
	@param dest  目标容器开始迭代器
	@return 目标容器的最后一个元素的迭代器地址

set_union算法 求两个set集合的并集
	注意:两个集合必须是有序序列
	@param beg1 容器1开始迭代器
	@param end1 容器1结束迭代器
	@param beg2 容器2开始迭代器
	@param end2 容器2结束迭代器
	@param dest  目标容器开始迭代器
	@return 目标容器的最后一个元素的迭代器地址

set_union算法 求两个set集合的并集
	注意:两个集合必须是有序序列
	@param beg1 容器1开始迭代器
	@param end1 容器1结束迭代器
	@param beg2 容器2开始迭代器
	@param end2 容器2结束迭代器
	@param dest  目标容器开始迭代器
	@return 目标容器的最后一个元素的迭代器地址

~~~



##### 代码示例

~~~c++
#include<iostream>
using namespace std;
#include<vector>
#include<algorithm>

/*
set_intersection算法 求两个set集合的交集
	注意:两个集合必须是有序序列
	@param beg1 容器1开始迭代器
	@param end1 容器1结束迭代器
	@param beg2 容器2开始迭代器
	@param end2 容器2结束迭代器
	@param dest  目标容器开始迭代器
	@return 目标容器的最后一个元素的迭代器地址
*/

/*
set_union算法 求两个set集合的并集
	注意:两个集合必须是有序序列
	@param beg1 容器1开始迭代器
	@param end1 容器1结束迭代器
	@param beg2 容器2开始迭代器
	@param end2 容器2结束迭代器
	@param dest  目标容器开始迭代器
	@return 目标容器的最后一个元素的迭代器地址
*/

/*
set_difference算法 求两个set集合的差集
	注意:两个集合必须是有序序列
	@param beg1 容器1开始迭代器
	@param end1 容器1结束迭代器
	@param beg2 容器2开始迭代器
	@param end2 容器2结束迭代器
	@param dest  目标容器开始迭代器
	@return 目标容器的最后一个元素的迭代器地址
*/


void test01()
{
	//初始化容器
	vector<int> v1;
	vector<int> v2;
	for (int i = 0; i < 10; i++)
		v1.push_back(i);
	for (int i = 5; i < 15; i++)
		v2.push_back(i);
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; }); cout << endl;
	for_each(v2.begin(), v2.end(), [](int a) {cout << a << " "; }); cout << endl;
	vector<int> v3;

	//set_intersection算法 求两个set集合的交集
	//交集，两个容器相同的元素
	v3.resize(max(v1.size(),v2.size()));
	set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin());
	for_each(v3.begin(), v3.end(), [](int a) {cout << a << " "; }); cout << endl;

	//set_union算法 求两个set集合的并集
	//两个容器二合一，所有不同的元素
	v3.resize(v1.size()+ v2.size());
	set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin());
	for_each(v3.begin(), v3.end(), [](int a) {cout << a << " "; }); cout << endl;

	//set_difference算法 求两个set集合的差集
	//v1的差集，v1独有的元素
	vector<int> v4;
	v4.resize(v1.size());
	set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), v4.begin());
	for_each(v4.begin(), v4.end(), [](int a) {cout << a << " "; }); cout << endl;


	//set_difference算法 求两个set集合的差集
	//v2的差集，v2独有的元素
	v4.resize(v2.size());
	set_difference(v2.begin(), v2.end(), v1.begin(), v1.end(), v4.begin());
	for_each(v4.begin(), v4.end(), [](int a) {cout << a << " "; }); cout << endl;
}


int main()
{
	test01();
	system("pause");
	return 0;
}
~~~







# Qt

## 创建项目的三个基类

>Qwidget：
>
>其他两个基类的父类，是一个空窗口
>
> 
>
>QMainWindow :
>
>自带一些控件的窗口，菜单栏，工具栏，铆接部件等
>
>可以通过函数返回控制控件的对象



## 对象树

>Qt中new出来的对象如果设置了父对象窗口，且父对象继承自QObject该对象就可以不手动释放，在父对象关闭后会自动释放



## 信号与槽

### 图解信号槽

![image-20200830155036137](D:\zm\note\Readme.assets\image-20200830155036137.png)



### 函数

>绑定信号和槽
>
>connect();
>
>参数1：发送信号的对象
>
>参数2：信号函数的入口地址
>
>参数3：使用槽函数的对象
>
>参数4：槽函数的入口地址，可以使用Lambda表达式



### 示例

~~~c++
//绑定信号
void(XueSheng:: *pp)() = &XueSheng::qk;
void(LaoShi:: *pp1)() = &LaoShi::chifan;
connect(this->ls,pp1,this->xs,pp);
//connect(this->ls,&LaoShi::chifan,this->xs,&XueSheng::qk);

//有参数的信号绑定槽函数，需要使用函数指针
void(XueSheng:: *p)(QString) = &XueSheng::qk;
void(LaoShi:: *p1)(QString) = &LaoShi::chifan;
connect(this->ls,p1,this->xs,p);

//Lambda表达式作为槽函数
connect(btn,&QPushButton::clicked,this,[=](){
     btn->setText("呵呵和");
});

//触发自定义信号,使用关键字 emit
emit this->ls->chifan();

//自定义信号
signals:
	//自定义信号，只写声明不写实现
	void chifan();
	void chifan(QString);

//自定槽函数的声明，槽函数需要写实现
 public slots:
	void qk();
	void qk(QString);
~~~



## Lambda表达式

>格式：
>
>​    [](){}：表示函数入口地址
>
>​    [](){}():表示函数调用
>
>
>
>- []：表达式里面使用外部数据，=表示值拷贝，&表示引用传递
>- ()：里面可以写参数，一般不写
>- {}：里面写函数体





## 多线程的使用方式2

>1. 写一个类继承obj类，里面写一个槽函数，槽函数里写要在其他线程执行的代码
>
> 
>
>2. 
>
>​    实例化自己写的类，要new出来的，不要指定父对象
>
>​    实例化一个Qthread对象，要new出来的，不要指定父对象
>
> 
>
>3. 把自己的写的类，对象放入Qthread对象中
>
>   ​      我的写的类对象->moveToThread(Qthread对象);
>
> 
>
>4. 绑定信号
>
>​    a、绑定槽函数启动信号：把Qthread对象的started信号，绑定到我写的类的槽函数
>
>​    b、释放Qthread对象的信号：把 Qthread对象的finished，绑定到Qthread对象 的deleteLater
>
>​    c、释放自己写的类对象的信号：，在类里面写一个槽函数，delete this，也绑定Qthread对象的finished信号
>
> 
>
>5. 启动线程：
>
>​    a、调用Qthread对象的start方法，发出信号，开始执行自己写的槽函数
>
>​    b、如果线程没有回收，再次调用start方法无用
>
> 
>
>6. 回收子线程
>
>​    a、调用Qthread对象的quit和exit方法
>
>​    b、调用quit和exit方法后，如果槽函数代码已经运行完毕，会销毁线程，同时发送finished信号，此时可以释放对象，同时也可以，再次调用start启动线程。 如果槽函数还没有运行完毕，等到槽函数运行完毕后才会销毁线程，发送finished信号，可以再次调用start启动线程



## Qt控件

### Qwidget-->空窗口

#### 控件模样

![image-20200830155532056](D:\zm\note\Readme.assets\image-20200830155532056.png)



#### 代码示例

~~~c++
//设置窗口名字
this->setWhatsThis("窗口");

// 设置窗口大小,大小可拖拽修改，大小单位应该是分辨率，像素
this->resize(600,400);

 // 设置窗口大小,大小不可以拖拽修改
this->setFixedSize(600,400);
~~~



### QPushButton->按钮

#### 控件模样

![image-20200830155733754](D:\zm\note\Readme.assets\image-20200830155733754.png)



#### 代码示例

~~~c++
//实例化按钮
QPushButton *bt1 =new QPushButton();
//显示按钮
bt1->show();
//设置按钮父对象
bt1->setParent(this);
//设置按钮文本
bt1->setText("呵呵");

//移动按钮位置
bt1->move(200,300);

//按钮单击信号
//&QPushButton::clicked单击信号 
//&Widget::close窗口关闭槽函数
connect(btn,&QPushButton::clicked,this,&Widget::close);
~~~



### QPushButton->按钮

#### 控件模样



#### 代码示例

~~~c++

~~~



### QMainWindow ->菜单

#### 控件模样

![image-20200830155830724](D:\zm\note\Readme.assets\image-20200830155830724.png)



#### 代码示例

~~~c++
//QmenuBar->菜单栏
//菜单栏 只能有一个
QMenuBar * bar =menuBar();


//Qmenu->菜单
//添加菜单
QMenu * fileMenu = bar->addMenu("文件");
QMenu * editMenu = bar->addMenu("编辑");


//Qaction->菜单项
//添加菜单项分割线
fileMenu->addSeparator();

//添加菜单项
QAction * openActino =fileMenu->addAction("新建");

//菜单项的单击事件
connect(newActino,&QAction::triggered,this,[](){
    qDebug()<<"呵呵";
});

//给菜单项添加图片 保存图片
QPixmap pix;
pix.load(":/new/prefix1/Image/butterfly.png");
//添加图片
newActino->setIcon(QIcon(pix));
~~~





### QToolBar ->工具栏

#### 控件模样

![image-20200830155945145](D:\zm\note\Readme.assets\image-20200830155945145.png)



#### 代码示例

~~~c++
//创建工具栏 可以有多个
QToolBar * toolBar = new QToolBar(this);

//状态栏放入当前窗口
//参数1：设置工具栏默认位置
//Qt::TopToolBarArea  上
//Qt::BottomToolBarArea  下
//Qt::LeftToolBarArea    左
//Qt::RightToolBarArea   右
this->addToolBar(Qt::LeftToolBarArea,toolBar);

//只允许左右停靠
toolBar->setAllowedAreas(Qt::LeftToolBarArea
                                | Qt::RightToolBarArea );

//不允许浮动在中间 必须停靠在一个地方
toolBar->setFloatable(false);

//不允许拖拽 固定位置
toolBar->setMovable(false);

//给工具栏添加菜单项
toolBar->addAction(newActino);
//添加菜单项之间的分割线
toolBar->addSeparator();
//给工具栏添加菜单项
toolBar->addAction(openActino);

~~~



### QStatusBar->状态栏

#### 控件模样

![image-20200830160022276](D:\zm\note\Readme.assets\image-20200830160022276.png)



#### 代码示例

~~~c++
//状态栏 只能有一个
QStatusBar * stBar = statusBar();
this->setStatusBar(stBar);

//左侧信息
QLabel *label1 = new QLabel("左侧信息");
stBar->addWidget(label1);

//右侧信息
QLabel *label2 = new QLabel("右侧信息");
stBar->addPermanentWidget(label2);

~~~



### Qdialog->弹出对话框

#### 控件模样

![image-20200830160101734](D:\zm\note\Readme.assets\image-20200830160101734.png)

#### 代码示例

~~~c++
//创建对话框（模态）
QDialog dl(this);
//设置对话框大小
dl.resize(150,50);
//已模态、阻塞的显示
dl.exec();

//非模态对话框创建
QDialog *dl1 =new QDialog(this);
//设置对话框大小
dl1->resize(180,60);
//正常显示，不阻塞、非模态
dl1->show();
//该对话框关闭后，对象释放
dl1->setAttribute(Qt::WA_DeleteOnClose);

//注意：非模态对话框因为不带阻塞，代码执行出了作用域会被自动释放，所以需要使用NEW出来的对象
~~~



### QradioButton->按钮组

#### 控件模样

![image-20200830160146119](D:\zm\note\Readme.assets\image-20200830160146119.png)



#### 代码示例

~~~c++
//默认选中那个按钮
ui->radioButton->setChecked(true);

//选中信号
connect(ui->radioButton,&QRadioButton::clicked,this,[](){
   qDebug()<<"选中男的了";
});

connect(ui->radioButton_2,&QRadioButton::clicked,this,[](){
   qDebug()<<"选中女的了";
});
~~~





### ListWidget->列表控件

#### 控件模样

![image-20200830160229374](D:\zm\note\Readme.assets\image-20200830160229374.png)



#### 代码示例

~~~c++
//列表头数据
QListWidgetItem *item =new QListWidgetItem(QString("静夜思"));
//设置列表对齐模式（头？）
item->setTextAlignment(Qt::AlignHCenter);
//加入列表头数据
ui->listWidget->addItem(item);
//列表数据
QStringList list;
list<<"江山如画犹无奈";
list<<"只与英雄做战场";
//放入列表数据
ui->listWidget->addItems(list);
~~~





### QtreeWidget->树控件

#### 控件模样

![image-20200830160314223](D:\zm\note\Readme.assets\image-20200830160314223.png)



#### 代码示例

~~~c++
//水平头信息
QStringList list2;
list2<<"英雄"<<"英雄介绍";
//水平头信息加入树控件
ui->treeWidget->setHeaderLabels(list2);

//添加顶层项目
QTreeWidgetItem * hero1 = new QTreeWidgetItem(QStringList()<<"枫");
//顶层项目放入树控件
ui->treeWidget->addTopLevelItem(hero1);
QTreeWidgetItem * hero2 = new QTreeWidgetItem(QStringList()<<"文侯");
ui->treeWidget->addTopLevelItem(hero2);
QTreeWidgetItem * hero3 = new QTreeWidgetItem(QStringList()<<"楚休红");
ui->treeWidget->addTopLevelItem(hero3);

//添加子控件
hero1->addChild(new QTreeWidgetItem(QStringList()<<"鼠虎鼠虎鼠虎鼠虎鼠虎鼠虎"));
hero2->addChild(new QTreeWidgetItem(QStringList()<<"文以载道文以载道文以载道"));

//单击选中事件
connect(ui->treeWidget,&QTreeWidget::itemClicked,this,[=](QTreeWidgetItem *item,int column){
    qDebug()<<item->text(column);
});
~~~

![image-20200830160357906](D:\zm\note\Readme.assets\image-20200830160357906.png)



### TableWidget->列表控件

#### 控件模样

![image-20200830160424101](D:\zm\note\Readme.assets\image-20200830160424101.png)

#### 代码示例

~~~c++
//设置表格的列数
ui->tableWidget->setColumnCount(3);
//设置表头信息
ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"姓名"<<"性别"<<"年龄");

//设置表格的行数
ui->tableWidget->setRowCount(5);
//准备表格信息
QStringList name;
QStringList sex;
name<<"枫"<<"太子"<<"曹闻道"<<"勇"<<"楚休红";
sex<<"女"<<"男"<<"男"<<"男"<<"男";

for (int i=0;i<5;++i)
{
    //给表格添加数据
    //参数1：插入第几行
    //参数2：插入第几列
    //参数3：需要插入的数据
    ui->tableWidget->setItem(i,0,new QTableWidgetItem(name[i]));
    ui->tableWidget->setItem(i,1,new QTableWidgetItem(sex[i]));
    ui->tableWidget->setItem(i,2,new QTableWidgetItem(QString::number(i+18)));
}

//单击选中事件
connect(ui->tableWidget,&QTableWidget::itemClicked,this,[=](QTableWidgetItem *item){
    qDebug()<<item->text();
});
~~~



### QcomboBox->下拉框

#### 控件模样

![image-20200830160522065](D:\zm\note\Readme.assets\image-20200830160522065.png)



#### 代码示例

~~~c++
//给下拉框放入内容
ui->comboBox->addItems(QStringList()<<"唯"<<"刀"<<"百"<<"辟");
//设置下拉框默认显示数据
ui->comboBox->setCurrentIndex(3);

//QComboBox::currentIndexChanged 发生重载 使用函数指针
void (QComboBox:: *p)(int) = &QComboBox::currentIndexChanged;

//当前选中事件 获取下拉框选中数据
connect(ui->comboBox,p,this,[=](int index){
    qDebug()<<ui->comboBox->itemText(index).toUtf8().data();
});
~~~



### QLabel->显示图片、gif动画

#### 控件模样

![image-20200830160551391](D:\zm\note\Readme.assets\image-20200830160551391.png)



#### 代码示例

~~~c++
//加载图片
QPixmap pix;
pix.load(":/Image/Sunny.jpg");
//图片放入label中
ui->label->setPixmap(pix);

//加载gif图片
QMovie * movie = new  QMovie(":/Image/mario.gif");
//设置gif图片尺寸
movie->setScaledSize(QSize(300,200));
//gif图片默认不播放 设置为播放状态
movie->start();
//gif图片放入labal中
ui->label_2->setMovie(movie);

//gif图片可以停止播放
//movie->stop();
~~~



## 自定义控件

### UI组合控件

#### 图解步骤



![image-20200830160838258](D:\zm\note\Readme.assets\image-20200830160838258.png)

![image-20200830160850072](D:\zm\note\Readme.assets\image-20200830160850072.png)



#### 代码

~~~c++
//QSpinBox::valueChanged 发生重载 使用指针
void(QSpinBox::*p)(int) = &QSpinBox::valueChanged;
//关联控件两个控件  一个改变发出信号  另外一个调用对应槽函数跟着改变
connect(ui->spinBox,p,ui->horizontalSlider,&QSlider::setValue);
connect(ui->horizontalSlider,&QSlider::valueChanged,ui->spinBox,&QSpinBox::setValue);


//自定义控件 可以提供一些自己需要的方法
void Form::setData(int data)
{
    ui->spinBox->setValue(data);
}
//自定义控件 可以提供一些自己需要的方法
int Form::getData()
{
    return ui->spinBox->value();
}
~~~



### 重写控件

#### 图解步骤

![image-20200830161033641](D:\zm\note\Readme.assets\image-20200830161033641.png)

![image-20200830161043584](D:\zm\note\Readme.assets\image-20200830161043584.png)

![image-20200830161052561](D:\zm\note\Readme.assets\image-20200830161052561.png)



#### 代码

~~~c++
#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>

class MyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(QWidget *parent = nullptr);

    //重写鼠标进入事件
    void enterEvent(QEvent *event);
    //重写鼠标离开事件
    void leaveEvent(QEvent *event);
    //重写鼠标移动
    void mouseMoveEvent(QMouseEvent *ev);
    //重写鼠标按下
    void mousePressEvent(QMouseEvent *ev);
    //重写鼠标释放
    void mouseReleaseEvent(QMouseEvent *ev);


    //重写事件分发
    bool event(QEvent *e);

signals:

public slots:
};

#endif // MYLABEL_H
~~~



~~~c++
#include "mylabel.h"
#include<QDebug>
#include<QEvent>
#include<QMouseEvent>

MyLabel::MyLabel(QWidget *parent) : QLabel(parent)
{
    //设置鼠标为跟踪模式，不用按下鼠标也能追踪鼠标移动
    this->setMouseTracking(true);

}

//重写鼠标进入事件
void MyLabel::enterEvent(QEvent *event)
{
    qDebug()<<"鼠标进入了";
}

//重写鼠标离开事件
void MyLabel::leaveEvent(QEvent *event)
{
    qDebug()<<"鼠标离开了";
}

//重写鼠标按下状态的移动
void  MyLabel::mouseMoveEvent(QMouseEvent *ev)
{
     qDebug()<<"鼠标移动了";
}

//重写鼠标按下
void  MyLabel::mousePressEvent(QMouseEvent *ev)
{
    qDebug()<<"鼠标按下了";
    qDebug()<<"鼠标按下位置的全局坐标"<<ev->globalX()<<ev->globalY();
    qDebug()<<"鼠标按下位置的当前控件坐标"<<ev->x()<<ev->y();
}
//重写鼠标释放
void  MyLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    //qDebug()<<"鼠标释放了";
}

bool MyLabel::event(QEvent *e)
{
    //拦截指定事件的事件
	//鼠标按下和单击事件 需要同时拦截双击事件才可以
    if(e->type() == QEvent::MouseButtonPress  || e->type() == QEvent::MouseButtonDblClick)
    {
        QMouseEvent * ev = (QMouseEvent*)e;
         qDebug()<<"拦截！！鼠标按下了";
         qDebug()<<"拦截！！鼠标按下位置的全局坐标"<<ev->globalX()<<ev->globalY();
         qDebug()<<"拦截！！鼠标按下位置的当前控件坐标"<<ev->x()<<ev->y();
        //返回true表示当前事件不要在处理
        return true;
    }

//    static int a;
//    qDebug()<<++a;

    //其他事件交还父类处理
    return QLabel::event(e);
}
~~~



## 定时器

>两种实现方法，通过重写定时器事件，或者使用定时器对象



~~~c++
//定时器对象 的实现

//给当前对象创建定时器
QTimer *timer =new QTimer(this);
//开始计时，并且给出每次发送信号的时间间隔
timer->start(500);//0.5s

//定时器发出的信号，捕获
connect(timer,&QTimer::timeout,this,[=](){
    static int lrTime=0;
    ui->label_2->setText(QString::number( lrTime++));
});

//可以暂停计时器
//timer->stop();

//简单的延时执行效果，5000表示5秒
QTimer::singleShot(5000,NULL,[=](){
    ui->label->setText("呵呵");
});
~~~



## 画图

>画图分为画家、画图引擎（工具）、画图设备



~~~c++
//在widget中画图需要重写事件
void Widget::paintEvent(QPaintEvent *)
{
///////////////////////////////基本设置 ///////////////////////////
//创建画家 this表示画图设备 让画家在当前窗口绘图
QPainter painter(this);
//创建一支画笔，同时指定颜色
QPen pen(QColor(255,0,0));
//可以设置笔的风格 虚线
//pen.setStyle(Qt::DotLine);
//可以设置笔的粗细
pen.setWidth(5);
//让画家拿起画笔
painter.setPen(pen);

//画线 给出两个点的坐标 画一条直线
painter.drawLine(QPoint(0,0),QPoint(100,100));
//画圆 参数1：圆心位置 参数2：横向的半径 参数3:纵向的半径
painter.drawEllipse(QPoint(100,100),50,50);
//画矩形（长方形正方形这种）
//第一个数，横向的坐标
//第二个数，纵向的坐标
//第三个数，横向长度
//第四个数，纵向长度
painter.drawRect(QRect(100,0,200,50));
//画文字
painter.drawText(QRect(10,200,60,50),"好好学习，天天向上" );


////////////////////////////高级设置 ///////////////////////////
//添加抗锯齿效果，参数里还可以选择其他的高级抗锯齿效果
painter.setRenderHint(QPainter::Antialiasing);
//画圆
painter.drawEllipse( QPoint(200,100),50,50 );
//移动画家基础坐标
painter.translate(100,0);
//保存当前画家位置
//painter.save();
//去往保存画家位置,如果没有保存的画家位置，则位置不变
//painter.restore();

///////////////////////////画成品图片 ///////////////////////////
//加载图片
QPixmap pix = QPixmap(":/Image/Luffy.png");
//画图片
painter.drawPixmap(0,0,pix);

//利用QImage 修饰像素点
QPainter painter3(this);
//创建画图设备
QImage img2;
//画图设备加入图片
img2.load(":/Image/Luffy.png");

//修改图片中的像素点
for(int i = 50;i<100;i++)
{
    for(int j = 50;j<100;j++)
    {
        //创建像素颜色
        QRgb value = qRgb(255,0,0);
        //根据坐标修改颜色
        img2.setPixel(i,j,value);
    }
}
//画图
painter.drawImage(0,0,img2);
}
///////////////////////////widget中也可以手动调用画图事件 /////////////////////////
//手动调用画图事件
this->update();


3、画图设备介绍
//利用QPixmap做绘图设备 会在不同平台上做了显示的优化
//创建画图设备
QPixmap pix(300,300);
//设置画图设备背景颜色
pix.fill(Qt::white);
//创建画家，传入画图设备
QPainter painter(&pix);
//传入画笔，和画笔颜色
painter.setPen(QPen(Qt::red));
//画圆
painter.drawEllipse(QPoint(150,150),100,100);
//画好图后 保存图片的位置
pix.save("D:\\zm\\pix.png");

//利用QImage做绘图设备  可以对像素级做访问
 //创建画图设备
QImage img(300,300,QImage::Format_RGB32);
//设置画图设备背景颜色
img.fill(Qt::white);
//创建画家，传入画图设备
QPainter painter2(&img);
//传入画笔，和画笔颜色
painter2.setPen(QPen(Qt::blue));
  //画圆
painter2.drawEllipse(QPoint(150,150),100,100);
 //画好图后 保存图片的位置
img.save("D:\\zm\\img.png");


//利用QPicture 做绘图设备  记录和重现画家的绘图指令
//创建画图设备
QPicture pic;
//创建画家
QPainter painter4;
//开始绘图
painter4.begin(&pic);
//笔的颜色
painter4.setPen(QPen(Qt::green));
//画图
painter4.drawEllipse(QPoint(150,150),100,100);
//结束绘图
painter4.end();
//保存画图指令的文件
pic.save("D:\\zm\\pic.lr"); //后缀名没有要求


//重现QPicture绘图指令
QPicture pic;
pic.load("D:\\zm\\pic.lr");

QPainter painter5(this);
//创建一支画笔，同时指定颜色
QPen pen(QColor(255,0,0));
//让画家拿起画笔
painter5.setPen(pen);

painter5.drawPicture(0,0,pic);
~~~



## 文件读写

~~~c++
//创建文件操作对象
QFile file("D://zm//35_my_code//Qt//untitled13//长恨歌_gbk.txt");
//打开方式和判断文件是否打开成功
if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
{
    qDebug()<<"文件打开失败";
    return;
}

//读取数据
QByteArray line;
while (!file.atEnd())
    line += file.readLine();
//使用gbk格式显示
QTextCodec *codec = QTextCodec::codecForName("gbk");
//显示数据到textEdit控件
ui->textEdit->setText(codec->toUnicode(line));
//默认utf-8显示
//ui->textEdit->setText(line);
//关闭文件操作对象
file.close();

//查看文件信息
QString qs("D://zm//35_my_code//Qt//untitled13//长恨歌_utf-8.txt");
QFileInfo info(qs);
qDebug() << "后缀名："<<info.suffix() << "文件名："<<info.fileName()
         << "文件路径："<<info.filePath()<<"文件大小："<<info.size();

//这里需要添加头文件QDateTime
qDebug() <<"创建日期："<<info.created().toString("yyyy-MM-dd hh:mm:ss")
        <<"最后修改日期："<<info.lastModified().toString("yyyy/MM/dd hh:mm:ss");
~~~



# linux基础

## 根目录下的常见目录

>/bin:    存放二进制可执行文件(系统命令)
>
>/dev:    存放 硬件设备所对应的文件（鼠标之类）
>
>/etc:    存放 系统配置文件
>
>/home:  家目录
>
>/peoc:   存放 系统核心，内存中的数据
>
>/tmp:   存放 用户程序工作期间产生的临时文件
>
>/var:    存放 系统工作期间产生的 经常变化的文件。（日志、数据库文件）
>
>/root:   root用户 家目录
>
>/lib:     存放 系统库
>
>usr:     存放 用户程序 相关 资源目录
>
>​       /usr/lib：用户库
>
>​       /usr/include/ 头文件



## 常用命令

### 查看端口占用

~~~shell
netstat -tnlp | grep :22
~~~



### 防火墙

~~~shell
#查看防火墙开放的端口
firewall-cmd --list-ports

#开放端口
firewall-cmd --zone=public --add-port=8084/tcp --permanent

#重启防火墙
firewall-cmd –reload

#停止防火墙
systemctl stop firewalld
    
#启动防火墙
systemctl start firewalld
~~~

 

 

### df 查看磁盘剩余空间

> 参数：-h人性化显示

 

### du 查看当前目录使用空间

>参数：-h 人性化显示
>
>注意：可以指定目录



### htop任务管理器

 

### man手册的使用

>向下翻页：空格
>
>上下翻页：PGUP、PGON
>
>上下翻动：上下方向键
>
>回到头部：Home
>
>回到尾部：END
>
>向下搜索：/
>
>向上搜索：？
>
>搜索上一个、下一个：n、N
>
>退出：q



### ls

#### 参数

>-l: 显示详细信息
>
>-a: 显示所有文件，包括隐藏
>
>-h: 显示人类可读信息，带有 单位 信息
>
>-i: 显示文件的inode节点



#### 命令  ls -l  解读

![image-20200830162051170](D:\zm\note\Readme.assets\image-20200830162051170.png)

>1、文件类型和权限
>
>​    a.文件的类型，第一个字符表示
>
>​    (1). 普通文件 -      占用磁盘存储。
>
>​    (2). 目录文件 d      占用磁盘存储。
>
>​    (3). 软链接文件 l     占用磁盘存储。
>
>​    (4). 块设备文件 b    伪文件，不占用磁盘存储。
>
>​    (5). 字符设备文件 c  伪文件，不占用磁盘存储。
>
>​    (6). 管道文件 p      伪文件，不占用磁盘存储。
>
>​    (7). 套接字 s        伪文件，不占用磁盘存储。
>
>​    (8). 未知文件
>
>​    b.文件的权限，后面的字符表示
>
>​    (1).r表示可读、 w表示可写、 x表示可执行
>
>​    (2).前三个所有者权限、中三同组权限、后三其他人权限
>
>2、硬连接数
>
>3、文件所有者
>
>4、文件所有组
>
>5、文件大小（单位字节）（4k 的整数倍）
>
>6、时间  文件首次创建的、最后一次修改的时间
>
>7、文件名



### shell 命令使用技巧

>history：列出过往执行过的命令
>
>​    使用 ！+ 命令编号，重新执行该命令
>
>Ctrl+a： 将光标移至最前端。
>
>Ctrl+e： 将光标移至最末端。
>
>Ctrl+u： 清空已输入的命令。



### 创建删除文件、目录

~~~shell
#创建文件语法：
touch 文件名

#创建目录语法：
mkdir 目录名
#参数：-p 创建多层目录

#删除文件、目录语法：
rm 文件、目录名
#参数：
	#-r  递归删除
	#-I 询问、交互式删除
~~~





 

 

### 软、硬链接

~~~SHELL
# 硬连接语法：
ln 源文件 硬链接文件

# 硬连接实现原理：源文件和硬链接文件之间有相同的inode节点 指向同一块扇区。

# 注意：不能给目录创建硬连接。目录的硬连接来自”.”和”..”
~~~





 ~~~shell
# 软连接语法：
ln -s  源文件  软链接文件

# 注意：
	# 1、软连接可以使用相对路径和绝对路径
	# 2、可以给目录创建软连接

 
 ~~~



### cp

~~~shell
#拷贝语法：
cp 源文件  新文件

#参数：-r 递归拷贝
~~~



### mv

~~~shell
#移动语法：
mv 源文件  新位置、新文件名
~~~





### which命令所在的位置

~~~shell
#查找命令语法：
which 需要查询的命令
~~~



 

### | 管道

~~~shell
# 将前一个命令的输出，作为后一个命令的输入。

#语法：
ps aux | grep xxx    #从 系统所有的进程中，过滤出 xxx 进程
~~~



 

 

### >、>>重定向 

>重定向，将前一个命令的输出，重定向到其他地方
>
>\>:  文件不存在创建,文件存在覆盖
>
>\>>: 文件不存在创建,文件存在追加



 

 

### cat、tac、more、head、tail

~~~shell
#查看文件语法：
cat 文件名

#逆序查看文件语法：
tac 文件名     
#-n：每行加编号

#查看大文件语法：
less 文件名

#显示前10行语法：
head  head -10 

#显示后10行语法：
tail  tail -10 
~~~



 

 

### 压缩、解压缩

~~~shell
# 压缩语法： 
tar -zcvf 压缩包名.tar.gz 压缩源材料
tar -jcvf 压缩包名.tar.bz2 压缩源材料

 
# 解压缩语法：
tar -zxvf 压缩包名.tar.gz -C 解压到什么路径
tar -jxvf 压缩包名.tar.bz2 

# 参数含义：
	# z: 使用gzip格式压缩

	# j: 使用bzip2 格式压缩

	# c: 创建压缩包（多个文件、只生成一个压缩包）

	# x：解压缩

	# v: 显示过程

	# f: 指定压缩包的名称。参数通常放在后面
~~~





 

 

### terr树状态目录显示

~~~shell
#语法：
terr 目录名
~~~





### ps 命令

~~~shell
#语法：
ps -aux
# 参数a：显示终端上的所有进程，包括其他用户的进程
# 参数u：显示所有进程的详细状态
# 参数x：显示没有控制终端的进程

#使用管道过滤需要的进程：
ps aux | grep xxx
~~~





### kill发送信号命令

~~~shell
#语法：
kill -9 进程PID

# 注意：
	# 9号信号的名称SIGKILL
	# 查看所有信号：kill -l
~~~



 

### 用户管理

~~~shell
# 添加用户
# 语法：
adduser 用户名

# 修改用户密码：
# 语法：
passwd 用户名

 

# 给用户管理员权限
chmod -v u+w /etc/sudoers
vim /etc/sudoers
# 找到 root ALL=(ALL) ALL
# 下面添加 用户名 ALL=(ALL) ALL
chmod -v u-w /etc/sudoers 
~~~





 

### 权限管理

~~~shell
# 修改文件拥有者
chown 用户名 需要修改的文件、目录名

# 修改文件所有组
chgrp 组名 需要修改文件/目录名

 

# 修改文件权限
chmod 777 文件名
# 7表示：
	# 读(r)  4
	# 写(w) 2
	# 执行(x) 1

# 第一个数字表示：所有者权限
# 第二个数字表示：所有组权限
# 第三个数字表示：其他人权限

 

 

# 权限对于目录和文件的不同作用
	# r对于文件：可以查看文件内容。cat、less、head
	# w对于文件：可以修改、删除文件内容。vim
	# x对于文件：可以被运行。./aout 
	# r对于目录：可以被查看。ls、tree  
	# w对于目录：可以被曾、删、改。mkdir、rm、mv、touch
	# x对于目录：可以被进入。cd

 
~~~





 

### find按文件属性查找

~~~shell
# 按名称找的语法：
find 需要查找的目录 -name “查找内容”
# 通配符：
	# * 表示零到多个任意字符
	# ? 表示一个任意字符

 

# 按类型找的语法：
find 需要查找的目录 -type “查找的类型”
# 类型：
	# 普通文件 f      
	# 目录文件 d      
	# 软链接文件 l     
	# 块设备文件 b    
	# 字符设备文件 c  
	# 管道文件 p      
	# 套接字 s        

 

# 按文件大小找的语法：
find 需要查找的目录 -size “+100M” -size “-1000M”
# 注意：
	# +100M 表示查找大于100M的文件
	# -1000M 表示查找小于1000M的文件
	# 不写单位单位的情况下默认是字节，其他单位k(小写)、M、G

 

# 指定搜索层级深度：
-maxdepth 1 # 放在目录后面，数字表示层级
# 语法：
find 需要查找的目录 -maxdepth 1 -name “查找内容”

 

# 对查询结果执行命令：
-exec # 放在结尾
#语法：
find 需要查找的目录 -name “查找内容” -exec 命令 {} \; 

 

#注意：
	#按类型、按文件、按大小、指定搜索深度、对查询结果执行命令，可以同时使用

 
# 了解，使用管道对查询结果执行命令 
find ./ -maxdepth 1 -type f | xargs ls -l
# 【问题】：当有一个 含有空格的文件存在时，上述实现报错
find ./ -maxdepth 1 -type f -print0 | xargs -0 ls -l # 解决 上述问题。


# 了解，可扩展：-ctime、-atime、-mtime -cmin -amin -mmin
~~~





 

 

### grep按文件内容查找

~~~shell
# 语法：
grep -rn “查找的内容” 查找的目录 
# 参数：
	# n：可以显示查找内容的所在的行号
	# r：递归查找

# 注意：
	# find可以和grep结合使用 示例：
	find ./ -name "*.c" -exec grep "main" -n {} \;
~~~



 

 

### ssh文件上传、下载

~~~shell
# 上传语法：
scp -r 本地文件 目标主机用户名@目标主机IP:绝对存储路径（不能使用相对）

# 下载语法：
scp -r 目标主机用户名@目标主机IP:待下载文件的绝对存储路径（不能使用相对） 本地存储位置（相对、绝对）

# 注意：需要输入密码
~~~





### ssh别名配置、免密码登录

>**别名配置**
>
>找到 .ssh 目录，在config目录下添加内容
>
>Host 别名 
>
>HostName 目标ip
>
>User 登录用户名
>
>IdentitiesOnly yes



>**免密码登录**
>
>1. 生成秘钥（默认名称id_rea）：ssh-keygen -t rsa -C "825422719@qq.com"
>2. 找到 .ssh 目录的id_rsa.pub 拷贝到服务器上
>3. Id_res 中的秘钥添加到 服务器 家 目录下的 .ssh 目录下的 authorized_keys 文件中



## vim

### 命令模式

>复制 : yy 复制多行  行数加yy
>
>粘贴：小p粘贴到光标前面，大P粘贴到光标后面
>
>剪切：dd 剪切多行  行数加dd
>
>多行选中：V 加 方向键
>
>撤销：u
>
>反撤销：Ctrl +r
>
>搜索：/向下搜索  ?向上搜索  小写n搜索下一个
>
>分屏显示：vsp 需要打开的文件名  
>
>​        还可以使用vs上下分屏显示
>
>​        Ctrl+w切换
>
>​        wqall 一次退出所有分屏
>
>自动对齐：gg=G  gg是跳转行首可以省略



### 末行模式

>q：退出
>
>q！：强制退出不保存
>
>w：保存
>
>wq：保存退出
>
>1：数字几就跳转第几行
>
>！：执行shell命令



### 基础配置

~~~shell
set tabstop=4 "tab 4个空格
set shiftwidth=4 	"自动缩进4个空格
set nu	"显示行号
syntax on "语法高亮

# 也可以使用vimplus，进行配置
~~~



## gcc

### gcc编译的4个步骤

>1. 预处理：-E
>
>   - 生成 .i 文件
>
>   - 头文件展开
>   - 宏替换
>   - 注释删除
>
>2. C转汇编：-S
>   - 生成 .s 汇编代码
>
>3. 汇编：-c
>   - 生成 .o 二进制文件
>
>4. 链接：
>
>   - 链接所有 .o 文件
>
>   - 生成可执行程序



### gcc 常用参数

| **gcc编译选项**                          | **选项的意义**                                               |
| ---------------------------------------- | ------------------------------------------------------------ |
| -E                                       | 预处理指定的源文件，不进行编译                               |
| -S                                       | 编译指定的源文件，但是不进行汇编                             |
| -c                                       | 编译、汇编指定的源文件，但是不进行链接                       |
| -o  [file1] [file2] / [file2] -o [file1] | 将文件 file2 编译成可执行文件 file1                          |
| -I  directory                            | 指定 include 包含文件的搜索目录                              |
| -g                                       | 在编译的时候，生成调试信息，该程序可以被调试器调试           |
| -D                                       | 在程序编译的时候，指定一个宏                                 |
| -w                                       | 不生成任何警告信息                                           |
| -Wall                                    | 生成所有警告信息                                             |
| -On                                      | n的取值范围：0~3。编译器的优化选项的4个级别，-O0表示没有优化，-O1为缺省值，-O3优化级别最高 |
| -l                                       | 在程序编译的时候，指定使用的库                               |
| -L                                       | 指定编译的时候，搜索的库的路径。                             |
| -fPIC/fpic                               | 生成与位置无关的代码                                         |
| -shared                                  | 生成共享目标文件。通常用在建立共享库时                       |
| -std                                     | 指定C方言，如:-std=c99，gcc默认的方言是GNU C                 |



## 库的制作和使用

### 静态库

>- **静态库的特点**
>
>  - 编译阶段加入程序
>
>  - 优点
>    - 静态库被打包到应用程序中加载速度快
>    - 发布程序无需提供静态库，移植方便
>
>  - 缺点
>
>    - 消耗系统资源，浪费内存
>
>    - 更新、部署、发布麻烦
>
> 
>
>- **静态库的制作**：
>
>  1. 通过gcc -c 编译源文件获得 .o 二进制文件
>
>  2. 使用 ar 把 .o 文件打包，生成 .a 库文件
>
>     - 示例：ar rcs libcalc.a add.o div.o mult.o sub.o 
>
>       - ar 命令
>
>       - rcs 参数 
>
>       - libcalc.a 
>         - lib前缀
>
>         - .a 后缀
>
>         - calc 静态库名称
>
> 
>
>- **静态库的使用**
>
>  - 指定头文件位置
>    - -I 参数 （大写i）
>
>  - 指定静态库位置
>    - -L 参数
>
>  - 指定静态库的名称
>
>    - -l 参数 （小写L）
>
>    - 不要静态库文件的前后缀，只要名称



### 动态库/共享库

>- **动态库的特点**
>
>  - 程序运行时加入程序，多个程序共享一份动态库文件
>
>  - 优点
>
>    - 可实现进程间资源共享
>
>    - 程序升级简单
>
>    - 程序员可以控制何时加载动态库
>
>  - 缺点
>
>    - 加载速度比静态库慢
>
>    - 发布程序需要提供依赖的动态库
>
> 
>
>- **动态库的制作**
>
>  - 使用gcc 得到 .o 文件
>    - 示例：gcc -c sub.c add.c div.c mult.c -fpic
>      - -fpic 动态库需要加-fpic参数，得到于位置无关的代码
>
>  - 使用gcc 得到 .so动态库文件
>    - 示例：gcc -shared add.o sub.o div.o mult.o -o libcalc.so
>      - lib前缀
>      - .so 后缀
>      - calc 动态库名称
>
> 
>
>- **动态库的使用**
>
>  - 指定头文件位置
>    - -I （大写i）
>
>  - 指定动态库位置
>    - -L 
>
>  - 指定动态库的名称
>
>    - -l （小写L）
>
>    - 不要动态库文件的前后缀，只要名称
>
> 
>
>- **找不到动态库文件解决**
>
>  - ldd 程序名 ，检测程序是否链接到动态库
>
>  - 解决方法1：动态库的绝对路径添加到 LD_LIBRARY_PATH 中
>
>    - 临时生效（当前终端有效）
>      - 执行该命令：export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:动态库的绝对路径和名称
>      - 库文件名称不要前后缀
>
>    - 用户级配置
>
>      - 把该代码添加到 ~ 目录下的 .bashrc 配置文件尾部：export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:动态库的绝对路径和名称
>
>      - 库文件名称不要前后缀
>
>    - 系统级配置
>
>      - 把该代码添加到 /etc/ 目录下的 profile配置文件尾部：export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:动态库的绝对路径
>
>      - 库文件名称不要前后缀
>
>        
>
>  - 解决方法2：在 /etc/ld.so.cache 添加动态库路径
>
>    - 无法直接修改 /etc/ld.so.cach 需要间接修改
>
>    - 在 /etc/ld.so.conf 配置文件中添加库所在的路径，不要名称
>
>    - 执行一个命令, 进行更新: sudo ldconfig
>
>  
>
>  - 解决方法3：库文件放入 /use/lib/ 或者 /lib/ 目录中
>
> 
>
>后期添加：
>
>​    使用VS时
>
>头文件放入：/usr/local/include/
>
>库文件放入：/usr/lib



## 简单的Makefile编写

### makefile中的规则语法：

>目标 … : 依赖 …
>
>​       命令(shell命令)
>
>
>
>- 目标：最终要生成的文件，伪目标不算（.PHINY:）
>- …：可以有一个或者多个生成文件或者依赖
>- 依赖：生成目标的需要的材料
>- 命令：通过命令对依赖操作生成目标，命令前面是tab缩进
>- 一个makefile中可以有多条规则，后面的规则一般为前面的规则服务，如果前面的规则找不到依赖的文件，就会执行后面的规则，如果依赖文件足够就不会执行后面的依赖



 

### makefile中的变量

>- 自动变量
>  - $@: 规则中的目标
>  - $<: 规则中的第一个依赖
>  - $^: 规则中的所有依赖



 

### makefile中的函数

> - $(wildcard ./*.c) 获取文件名，多个目录用空格分开
>
> - $(patsubst %.c,%.o,$(src)) 字符串替换
>   - 参数：查找参数3字符串中的单词，(单词以“空格”、“tab”、”回车”、“换行”分割),是否符合参数1的单词格式，如果符合使用参数2的单词替换，结果使用变量接收，参数三的数据不会改变
>   - %号是通配符，表示任意长度的字符串，如果使用两个百分号，那第二个百分号中的内容来源就是第一个百分号中的内容



 

### 代码示例

~~~makefile
#定义变量保存.c文件的名称
#通过wildcard函数获取当前目录下所有.c文件名称
src=$(wildcard ./*.c)
#定义变量保存.o文件名称
#通过patsubst函数获取src变量中.c文件对应的.o文件名称
objs=$(patsubst %.c,%.o,$(src))
#定义变量保存可执行程序的名称
target=app

#规则含义：通过objs中保存的.o文件生成名称为target变量值的可执行程序
#：号前面表示目标，：后面表示依赖
$(target):$(objs)
#gcc命令
	gcc $(objs) -o $(target)

#规则含义：任何.o,都通过同名.c生成
#%号是通配符，表示一个字符串
#两个百分号表示同一个字符串
%.o:%.c
# $< 自动变量，表示这条规则的第一个依赖
# $@ 自动变量，表示这条规则的第一个目标 
	gcc -c $< -o $@

#.PHINY:声明当前目标是伪目标，不会检测依赖的时间
.PHINY:clean
#这条规则用于执行 程序的清理
clean:
#清理所有当前make生成的所有.o文件和可执行程序
	rm -f $(objs) $(target)
~~~





# 数据库

## 增删改查

### select 

#### 语法

~~~sql
select：结果显示
	DISTINCT：去除重复结果
from：	表名
where：对表的数据进行筛选
	1、模糊查询
		NOT LIKE：相当于 !=
		LIKE：相当于 =
		%：表示0到多个字符
		_：表示一个字符
		escape '/'：自定义转义字符 
	2、筛选NULL数据
		is：相当于 =
		is not：相当于不等于 !=	
group by：分组，分组后select只能显示分组的字段，或者统计函数
having：对分组结果进行筛选，能使用统计函数的结果对分组结果进行筛选
order by：排序，asc升序，desc降序
~~~



#### select 查询逻辑

>首先从from中查询结果，
>
>然后通过where进行筛选，
>
>然后通过group by进行分组，
>
>然后通过having对分组结果筛选，可以使用统计函数筛选
>
>然后使用order by 进行排序，
>
>最后通过 select 显示



 

#### **注意点**

##### **外连接**

**说明**

>- 外连接就是多表查询时，保留没有通过条件筛选的数据，
>
>- 可用于多表查询分组时，统计数据为0的组

​    

**使用方法：**

> 1、使用jion，on连接表
>
> 2、然后在jion前面加left 或者right 或者full
>
> 3、加left就是左外连接，保留左边表的数据
>
> 4、加right就是右外连接，保留右边表的数据
>
> 5、加full就是全外连接，保留两张表的数据
>
> > jion的使用：
> >
> > ​       1、jion替换from中的逗号
> >
> > ​       2、on 替换 where
> >
> > ​       3、on 后面可以接着写where

 

##### **子查询**

 **说明：**

>- 一个查询的select语句里边嵌套了另一个select语句
>
>- select、from、where、having都可以使用子查询
>
>- 子查询返回多行数据使用IN、ANY、ALL判断
>
> - IN()：= 符号
>   - IN(子查询)
>
> - ANY：集合中的任意一个数据
>   - **>=** ANY(子查询)
> - ALL：集合中的所有数据
>   - **>=**ALL(子查询)



**小示例：查询工资大于李磊员工** 

~~~sql
select *
from 员工表
where 工资 > 
(
	select 工资
	from 员工表
	where ename = '李雷'
)


~~~

  

 

 

##### **集合：**

**说明：**

>- 在一个select语句下面在接一个select语句使用集合关键字进行分割



**关键字：**

~~~sql  
并集 union
全并集 union all
交集 intersect
差集 minus
~~~



**示例：**

~~~sql
select *
from emp
where deptno = 10
union all
select * 
from emp
~~~



​        

##### **mysql Top**

~~~mysql
limit：m,n 
	-- m可选， 表示跳过前面m条记录
	-- n必填， 只显示n条记录
~~~



 

##### Oraclet Top

~~~sql
-- 取前几条数据：
WHERE ROWNUM < 3
~~~

~~~sql
-- 由于先判断where 后执行排序所以取Top N，需要使用子查询先排序：
SELECT *
FROM (
	SELECT *
	FROM EMP
	ORDER BY DEPTNO asc
) 
WHERE ROWNUM < 3
~~~





 

### insert into 

**语法：**

~~~sql
insert into 表名(列名,列名2.....)
values(…………),
values(…………),
………;

-- 列名可以忽略
~~~

 

**示例：**

~~~sql
INSERT INTO gua_64 (gua_id,gua_name,gua_message)VALUES
(1,'乾为天','元亨利贞。'),
(2,'坤为地','元亨。。'); 
~~~



### update

~~~sql
update 表名 set 列名1=值1,列名2=值2..... where 更新条件
~~~



### delete

~~~sql
delete 表名 where 删除条件
~~~



## Oracle 

![image-20200831183144381](D:\zm\note\Readme.assets\image-20200831183144381.png)

​                               

### 基本操作

>- 管理员登陆： sqlplus / as sysdba 
>
>- 启动：   startup
>
>- 停止：   shutdown
>
>- 解锁用户：   alter user 用户名account unlock;
>
>- 修改密码：   alter user 用户名identified by "密码";
>
>- 本地登陆：   sqlplus 用户名/密码
>
>- 远程登陆：   sqlplus 数据库用户名/密码@//IP地址/orcl
>
>- 查看当前用户名称：   show user
>
>- 查看当前用户拥有的表：   select * from tab
>
>- 查看当前表结构：  desc 表名
>
>- 开启监听服务，开启后允许远程登陆（在linux终端执行）
> - 启动：   lsnrctl start
> - 停止：   lsnrctl stop
> - 查看状态：   lsnrctl status
>
>- 查看用户的表
>
> ~~~sql
> select * from all_tables where owner='用户名';
> ~~~



 

 

### 常用函数

**统计函数**

~~~sql
sum -- 求和
	sum(字段)

count -- 统计个数，NULL的情况不统计
	count(字段)
	count(distinct 字段)，统计中去重

max/min -- 返回最大 或 最小值
	max(字段)
	min(字段)

avg -- 求平均值，忽略值为NULL的字段
	avg(字段)
~~~



**字符串处理**

~~~sql
lower -- 将字符串转小写
	lower('hEllo,woRLD')

upper -- 将字符串转大写
	upper('hEllo,woRLD'),

initcap -- 将首字母变成大写
	initcap('hEllo,woRLD')

concat -- 字符串拼接
	concat('hello','world')
	-- 只支持两个参数如果需要使用多个，通过返回值继续拼接
	concat(concat('hello','world'),'123')

substr -- 提取子串
	substr('helloworld',3)  --从第三个字符开始一直提取到最后
	substr('helloworld',3,5) --从第三个字符开始取5个字符

instr -- 查找某个字符串是否另一个字符串的子串
	select instr('hello world', 'llo')
		是的话返回：3，首字符出现的位置
		不是的话返回：0

lpad,rpad -- 左右填充
	lpad('hello','10','*') from dual;
	-- 向左填充，填充后字符串的长度为10

trim -- 裁剪
	trim ( '  Hello world  ')    --默认裁剪空白字符
	trim ( 'H' from 'Hello worldHHHH') --裁剪掉H字符

replace -- 替换子串
	replace('Hello world','l','*')
		第二个参数：被替换的函数
		第三个参数：替换的字符串    
~~~





**数值处理函数**

~~~sql
round --四舍五入
	round(45.926,2)    -- 结果：45.93

trunc --截取
	trunc(45.926,2)  -- 结果：45.92

ceil  --向上取整
	ceil(45.926),    -- 结果：46

floor --向下取整
	floor(45.926),   -- 结果：45

mod  --取模 %
	mod(1600,1000)  -- 结果：600
~~~





 

**转换函数**

~~~sql
to_char     --数字转字符串

to_number  --字符串转数字

to_char     --日期转字符串

to_date     --字符串转日期 
~~~



**日期函数**

~~~sql
sysdate     --获取当前系统时间
	select sysdate from dual ;  -- oracle使用不加括号
	SELECT SYSDATE();   -- mysql使用加括号

months_between --计算两个时间相差的月份，自动计算大小月
	months_between(sysdate,sysdate)

last_day     --获取当前月份的最后一天
	last_day(sysdate)
~~~



  

**通用函数（oracle，特有函数）**

~~~sql
nvl(字段,val)
	-- 如果字段为null，那么返回val，否则返回字段的值

nvl2(字段,val1,val2)
	-- 如果字段为null，返回val2 ，否则返回val1
~~~



 

 

### 表的实现

**创建表：**

~~~sql
create table 表名
(
	列名1 类型1,
	列名2 类型2,
	......
)
~~~



**删除表：**

~~~sql
drop table 表名
~~~





**约束：**

~~~sql
-- 主键 
primary key

-- 非空 
not null

-- 唯一 
unique 

-- 检查 
check

-- 外键
foreign key
~~~





 

**约束使用示例：**

~~~sql
create table student 
(
	sid number constraint pk_student primary key, --学生Id主键约束
	
    --字段名 字段类型 constraint 约束名 约束关键字
	
    sname varchar2(20) constraint nn_student_name not null,--学生姓名非空约束
	
    email varchar2(20) constraint un_student_email unique --学生邮件唯一约束
	
    constraint nn_student_email not null, --同时邮件可再设非空，没有“，”

	age number constraint chk_student_age_min check(age > 10),   --学生年龄设置check约束

	gender varchar2(6) constraint chk_student_gender check(gender in ('男', '女')),

	deptno number constraint fk_student references dept (deptno) ON DELETE SET NULL

)  
~~~





 

 

### 其他知识点 

#### 事务

**说明：**

>- 事务是一个不可分割的单元
>
>- 多条SQL语句需要一起执行时，打开事务。如发生问题回滚，没有问题就提交。



**事务的提交：**

>- 显示提交：commit
>
>- 隐示提交
>
> - 使用创表语句create table，会隐式提交事务
>
> - 正常退出，exit/quit
>
>- 显示回滚：rollback
>
>- 隐式回滚：掉电、宕机、非正常退出

   

#### 视图

**说明：**

> 视图就是创建一个虚表，这个表的内容就是一条select语句，每次查看这个虚表其实就是执行这个select语句

 

**视图的创建：**    

~~~sql
create view 视图名as 子查询
~~~



**视图的删除：**

~~~sql 
drop view 视图名
~~~



#### 序列

**说明：**

> 序列就是一个自动增长种子

 

**创建序列：**

~~~sql
 create sequence 序列名;
~~~



**使用序列：**

> - 查看序列当前的值：
>
>   ~~~sql
>   select 序列名.currval from dual;
>   ~~~
>
> - 插入数据时使用：
>
>   ~~~sql
>   ---字段值填写：
>   序列名.nextval
>   ~~~

​      

 

#### 索引

**说明：**

> 索引就是对表的某一列或者多列进行排序，下次查询表的时候先查询索引，然后再取数据，设置多个列为索引，只有第一个列是有序的

 

**创建一个索引**

~~~sql
create index 索引名on 表名(字段名)
~~~



**删除索引**

~~~sql
drop index 索引名
~~~

 

#### 同义词

**说明：**

> 有的表名太长不方便使用，可以创建一个同义词使用，类似于别名

 

**创建同义词：**

~~~sql
create synonym 同义词名 for 用户名.表名;
~~~



**删除同义词：**

~~~sql
drop synonym 同义词 
~~~





## MYSQL

### 安装

[src\SQL\MYSQL不同版本第一次使用.docx](src/SQL/MYSQL不同版本第一次使用.docx)

 

 

### 基本操作

**数据库启动关闭**

~~~shell
service mysqld stop
service mysqld start
~~~



**登陆**

~~~shell
mysql -u用户名 -p
~~~



**查看数据库**

~~~mysql
show databases
~~~

 

**选择使用哪个数据库**

~~~mysql
use 数据库名
~~~



**查看表、结构、创表语句**

~~~mysql
show tables;  --查看表
desc 表名   --查看表结构
show create table 表名       --查看创表语句
~~~





### 数据库实现

**约束：**

~~~mysql
-- 唯一 
unique

-- 非空 
not null

-- 主键
primary key	-- 自带非空和唯一约束，一个表只能有一个主键

-- 外键 
foreign key -- 参照其他的列，只能存入参照列中存在的数据

-- 检查约束
check -- mysql没有check需要其他方法实现，枚举实现
~~~



**实现：**

~~~mysql
drop database if exists admin_gl;
set names utf8;
create database if not exists admin_gl default charset utf8;
use admin_gl;

create table jsy(
	jsy_id bigint unsigned primary key,
	jsy_name char(50) not null,
	sex char(1) not null  default '男' check(sex in('男','女')),
	birthdate date not null,
	instructor_id bigint unsigned,
	foreign key(instructor_id) references jsy(jsy_id)
);

INSERT INTO jsy (jsy_id,jsy_name,sex,birthdate,instructor_id)VALUES
(1,'文侯','男',"1994-11-10",NULL),
(2,'武侯','男',"1994-11-10",NULL),
(3,'枫','女',"1994-11-10",NULL),
(4,'沈西平',default,"1994-11-10",2),
(5,'陆经鱼','和',"1994-11-10",2),
(6,'毕炜','男',"1994-11-10",1),
(7,'邓沧澜','男',"1994-11-10",1),
(8,'邵风观','男',"1994-11-10",1),
(9,'茵郡主','女',"1994-11-10",null),
(10,'南武公子','男',"1994-11-10",NULL),
(11,'丁亨利','男',"1994-11-10",5),
(12,'楚休红','男',"1994-11-10",2),
(13,'武昭','男',"1994-11-10",NULL),
(14,'小王子','男',"1994-11-10",13),
(15,'苏纹月','女',"1994-11-10",NULL),
(16,'路恭行','男',"1994-11-10",null),
(17,'钱文义','男',"1994-11-10",12),
(18,'陈忠','男',"1994-11-10",12),
(19,'柳风舞','男',"1994-11-10",12);


create table xm(
	xm_id bigint unsigned primary key,
	xm_name char(50) not null,
	startdate datetime not null,
	enddate datetime
);


INSERT INTO xm(xm_id,xm_name,startdate,enddate)VALUES
(1,'高？城守卫',"1994-11-10","1994-11-10"),
(2,'保卫帝都',"1994-11-10","1994-11-10"),
(3,'投降共和',"1994-11-10","1994-11-10");

create table zjb(
	xm_id bigint unsigned not null,
	jsy_id bigint unsigned not null,
	foreign key (xm_id) references xm(xm_id),
	foreign key (jsy_id) references jsy(jsy_id)
);

INSERT INTO zjb(xm_id,jsy_id)VALUES
(1,1),
(1,2),
(1,3),
(1,4),
(1,5),
(1,12),
(1,15),
(1,16),
(1,17),
(2,1),
(2,3),
(2,6),
(2,7),
(2,8),
(2,9),
(2,12),
(2,13),
(2,14),
(2,16),
(2,17),
(2,18),
(3,3),
(3,6),
(3,7),
(3,8),
(3,10),
(3,11),
(3,12),
(3,14),
(3,17),
(3,18);

set names gbk;
-- 1、	指定一名技术人员，可查询直接指导他（她）的技术人员的信息；
SELECT b.*
FROM jsy as a,jsy as b
WHERE a.instructor_id=b.jsy_id
-- AND a.jsy_id=18，
and a.jsy_name='陈忠';

-- 2、	指定一名技术人员，可查询他（她）直接指导的其它技术人员的信息；
SELECT a.*
FROM jsy as a,jsy as b
WHERE a.instructor_id=b.jsy_id;
-- AND b.jsy_name='楚休红';

-- 3、	指定一名技术人员，可查询他（她）所参与的所有项目的信息；
SELECT xm_name '(姓名)'
FROM jsy,zjb,xm
WHERE jsy.jsy_id=1
and xm.xm_id=zjb.xm_id
and jsy.jsy_id=zjb.jsy_id;

-- 4、	指定一个项目，可查询参与此项目的所有技术人员信息。
SELECT jsy_name as 姓名
FROM jsy,zjb,xm
WHERE xm.xm_id =1
and xm.xm_id=zjb.xm_id
and jsy.jsy_id=zjb.jsy_id;
~~~

[src\SQL\MYSQL数据库实现\数据字典表格.xlsx](src/SQL/MYSQL数据库实现/数据字典表格.xlsx)

[src\SQL\MYSQL数据库实现\er.png](src/SQL/MYSQL数据库实现/er.png)

 

 

### mysql，C语言接口

**代码示例**

~~~c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mysql/mysql.h>
#define _DB_IP_	"127.0.0.1"	//IP地址 
#define _DB_USER_ "root"	//数据库账号	
#define _DB_PASSWD_ "go123"	//数据库密码
#define _DB_NAME_ "sjs"		//数据库名称
#define _DB_PORT_ 3306		//mysqld服务器端口


int main()
{
	int i;
	//初始化mysql结构体
	MYSQL* mysql = NULL;
	mysql = mysql_init(NULL);
	if (mysql == NULL)
	{
		perror("mysql_init");
		perror(mysql_error(mysql));
		exit(-1);
	}

	//建立连接
	if (mysql_real_connect(
		mysql,		//mysql结构体
		_DB_IP_,	//服务器IP
		_DB_USER_,	//登陆服务器的用户名
		_DB_PASSWD_,//登陆服务器的密码
		_DB_NAME_,	//使用哪个数据库
		_DB_PORT_,	//服务器使用端口
		NULL,		//如果服务器是以本地套接字部署的，这里填本地套接字地址
		0

	)==NULL)
	{
		perror("mysql_real_connect");
		perror(mysql_error(mysql));
		exit(-1);
	}

	//修改当前连接的字符集
	if (mysql_set_character_set(mysql,"UTF8") == -1)
	{
		//fprintf(stderr, 
		//	"Error in mysql_set_character_set() :%s\n", 
		//	mysql_error(mysql));
		perror("mysql_set_character_set()");
		perror(mysql_error(mysql));
		exit(-1);
	}


	//业务处理
	while (1)
	{
		//保存sql语句
		char sql[1024] = { 0 };
		memset(sql,0,sizeof(sql));

		//提示输入
		write(1,"mysql_lr:",sizeof("mysql_lr:"));
		//读取用户输入
		read(0,sql,sizeof(sql));
		//用户输入quit 和 exit  退出程序
		if (strcmp(sql,"quit\n")==0  || strcmp(sql,"exit\n") ==0)
		{
			write(1,"Bye!\n");
			exit(-1);
		}
		//执行sql语句
		if (mysql_query(mysql, sql)==0 )
		{
			//打印受影响的行，
			//my_ulonglong affrow = 
			//	mysql_affected_rows(mysql);
			//printf("影响了%ld行。\n",affrow);

			printf("SQL语句执行成功\n" );

			//执行sql语句后，获取结果集
			MYSQL_RES* res = mysql_store_result(mysql);
			if (res != NULL)
			{
				//获取当前结果集的列
				unsigned int num_fields = mysql_num_fields(res);

				//获取结果集的表头
				MYSQL_FIELD* fields = mysql_fetch_fields(res);
				for (i = 0; i < num_fields; i++)
					printf("%s\t", fields[i].name);
				printf("\n--------------------\
					--------------------------------\n");


				//保存结果集的每一行，其实就是个char二级指针
				MYSQL_ROW row = NULL;
				//获取结果集合的每一行
				while (row = mysql_fetch_row(res))
				{
					for (i = 0; i < num_fields; i++)
						printf("%s\t", row[i]);
					printf("\n");

				}
		}
		else
		{
				//sql语句有误
				printf("sql语句有误!\n%s\n",
					mysql_error(mysql));

			}
			//释放结果集
			mysql_free_result(res);
		}

	}

	//释放 mysql 结构体
	mysql_close(mysql);
	exit(0);
}
~~~

 

**中文文档**

[src\SQL\MYSQL_C语言_API\MySQL 中文完全参考手册5.1.chm](src/SQL/MYSQL_C语言_API/MySQL 中文完全参考手册5.1.chm)

 

**函数**

~~~c
//头文件：
#include<mysql/mysql.h>

//初始化mysql结构体
MYSQL *mysql_init(MYSQL *mysql) 
参数：
	mysql：传NULL, 如果mysql是NULL指针，该函数将分配、初始化、并返回新对象。
返回值：
	成功返回：mysql指针
	失败返回：在内存不足的情况下，返回NULL。

//释放mysql结构体资源
void mysql_close(MYSQL *mysql) 

//建立连接
MYSQL *mysql_real_connect(MYSQL *mysql,
			const char *host, 
			const char *user, 
			const char *passwd, 
			const char *db, 
			unsigned int port, 
			const char *unix_socket, 
			unsigned long client_flag)
参数：
	mysql：分配好资源的mysql结构体
	host：服务器的IP
	user：访问服务器的用户名
	passwd：访问服务器的密码
	db：需要访问的数据库
	port：服务器的端口，默认3306
	unix_socket：传NULL
	client_flag：传0
返回值：
	成功返回：mysql指针
	失败返回：NULL。

//设置当前连接的字符集
int mysql_set_character_set(MYSQL *mysql, char *csname) 
参数：
	mysql：mysql指针
	csname：要是用的字符集，如 "UTF8"
返回值：
	成功返回：0
	失败返回：非0值


//执行SQL语句
int mysql_query(MYSQL *mysql, const char *query)
函数功能：执行以 \0 为结尾的sql语句，不要以 ; 或者 \g为结尾
参数：
	mysql：mysql指针
	query：sql语句
返回值：
	成功返回：0
	失败返回：
		CR_COMMANDS_OUT_OF_SYNC
			以不恰当的顺序执行了命令。
		CR_SERVER_GONE_ERROR
			MySQL服务器不可用。 
		CR_SERVER_LOST
			在查询过程中，与服务器的连接丢失。 
		CR_UNKNOWN_ERROR
			出现未知错误。 
注意：
	如果sql语句中包含一些二进制数据（可能包含\0字节数据) ，使用mysql_real_query来执行

//执行SQL语句后获取结果集合
MYSQL_RES *mysql_store_result(MYSQL *mysql)
参数：
	mysql：mysql指针
返回值：
	成功返回：具有多个结果的MYSQL_RES结果集合。
	失败返回：NULL，一些宏看API

//释放结果集
void mysql_free_result(MYSQL_RES *result) 
参数：
	result：获取到的结果集

//获取结果集合有几列
unsigned int mysql_num_fields(MYSQL_RES *result)
参数：
	result：获取到的结果集
返回值：
	成功返回：结果集中的列数
	失败返回：无

//获取结果集合中的表头
MYSQL_FIELD *mysql_fetch_fields(MYSQL_RES *result)
参数：
	result：获取到的结果集	
返回值：
	成功返回：对于结果集，返回所有MYSQL_FIELD结构的数组。每个结构提供了结果集中1列的字段定义。
	失败返回：无
注意：
	fields[i].name 通过结果集点name的方式获取表头

//获取结果集合中的每一行数据
MYSQL_ROW mysql_fetch_row(MYSQL_RES *result)
参数：
	result：获取到的结果集	
返回值：
	下一行的MYSQL_ROW结构。如果没有更多要检索的行或出现了错误，返回NULL。

//获取受影响的行
my_ulonglong mysql_affected_rows(MYSQL *mysql)
参数：
	mysql：mysql指针
返回值：
 

//C语言打开事物的接口：
my_bool mysql_autocommit(MYSQL *mysql, my_bool mode)
函数功能：打开和关闭mysql事物
参数：
	mysql：mysql句柄
	mode：传0打开事物，传1关闭事物
返回值：
	成功返回：0
	失败返回：非0值
~~~



 

### mysql中文乱码问题

**1、连接服务器查询编码集**

~~~mysql
show variables like 'character%'; 
~~~



**2、查询结果解析**

~~~mysql
-- 主要用来设置客户端使用的字符集。
character_set_client

-- 主要用来设置连接数据库时的字符集，如果程序中没有指明连接数据库使用的字符集类型则按照这个字符集设置。
character_set_connection

-- 主要用来设置默认创建数据库的编码格式，如果在创建数据库时没有设置编码格式，就按照这个格式设置。
character_set_database


-- 文件系统的编码格式，把操作系统上的文件名转化成此字符集，即把 character_set_client转换character_set_filesystem， 默认binary是不做任何转换的。
character_set_filesystem   -- 不用关心

-- 数据库给客户端返回时使用的编码格式，如果没有指明，使用服务器默认的编码格式。
character_set_results

-- 服务器安装时指定的默认编码格式，这个变量建议由系统自己管理，不要人为定义。
character_set_server

 
-- 数据库系统使用的编码格式，这个值一直是utf8，不需要设置，它是为存储系统元数据的编码格式。
character_set_system  -- 不用关心

　　    
-- 这个变量是字符集安装的目录。
character_sets_dir -- 不用关心

　　    
~~~





 

**3、字符集装换**

>  一、mysql Server收到请求时将请求数据从 character_set_client 转换为 character_set_connection
>
>  二、进行内部操作前将请求数据从 character_set_connection 转换为内部操作字符集,步骤如下 
>
>​      A. 使用每个数据字段的 CHARACTER SET 设定值； 
>
>　　    B. 若上述值不存在，则使用对应数据表的字符集设定值 
>
>　　    C. 若上述值不存在，则使用对应数据库的字符集设定值； 
>
>　　    D. 若上述值不存在，则使用 character_set_server 设定值。
>
>  三、最后将操作结果从内部操作字符集转换为 character_set_results



 

**4、LINUX的编码集问题**

统一设置为中文UTF-8编码

~~~shell
export LC_ALL="zh_CN.UTF-8"  
~~~



**5、终端软件编码集问题**

> 如Xshell、CMD、等等

 

 

### 其他知识点 

#### 事物

**说明：**

> mysql默认不打开事物，每执行一条数据都会自动提交相当于没有事物

 

**mysql客服端查看事物是否打开:**

~~~mysql
select @@autocommit; -- 0为事物打开。1 为事物关闭
~~~



**mysql客服端打开事物:**

~~~mysql
set autocommit=0; --设置为1，关闭事物
~~~



**事物的提交和回滚：**

~~~mysql
ROLLBACK -- 回滚
COMMIT -- 提交
~~~

 

#### 预处理



#### 存储过程





