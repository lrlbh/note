#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include<string>

class AA
{
public:
	//加号运算符重载
	AA operator+(AA &aa)
	{
		AA temp; 
		temp.a =  this->a + aa.a;
		strcat(temp.b,this->b);
		strcat(temp.b,aa.b);
		temp.c = this->c + aa.c;

		return temp;
	}

	//减号运算符重载
	AA operator-(AA &aa)
	{
		AA temp;
		temp.a = this->a = aa.a;
		return temp;
	}

	//比较运算符重载
	bool operator==(AA &aa)
	{
		if (this->a==aa.a && this->c==aa.c && (strcmp(this->b,aa.b)==0))
		{
			return true;
		}

		return false;
	}

	//前置++重载
	AA& operator++()
	{
		this->a++;
		return *this;
	}

	//后置++重载
	//返回值加const 是为了防止连续++ 后置++ 无法连续++
	//但是这样返回值就是一个常对象，常对象只能调用常函数
	const AA operator++(int)
	{
		AA temp(*this);
		//temp.a = this->a;
		this->a++;
		return temp;
	}

	//赋值运算符重载
	AA& operator=(const AA & aa)
	{
		cout << "operator=()调用" << endl;
		this->a = aa.a;
		strcpy(this->b,aa.b);
		this->c = aa.c;
		return *this;
	}

	//void abc(AA &a,AA &b)
	//{
	//	AA temp;
	//	temp.a = a.a + b.a;
	//	
	//}

	AA()
	{
		cout << "无参构造调用" << endl;
		this->a = 0;
		this->b = (char*)malloc(128);
		memset(this->b,0x0,128);
	}

	AA(int a,const char *b, const string c)
	{
		cout << "有参构造调用" << endl;
		this->b = (char*)malloc(128);
		memset(this->b, 0x0, 128);
		this->a = a;
		strcpy(this->b,b);
		this->c = c;
	}

	AA(AA &aa)
	{
		cout << "拷贝参构造调用" << endl;
		this->b = (char*)malloc(128);
		memset(this->b, 0x0, 128);
		this->a = aa.a;	
		strcpy(this->b,aa.b);
		this->c = aa.c;
	}

	~AA()
	{
		cout << "析构调用" << endl;
		this->a = 0;
		free(this->b);
		c = "\0";
	}
	
	 int getA()const
	{
		return this->a;
	}

	char* getB()
	{
		return this->b;
	}

	string getC()
	{
		return this->c;
	}
private:
	int a;
	char *b;
	string c;
};



int main()
{
	
	AA a1(12,"char","string");
	(a1++);
	cout << a1.getA() << endl;
	cout << (a1++).getA() << endl;
	cout << (++a1).getA() << endl;
	++(++(++a1));
	cout << a1.getA() << endl;
	//a1.getA();
	AA a2 = a1;//拷贝构造的调用
	AA a3 (a2);
	AA a4(13,"char1","char2");
	a3 = a2 + a1 + a1; 
	a1 = a2 = a3 = a4;
	cout << a3.getA() <<"  "<<"char="<< a3.getB() << "  "<<"string=="<<a3.getC() << endl;

	if (a1==a2)
	{
		cout << "a1==a2" << endl;
		
	}
	else
	{
		cout << "a1!=a2" << endl;
	}
	system("pause");
	return 0;
}