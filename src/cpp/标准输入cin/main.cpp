#include<iostream>
using namespace std;

//读取一个字符cin.get()
void test()
{
	char ch = cin.get();
	cout << ch << endl;
}

//读取一个字符串cin.get(buf,sizeof(buf))
void test01()
{
	char buf[1024];
	//这种方式读取遇到换行符结束，同时换行符会留在缓冲区
	cin.get(buf,sizeof(buf));
	cout << buf << endl;
	cin.get();
}

//读取一个字符串cin.getline()
void test02()
{
	char buf[1024] = { 0 };
	//这种方式读取遇到换行符结束，换行符会被抛弃，不在缓冲区，也不在字符串
	cin.getline(buf, sizeof(buf));
	cout << buf << endl;
}

//忽略一个或多个字符cin.ignore()
void test03()
{
	char buf[1024] = { 0 };
	//忽略字符，小括号填多少就忽略多少个字符
	cin.ignore();
	cin.get(buf, sizeof(buf));
	cout << buf << endl;
}

//偷窥一个字符 cin.peek()
void test04()
{
	char buf[1024] = {0};
	//窥探
	char ch = cin.peek();
	cout << ch << endl;
	cin.get(buf, sizeof(buf));
	cout << buf << endl;

}

//放回一个字符，只能放回一个 cin.putback()
void test05()
{
	//读取出来后在放回去
	char ch= cin.get();
	cout << ch << endl;
	cin.putback(ch);

	char buf[1024] = { 0 };
	cin.get(buf, sizeof(buf));
	cout << buf << endl;
}

int main()
{
	//test();
	//test01();
	//test02();
	//test03();
	test05();
	system("pause");
	return 0;
}


//案例2  让用户输入 1 ~ 10之间的数字，如果输入有误，重新输入
void test08()
{
	int num;

	while (true)
	{
		cout << "请输入1~10之间的数字： " << endl;
		//cin>>num;读取折标会设置标志位为1  然后向下走
		cin >> num;
		if (num > 0 && num < 10)
		{
			cout << "输入正确-值为：" << num << endl;
			break; 
		}

		//重置标志位
		cin.clear();
		//清空缓冲区   2013可用 2015以上 cin.ignore()/ cin.getline();
		cin.sync();

		//缓冲区中的标志位  0正常   1异常
		cout << "标志位： " << cin.fail() << endl;

		//cout << "输入有误请重新输入" << endl;
	}
}






