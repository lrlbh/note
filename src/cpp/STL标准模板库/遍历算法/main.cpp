#include<iostream>
using namespace std;
#include<string>
#include<vector>
#include<algorithm>

/*
遍历算法 遍历容器元素
	@param beg 开始迭代器
	@param end 结束迭代器
	@param _callback  函数回调或者函数对象
	@return 函数对象
	for_each(iterator beg, iterator end, _callback);
*/
class MyPrint
{
public:
	void operator()(int v)
	{
		//for_each对单个元素的操作写在这里
		cout<< v <<endl;
	}
};

void test01()
{
	vector<int> v;
	for (int i = 0; i < 10; i++)
		v.push_back(i);

	for_each(v.begin(),v.end(), MyPrint());
}


/*
transform算法 将指定容器区间元素搬运到另一容器中
注意 : transform 不会给目标容器分配内存，所以需要我们提前分配好内存
	@param beg1 源容器开始迭代器
	@param end1 源容器结束迭代器
	@param beg2 目标容器开始迭代器
	@param _cakkback 回调函数或者函数对象
	@return 返回目标容器迭代器
	for_each(iterator beg1, iterator end1, iterator beg2, _callbakc)
*/
class MyTransForm
{
public:
	int operator()(int v)
	{
		//for_each对单个元素的自定义赋值操作
		return v +100;
	}

};
void test02()
{
	vector<int> v;
	for (int i = 0; i < 10; i++)
		v.push_back(i);

	vector<int> v1;
	//这里不知道为什么要指定长度，不可以指定大小
	v1.resize(v.size());
	//transform算法 将指定容器区间元素搬运到另一容器中
	transform(v.begin(),v.end(),v1.begin(),MyTransForm());
	//遍历输出
	for_each(v1.begin(), v1.end(), MyPrint());
}




int main()
{
	test02();
	system("pause");
	return 0;
}