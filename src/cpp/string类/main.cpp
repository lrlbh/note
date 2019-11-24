#include<iostream>
#include<string>
#include<vector>
using namespace std;

/*
构造函数：
	string();//创建一个空的字符串 例如: string str;
	string(const string& str);//使用一个string对象初始化另一个string对象
	string(const char* s);//使用字符串s初始化
	string(int n, char c);//使用n个字符c初始化
*/

/*
基本赋值操作：
	string& operator=(const char* s);//char*类型字符串 赋值给当前的字符串
	string& operator=(const string &s);//把字符串s赋给当前的字符串
	string& operator=(char c);//字符赋值给当前的字符串
	string& assign(const char *s);//把字符串s赋给当前的字符串
	string& assign(const char *s, int n);//把字符串s的前n个字符赋给当前的字符串
	string& assign(const string &s);//把字符串s赋给当前字符串
	string& assign(int n, char c);//用n个字符c赋给当前字符串
	string& assign(const string &s, int start, int n);//将s从start开始n个字符赋值给字符串
*/

/*
基本字符操作：
	char& operator[](int n);//通过[]方式取字符
	char& at(int n);//通过at方法获取字符
*/


/*
 string拼接操作：
	string& operator+=(const string& str);//重载+=操作符
	string& operator+=(const char* str);//重载+=操作符
	string& operator+=(const char c);//重载+=操作符
	string& append(const char *s);//把字符串s连接到当前字符串结尾
	string& append(const char *s, int n);//把字符串s的前n个字符连接到当前字符串结尾
	string& append(const string &s);//同operator+=()
	string& append(const string &s, int pos, int n);//把字符串s中从pos开始的n个字符连接到当前字符串结尾
	string& append(int n, char c);//在当前字符串结尾添加n个字符c
*/

/*
string比较操作：
	compare函数在>时返回 1，<时返回 -1，==时返回 0。
	比较区分大小写，比较时参考字典顺序，排越前面的越小。
	大写的A比小写的a小。
	int compare(const string &s) const;//与字符串s比较
	int compare(const char *s) const;//与字符串s比较
*/

/*
string查找和替换：没找到返回-1
	int find(const string& str, int pos = 0) const; //查找str第一次出现位置,从pos开始查找
	int find(const char* s, int pos = 0) const;  //查找s第一次出现位置,从pos开始查找
	int find(const char* s, int pos, int n) const;  //从pos位置查找s的前n个字符第一次位置
	int find(const char c, int pos = 0) const;  //查找字符c第一次出现位置
	int rfind(const string& str, int pos = npos) const;//查找str最后一次位置,从pos开始查找
	int rfind(const char* s, int pos = npos) const;//查找s最后一次出现位置,从pos开始查找
	int rfind(const char* s, int pos, int n) const;//从pos查找s的前n个字符最后一次位置
	int rfind(const char c, int pos = 0) const; //查找字符c最后一次出现位置
	string& replace(int pos, int n, const string& str); //替换从pos开始n个字符为字符串str
	string& replace(int pos, int n, const char* s); //替换从pos开始的n个字符为字符串s
*/

/*
string子串：
	string substr(int pos = 0, int n = npos) const;//返回由pos开始的n个字符组成的字符串
*/

/*
string插入和删除操作：
	string& insert(int pos, const char* s); //插入字符串
	string& insert(int pos, const string& str); //插入字符串
	string& insert(int pos, int n, char c);//在指定位置插入n个字符c
	string& erase(int pos, int n = npos);//删除从Pos开始的n个字符
*/

/*
string和c-style字符串转换：
	string 转 char*
	string str = "itcast";
	const char* cstr = str.c_str();
	char* 转 string
	char* s = "itcast";
	string str(s);
*/

//查找
void test01()
{
	//查找子字符串第一次出现的位置，没找到返回-1
	string str1 = "hello world";
	int ret= str1.find("loo");
	cout <<ret<< endl;

	//查找所有子字符串出现的位置
	ret = 0;
	while ((ret = str1.find("l",ret)) != -1)
	{
		cout << ret << endl;
		ret++;
	}
}

//返回一个子字符串
void test02()
{
	string str1 = "hello world";
	cout << str1.substr(0, 5) << endl;
}

//string插入和删除字符
void test03()
{
	//插入字符
	string str1 = "hello world";
	str1.insert(5,"hehe");
	cout << str1 << endl;

	//删除字符
	str1.erase(5,4);
	cout << str1 << endl;
}

//大写转小写
void test04()
{
	string str1 = "ASFS425ADasdS//\\!DFSD1";
	for (int i = 0; i < str1.length(); i++)
	{
		if (str1[i] >='A' && str1[i] <= 'Z')
		{
			str1[i] += 32;
		}
	}
	cout << str1 << endl;
}

//提取子串
void test05()
{
	string str1 = "hehe:ha!ha:xix!ixi:la!lala:heihei:w!uwu";
	string str2[20];
	//提取到str2数组中
	for (int i = 0,ii=0; i < str1.length(); i++)
	{
		if (str1[i]==':'||str1[i]=='!')
		{
			ii++;
			continue;
		}
		str2[ii] += str1[i];
	}


	//遍历提取到字符串
	for (int i = 0; ; i++)
	{
		if (str2[i][0] == NULL)
			break;
		
		cout << str2[i] << endl;
	}
}

int main()
{
	test05();
	system("pause");
	return 0;
}