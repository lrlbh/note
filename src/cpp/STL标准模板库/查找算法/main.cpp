#include<iostream>
using namespace std;
#include<vector>
#include<algorithm>

/*
find算法 查找元素
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param value 查找的元素
	@return 返回第一个找到元素的迭代器
	find(iterator beg, iterator end, value)
*/
void test01()
{
	//初始化容器
	vector<int> v;
	for (int i = 0; i < 10; i++)
		v.push_back(i);
	v.push_back(4);
	v.push_back(4);
	for_each(v.begin(), v.end(), [](int a) {cout << a << " "; });
	cout << endl;

	//find算法的使用
	//cout << *(find(v.begin(), v.end(), 4)) << endl;
	for (vector<int>::iterator ret = find(v.begin(), v.end(), 4);ret!=v.end();)
	{
		cout << *ret << endl;
		ret++;
		ret = find(ret, v.end(), 4);
	}
}

/*
find_if算法 条件查找
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param  callback 回调函数或者谓词(返回bool类型的函数对象)
	@return 返回第一个找的元素的迭代器
	find_if(iterator beg, iterator end, _callback);
*/
bool myFindIf(int a)
{
	//find_if写条件的地方
	return a > 1;
}
void test02()
{
	//初始化容器
	vector<int> v;
	for (int i = 0; i < 10; i++)
		v.push_back(i);
	for_each(v.begin(), v.end(), [](int a) {cout << a << " "; });
	cout << endl;

	//find_if条件查找
	cout << *(find_if(v.begin(), v.end(), myFindIf)) << endl;;
}

/*
	adjacent_find算法 查找相邻重复元素
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@return 返回相邻元素的第一个位置的迭代器
	adjacent_find(iterator beg, iterator end);
*/
void test03()
{
	//初始化容器
	vector<int> v;
	for (int i = 0; i < 10; i++)
		v.push_back(i);
	v.push_back(9);
	v.push_back(2);
	for_each(v.begin(), v.end(), [](int a) {cout << a << " "; });
	cout << endl;

	//adjacent_find算法，查找相邻的重复元素
	cout << *(adjacent_find(v.begin(),v.end())++) << endl;
}

/*
	binary_search算法 二分查找法
	注意: 在无序序列中不可用
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param value 查找的元素
	@return bool 查找返回true 否则false
	bool binary_search(iterator beg, iterator end, value);
*/
void test04()
{
	//初始化容器
	vector<int> v;
	for (int i = 0; i < 10; i++)
		v.push_back(i);
	for_each(v.begin(), v.end(), [](int a) {cout << a << " "; });
	cout << endl;

	//二分查找，返回bool，只可以用于有序容器
	cout << binary_search(v.begin(), v.end(), 9) << endl;
	cout << binary_search(v.begin(), v.end(), 10) << endl;;
}

/*
	count算法 统计元素出现次数
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@return int返回元素个数
*/
void test05()
{
	//初始化容器
	vector<int> v;
	for (int i = 0; i < 10; i++)
		v.push_back(i);
	v.push_back(4);
	v.push_back(4);
	v.push_back(4);
	for_each(v.begin(), v.end(), [](int a) {cout << a << " "; });
	cout << endl;

	//cout算法查找某个元素出现的次数，返回int
	cout << count(v.begin(),v.end(),4) << endl;
}

/*
count_if算法 统计元素出现次数
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param  callback 回调函数或者谓词(返回bool类型的函数对象)
	@return int返回元素个数
	count_if(iterator beg, iterator end, _callback);
*/
class MyCountIf
{
public:
	MyCountIf() 
	{
		this->temp = -1;
	}
	MyCountIf(int a)
	{
		this->temp = a;
	}
	
	//可以有多个参数
	int temp;

	bool operator()(int value)
	{
		//写条件的地方
		return value > this->temp;
	}

};
void test06()
{
	//初始化容器
	vector<int> v;
	for (int i = 0; i < 10; i++)
		v.push_back(i);

	for_each(v.begin(), v.end(), [](int a) {cout << a << " "; });
	cout << endl;

	//count_if算法，条件统计
	cout<<count_if(v.begin(),v.end(), MyCountIf(1))<<endl;

}





int main()
{
	test06();
	system("pause");
	return 0;
}