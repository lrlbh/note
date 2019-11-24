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


