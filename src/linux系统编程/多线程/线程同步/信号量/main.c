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