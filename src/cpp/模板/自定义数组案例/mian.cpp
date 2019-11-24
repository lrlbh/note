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