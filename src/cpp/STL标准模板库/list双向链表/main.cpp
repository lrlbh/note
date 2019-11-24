#include<iostream>
using namespace std;
#include <string>
#include<list>

/*
list构造函数：
	list<T> lstT;//list采用采用模板类实现,对象的默认构造形式：
	list(beg, end);//构造函数将[beg, end)区间中的元素拷贝给本身。
	list(n, elem);//构造函数将n个elem拷贝给本身。
	list(const list &lst);//拷贝构造函数。
	
list数据元素插入和删除操作：
	push_back(elem);//在容器尾部加入一个元素
	pop_back();//删除容器中最后一个元素
	push_front(elem);//在容器开头插入一个元素
	pop_front();//从容器开头移除第一个元素
	insert(pos, elem);//在pos位置插elem元素的拷贝，返回新数据的位置。
	insert(pos, n, elem);//在pos位置插入n个elem数据，无返回值。
	insert(pos, beg, end);//在pos位置插入[beg,end)区间的数据，无返回值。
	clear();//移除容器的所有数据
	erase(beg, end);//删除[beg,end)区间的数据，返回下一个数据的位置。
	erase(pos);//删除pos位置的数据，返回下一个数据的位置。
	remove(elem);//删除容器中所有与elem值匹配的元素。

list大小操作：
	size();//返回容器中元素的个数
	empty();//判断容器是否为空
	resize(num);//重新指定容器的长度为num，
	若容器变长，则以默认值填充新位置。
	如果容器变短，则末尾超出容器长度的元素被删除。
	resize(num, elem);//重新指定容器的长度为num，
	若容器变长，则以elem值填充新位置。
	如果容器变短，则末尾超出容器长度的元素被删除。

list赋值操作：
	assign(beg, end);//将[beg, end)区间中的数据拷贝赋值给本身。
	assign(n, elem);//将n个elem拷贝赋值给本身。
	list& operator=(const list &lst);//重载等号操作符
	swap(lst);//将lst与本身的元素互换。

list数据的存取：
	front();//返回第一个元素。
	back();//返回最后一个元素。

list反转排序：
	reverse();//反转链表，比如lst包含1,3,5元素，运行此方法后，lst就包含5,3,1元素。
	sort(); //list排序
*/


class Person
{
public:
	Person(const char * name, int age)
	{
		this->name = name;
		this->age = age;
	}

	Person()
	{
	}

	//重载等号
	bool operator<(Person &p)
	{
		//age相同的情况下使用name排序
		if (this->age == p.age)
			return this->name < p.name;

		//age排序
		return this->age < p.age;
	}

	string name;
	int age;
};

void test01()
{
	list<Person> p;
	p.insert(p.end(), Person("AA", 12));
	p.insert(p.begin(), Person("BB", 13));
	p.insert(p.end(), Person("CC", 14));
	p.insert(p.begin(), Person("DD", 15));
	p.insert(p.end(), Person("EE", 16));
	p.insert(p.end(), Person("FF", 16));
	p.insert(p.end(), Person("DD", 16));

	//迭代器的定义
	//容器::iterator 迭代器名 = 返回迭代器的方法;
	//list<Person>::iterator it= p.begin();

	//遍历
	for (list<Person>::iterator it = p.begin();it!=p.end();)
		cout <<"姓名："<< (*(it++)).name<<"  年龄："<<it->age<<endl;
	cout << endl << endl;

	//逆序
	p.reverse();

	//遍历
	for (list<Person>::iterator it = p.begin(); it != p.end();)
		cout << "姓名：" << (*(it++)).name << "  年龄：" << it->age << endl;
	cout << endl << endl;

	//排序
	//自定义类型排序需要重载 < 号,升序
	//想逆序可以直接改重载 < 号的的代码
	//或者写个回调函数
	p.sort();
	//遍历
	for (list<Person>::iterator it = p.begin(); it != p.end();)
		cout << "姓名：" << (*(it++)).name << "  年龄：" << it->age << endl;

	
}

int main()
{
	test01();

	system("pause");
	return 0;
}