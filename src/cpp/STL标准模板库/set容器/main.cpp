#include<iostream>
using namespace std;
#include<set>
#include<string>

class MySet
{
public:
	bool operator()(const int & mySet1, const int & mySet2)
	{
		return mySet1 > mySet2;
	}
};

/*
构造函数：
	set<T> st;//set默认构造函数：
	mulitset<T> mst; //multiset默认构造函数: 
	set(const set &st);//拷贝构造函数

set赋值操作：
	set& operator=(const set &st);//重载等号操作符
	swap(st);//交换两个集合容器

set大小操作：
	size();//返回容器中元素的数目
	empty();//判断容器是否为空

set插入和删除操作：
	insert(elem);//在容器中插入元素。
	clear();//清除所有元素
	erase(pos);//删除pos迭代器所指的元素，返回下一个元素的迭代器。
	erase(beg, end);//删除区间[beg,end)的所有元素 ，返回下一个元素的迭代器。
	erase(elem);//删除容器中值为elem的元素。

set查找操作：
	find(key);//查找键key是否存在,若存在，返回该键的元素的迭代器；若不存在，返回set.end();
	count(key);//查找键key的元素个数
	lower_bound(keyElem);//返回第一个key>=keyElem元素的迭代器。
	upper_bound(keyElem);//返回第一个key>keyElem元素的迭代器。
	equal_range(keyElem);//返回容器中key与keyElem相等的上下限的两个迭代器。
*/

void mySetPrintf(set<int> &s)
{
	int i = 0;
	for (set<int>::iterator it = s.begin();it != s.end();)
	{
		cout << *it << endl;
		it++;
		i++;
	}
	if (i == 0)
		cout << "NULL"<<endl;
	cout << endl << endl;
}

void mySetPrintf2(set<int,MySet> &s)
{
	int i = 0;
	for (set<int>::iterator it = s.begin(); it != s.end();)
	{
		cout << *it << endl;
		it++;
		i++;
	}
	if (i == 0)
		cout << "NULL" << endl;
	cout << endl << endl;
}

/*
set插入和删除操作：
	insert(elem);//在容器中插入元素。
	clear();//清除所有元素
	erase(pos);//删除pos迭代器所指的元素，返回下一个元素的迭代器。
	erase(beg, end);//删除区间[beg,end)的所有元素 ，返回下一个元素的迭代器。
	erase(elem);//删除容器中值为elem的元素。
*/
void test01()
{
	//初始化
	set<int> s1;

	//insert(elem);//在容器中插入元素。
	s1.insert(1);
	s1.insert(0);
	s1.insert(99);
	s1.insert(5);
	s1.insert(50);
	s1.insert(324);
	s1.insert(3453);
	mySetPrintf(s1);

	//删除指定元素
	//erase(pos);//删除pos迭代器所指的元素，返回下一个元素的迭代器
	for (set<int>::iterator it = s1.begin();it!=s1.end();)
	{	
		if ((*it) == 324)
		//it必须接收返回值，否则当前匀速被删迭代器失效
			it = s1.erase(it);
		it++;
	}
	mySetPrintf(s1);

	//erase(elem);//删除容器中值为elem的元素
	s1.erase(99);
	mySetPrintf(s1);

	//clear();//清除所有元素
	s1.clear();
	mySetPrintf(s1);

}

/*
set查找操作：
	find(key);//查找键key是否存在,若存在，返回该键的元素的迭代器；若不存在，返回set.end();
	count(key);//查找键key的元素个数
	lower_bound(keyElem);//返回第一个key>=keyElem元素的迭代器。
	upper_bound(keyElem);//返回第一个key>keyElem元素的迭代器。
	equal_range(keyElem);//返回容器中key与keyElem相等的上下限的两个迭代器。
*/
void test02()
{
	//初始化
	set<int> s1;
	//insert(elem);//在容器中插入元素。
	s1.insert(1);
	s1.insert(4);
	s1.insert(7);
	s1.insert(2);
	s1.insert(5);
	s1.insert(8);
	s1.insert(3);
	s1.insert(6);
	s1.insert(9);
	mySetPrintf(s1);

	//find(key);//查找键key是否存在,若存在，返回该键的元素的迭代器；若不存在，返回set.end();
	cout << *(s1.find(1)) << endl;

	//count(key);//查找键key的元素个数
	cout << s1.count(5) << endl;

	//lower_bound(keyElem);//返回第一个key >= keyElem元素的迭代器。
	cout << *s1.lower_bound(2) << endl;

	//upper_bound(keyElem);//返回第一个key > keyElem元素的迭代器。
	cout << *s1.upper_bound(2)<< endl;

	//equal_range(keyElem);//返回容器中key与keyElem相等的上下限的两个迭代器,
	//对组接收
	pair<set<int>::iterator, set<int>::iterator> pit = s1.equal_range(5);
	cout << "下一个元素的迭代器*(pid.second)="<<*(pit.second) << endl;//下一个元素的迭代器
	cout << "当前元素的迭代器*(pid.first)="<<*(pit.first) << endl;;//当前元素的迭代器
	
	//自定义排序规则降序
	set<int,MySet> s2;
	s2.insert(1);
	s2.insert(4);
	s2.insert(7);
	s2.insert(2);
	s2.insert(5);
	s2.insert(8);
	s2.insert(3);
	s2.insert(6);
	s2.insert(9);
	mySetPrintf2(s2);
}

int main()
{
	test02();

	system("pause");
	return 0;
}