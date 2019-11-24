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