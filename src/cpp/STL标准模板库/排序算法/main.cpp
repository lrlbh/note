#include<iostream>
using namespace std;
#include<algorithm>
#include<vector>
/*
merge算法 容器元素合并，并存储到另一容器中
注意 : 两个容器必须是有序的
	@param beg1 容器1开始迭代器
	@param end1 容器1结束迭代器
	@param beg2 容器2开始迭代器
	@param end2 容器2结束迭代器
	@param dest  目标容器开始迭代器
*/
void test01()
{
	//初始化容器
	vector<int> v1;
	for (int i = 0; i < 10; i++)
		v1.push_back(i);
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; });
	cout << endl;
	vector<int> v2;
	for (int i = 5; i < 15; i++)
		v2.push_back(i);
	for_each(v2.begin(), v2.end(), [](int a) {cout << a << " "; });
	cout << endl;

	vector<int> v3;
	v3.resize(v1.size()+v2.size());
	//merge算法 
	merge(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin());
	for_each(v3.begin(), v3.end(), [](int a) {cout << a << " "; });
	cout << endl;
}


/*
sort算法 容器元素排序
	@param beg 容器1开始迭代器
	@param end 容器1结束迭代器
	@param _callback 回调函数或者谓词(返回bool类型的函数对象)
	sort(iterator beg, iterator end, _callback)
*/
void test02()
{
	//初始化容器
	vector<int> v1;
	for (int i = 0; i < 10; i++)
		v1.push_back(rand()%10);
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; });
	cout << endl;

	//排序算法默认升序
	sort(v1.begin(), v1.end());
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; }); cout << endl;
	//用内建函数对象，实现降序
	//排序需求复杂，可以自己写仿函数或者回调函数
	sort(v1.begin(), v1.end(),greater<int>());
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; }); cout << endl;
}

/*
random_shuffle算法 对指定范围内的元素随机调整次序
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	random_shuffle(iterator beg, iterator end);
*/
void test03()
{
	//初始化容器
	vector<int> v1;
	for (int i = 0; i < 10; i++)
		v1.push_back(i);
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; }); cout << endl;

	//random_shuffle算法，洗牌、打乱
	random_shuffle(v1.begin(),v1.end());
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; }); cout << endl;
}

/*
reverse算法 反转指定范围的元素
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	reverse(iterator beg, iterator end)
*/
void test04()
{
	//初始化容器
	vector<int> v1;
	for (int i = 0; i < 10; i++)
		v1.push_back(i);
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; }); cout << endl;

	//reverse算法，反转指定范围的元素
	reverse(v1.begin(), v1.end());
	for_each(v1.begin(), v1.end(), [](int a) {cout << a << " "; }); cout << endl;
}





int main()
{
	test04();
	system("pause");
	return 0;
}
