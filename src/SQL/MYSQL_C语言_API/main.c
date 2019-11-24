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