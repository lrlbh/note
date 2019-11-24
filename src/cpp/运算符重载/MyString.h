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

