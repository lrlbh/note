#include<iostream>
using namespace std;
#include<vector>
#include<algorithm>
#include<numeric>
/*
accumulate算法 计算容器元素累计总和
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param value累加值
	accumulate(iterator beg, iterator end, value)
*/
void test01()
{
	//初始化容器
	vector<int> v1;
	for (int i = 0; i < 10; i++)
		v1.push_back(i);
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; }); cout << endl;

	//accumulate算法 计算容器元素累计总和
	cout << accumulate(v1.begin(), v1.end(), 1000) << endl;;
	//int sum = accumulate(v1.begin(), v1.end(), 100);
	//cout << sum << endl;
}

/*
fill算法 向容器中添加元素
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param value t填充元素
	fill(iterator beg, iterator end, value)
*/
void test02()
{
	//初始化容器
	vector<int> v1;
	for (int i = 0; i < 10; i++)
		v1.push_back(i);
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; });
	cout << endl;

	//fill算法 向容器中添加元素
	fill(v1.begin(),v1.end(),12);
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; });
	cout << endl;
}

int main()
{
	test02();

	system("pause");
	return 0;
}