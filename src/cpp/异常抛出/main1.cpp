#include<iostream>
#include<string>
using namespace std;

//继承c++错误父类
class MyErr :public exception
{
public:
	MyErr()
	{
		err = "没有错误信息！";
	}

	//构造创建对象
	MyErr(const char * err)
	{
		this->err = err;
	}
	//构造创建对象
	MyErr(string err)
	{
		this->err = err;
	}
	//重写父类虚拟函数打印返回错误信息
	virtual const char * what() const
	{
		return this->err.c_str();
	}

	virtual ~MyErr()
	{
	}

private:
	string err;

};

//抛出错误的函数
void myErr()
{
	//抛出错误对象
	throw MyErr();
}

int main()
{
	try
	{
		myErr();

	}
	catch (exception & e)//通过多态，父类接收子类错误对象
	{
		cout << e.what() << endl;
	}
	catch (...)
	{
		cout << "其他错误信息" << endl;
	}

	system("pause");
	return 0;
}