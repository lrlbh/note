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