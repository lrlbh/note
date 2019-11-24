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