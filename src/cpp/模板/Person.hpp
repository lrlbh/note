#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;

template<class NAME, class AGE = int>
class Person
{
public:
	//Person()
	//{
	//	cout << "默认" << endl;
	//}
	Person(NAME name, AGE age);
	void showPerson();


	NAME name;
	AGE age;

};

template<class NAME, class AGE>
Person<NAME,AGE>::Person(NAME name, AGE age)
{
	cout << "有参" << endl;
	this->age = age;
	this->name = name;
}

template<class NAME, class AGE>
void Person<NAME, AGE>::showPerson()
{
	cout << "姓名：" << this->name << "  " << "年龄：" << this->age << endl;
}