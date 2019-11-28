#include<iostream>
using namespace std;
#include<vector>
#include<algorithm>

/*
set_intersection算法 求两个set集合的交集
	注意:两个集合必须是有序序列
	@param beg1 容器1开始迭代器
	@param end1 容器1结束迭代器
	@param beg2 容器2开始迭代器
	@param end2 容器2结束迭代器
	@param dest  目标容器开始迭代器
	@return 目标容器的最后一个元素的迭代器地址
*/

/*
set_union算法 求两个set集合的并集
	注意:两个集合必须是有序序列
	@param beg1 容器1开始迭代器
	@param end1 容器1结束迭代器
	@param beg2 容器2开始迭代器
	@param end2 容器2结束迭代器
	@param dest  目标容器开始迭代器
	@return 目标容器的最后一个元素的迭代器地址
*/

/*
set_difference算法 求两个set集合的差集
	注意:两个集合必须是有序序列
	@param beg1 容器1开始迭代器
	@param end1 容器1结束迭代器
	@param beg2 容器2开始迭代器
	@param end2 容器2结束迭代器
	@param dest  目标容器开始迭代器
	@return 目标容器的最后一个元素的迭代器地址
*/


void test01()
{
	//初始化容器
	vector<int> v1;
	vector<int> v2;
	for (int i = 0; i < 10; i++)
		v1.push_back(i);
	for (int i = 5; i < 15; i++)
		v2.push_back(i);
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; }); cout << endl;
	for_each(v2.begin(), v2.end(), [](int a) {cout << a << " "; }); cout << endl;
	vector<int> v3;

	//set_intersection算法 求两个set集合的交集
	//交集，两个容器相同的元素
	v3.resize(max(v1.size(),v2.size()));
	set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin());
	for_each(v3.begin(), v3.end(), [](int a) {cout << a << " "; }); cout << endl;

	//set_union算法 求两个set集合的并集
	//两个容器二合一，所有不同的元素
	v3.resize(v1.size()+ v2.size());
	set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin());
	for_each(v3.begin(), v3.end(), [](int a) {cout << a << " "; }); cout << endl;

	//set_difference算法 求两个set集合的差集
	//v1的差集，v1独有的元素
	vector<int> v4;
	v4.resize(v1.size());
	set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), v4.begin());
	for_each(v4.begin(), v4.end(), [](int a) {cout << a << " "; }); cout << endl;


	//set_difference算法 求两个set集合的差集
	//v2的差集，v2独有的元素
	v4.resize(v2.size());
	set_difference(v2.begin(), v2.end(), v1.begin(), v1.end(), v4.begin());
	for_each(v4.begin(), v4.end(), [](int a) {cout << a << " "; }); cout << endl;
}


int main()
{
	test01();
	system("pause");
	return 0;
}