#include<iostream>
using namespace std;
#include<vector>
#include<algorithm>

/*
copy算法 将容器内指定范围的元素拷贝到另一容器中
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param dest 目标起始迭代器
	copy(iterator beg, iterator end, iterator dest) 
*/
void test01()
{
	//初始化容器
	vector<int> v1;
	for (int i = 0; i < 10; i++)
		v1.push_back(i);
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; }); cout << endl;

	vector<int> v2;
	v2.resize(v1.size());
	//copy算法 将容器内指定范围的元素拷贝到另一容器中
	copy(v1.begin(), v1.end(),v2.begin());
	for_each(v2.begin(), v2.end(), [](int a) {cout << a << " "; }); cout << endl;
}

/*
replace算法 将容器内指定范围的旧元素修改为新元素
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param oldvalue 旧元素
	@param newvalue 新元素
	replace(iterator beg, iterator end, oldvalue, newvalue)
*/
void test02()
{
	//初始化容器
	vector<int> v1;
	for (int i = 0; i < 10; i++)
		v1.push_back(i);
	v1.push_back(4);
	v1.push_back(4);
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; }); cout << endl;

	//replace算法 将容器内指定范围的旧元素修改为新元素
	replace(v1.begin(),v1.end(),4,8);
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; }); cout << endl;
}

/*
replace_if算法 将容器内指定范围满足条件的元素替换为新元素
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param callback函数回调或者谓词(返回Bool类型的函数对象)
	@param oldvalue 新元素
	replace_if(iterator beg, iterator end, _callback, newvalue);
*/
class MyReplaceIf
{
public:
	bool operator()(int a)
	{
		//自定义规则处
		return a < 5;
	}

};
void test03()
{
	//初始化容器
	vector<int> v1;
	for (int i = 0; i < 10; i++)
		v1.push_back(i);
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; }); cout << endl;

	//replace_if算法 将容器内指定范围满足条件的元素替换为新元素
	replace_if(v1.begin(),v1.end(),MyReplaceIf(),5);
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; }); cout << endl;
}

/*
swap算法 互换两个容器的元素
	@param c1容器1
	@param c2容器2
	swap(container c1, container c2);
*/
void test04()
{
	//初始化容器
	vector<int> v1;
	vector<int> v2;
	for (int i = 0; i < 10; i++)
		v1.push_back(i);
	for (int i = 9; i>-1; i--)
		v2.push_back(i);
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; }); cout << endl;
	for_each(v2.begin(), v2.end(), [](int a) {cout << a << " "; }); cout << endl;

	//replace_if算法 将容器内指定范围满足条件的元素替换为新元素
	swap(v1, v2);
	cout << "交换后" << endl;
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; }); cout << endl;
	for_each(v2.begin(), v2.end(), [](int a) {cout << a << " "; }); cout << endl;
}



int main()
{
	test04();
	system("pause");
	return 0;
}