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