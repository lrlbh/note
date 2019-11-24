#include<iostream>
using namespace std;
#include<map>
#include<string>
/*
map构造函数：
	map<T1, T2> mapTT;//map默认构造函数: 
	map(const map &mp);//拷贝构造函数
	
	
map赋值操作：
	map& operator=(const map &mp);//重载等号操作符
	swap(mp);//交换两个集合容器
	
map大小操作：
	size();//返回容器中元素的数目
	empty();//判断容器是否为空
	
map插入数据元素操作：
	map.insert(...); //往容器插入元素，返回pair<iterator,bool>
	map<int, string> mapStu;
	// 第一种 通过pair的方式插入对象
	mapStu.insert(pair<int, string>(3, "小张"));
	// 第二种 通过pair的方式插入对象
	mapStu.inset(make_pair(-1, "校长"));
	// 第三种 通过value_type的方式插入对象
	mapStu.insert(map<int, string>::value_type(1, "小李"));
	// 第四种 通过数组的方式插入值
	mapStu[3] = "小刘";
	mapStu[5] = "小王";
	
map删除操作：
	clear();//删除所有元素
	erase(pos);//删除pos迭代器所指的元素，返回下一个元素的迭代器。
	erase(beg, end);//删除区间[beg,end)的所有元素 ，返回下一个元素的迭代器。
	erase(keyElem);//删除容器中key为keyElem的对组。

map查找操作：
	find(key);//查找键key是否存在,若存在，返回该键的元素的迭代器；/若不存在，返回map.end();
	count(keyElem);//返回容器中key为keyElem的对组个数。对map来说，要么是0，要么是1。对multimap来说，值可能大于1。
	lower_bound(keyElem);//返回第一个key>=keyElem元素的迭代器。
	upper_bound(keyElem);//返回第一个key>keyElem元素的迭代器。
	equal_range(keyElem);//返回容器中key与keyElem相等的上下限的两个迭代器。
*/

class Person
{
public:
	Person()
	{}

	Person(string name,int age)
	{
		this->name = name;
		this->age = age;
	}

	string name;
	int age;
};

void myPerinfMap(map<int, Person> &p)
{
	for (map<int, Person>::iterator it = p.begin();
		it!=p.end();++it)
	{
		cout<< "ID=" <<(*it).first<<"  name="<<(*it).second.name 
			<<"  age="<<(*it).second.age <<endl;
	}
	cout << endl<<endl;
}

/*
map插入数据元素操作：
	map.insert(...); //往容器插入元素，返回pair<iterator,bool>
	map<int, string> mapStu;
	// 第一种 通过pair的方式插入对象
	mapStu.insert(pair<int, string>(3, "小张"));
	// 第二种 通过pair的方式插入对象
	mapStu.inset(make_pair(-1, "校长"));
	// 第三种 通过value_type的方式插入对象
	mapStu.insert(map<int, string>::value_type(1, "小李"));
	// 第四种 通过数组的方式插入值
	mapStu[3] = "小刘";
	mapStu[5] = "小王";
*/
void test01()
{
	map<int, Person> p;

	//第一种方式
	p.insert(pair<int,Person>(1,Person("BB",13)));

	//第二种方式
	p.insert(make_pair(0, Person("AA",12)));

	//第三种方式
	p.insert(map<int,Person>::value_type(-1,Person("CC",14)));

	//第四种方式
	//下标为键，ID
	//下标重复的情况下，修改ID
	p[-1] = Person("DD",15);
	p[4] = Person("EE", 16);
	p[3] = Person("FF", 17);
	myPerinfMap(p);
}

/*
map删除操作：
	clear();//删除所有元素
	erase(pos);//删除pos迭代器所指的元素，返回下一个元素的迭代器。
	erase(beg, end);//删除区间[beg,end)的所有元素 ，返回下一个元素的迭代器。
	erase(keyElem);//删除容器中key为keyElem的对组。
*/
void test02()
{
	//初始化
	map<int, Person> p;
	p[0] = Person("DD", 15);
	p[4] = Person("EE", 16);
	p[3] = Person("FF", 17);
	p[1] = Person("AA", 18);
	p[2] = Person("BB", 18);
	p[5] = Person("CC", 19);
	myPerinfMap(p);

	//erase(keyElem);//删除容器中key为keyElem的对组。
	//重复删除，没反应
	//p.erase(1);
	p.erase(1);
	myPerinfMap(p);

	//其方法同set容器不演示了
}

/*
map查找操作：
	find(key);//查找键key是否存在,若存在，返回该键的元素的迭代器；/若不存在，返回map.end();
	count(keyElem);//返回容器中key为keyElem的对组个数。对map来说，要么是0，要么是1。对multimap来说，值可能大于1。
	lower_bound(keyElem);//返回第一个key>=keyElem元素的迭代器。
	upper_bound(keyElem);//返回第一个key>keyElem元素的迭代器。
	equal_range(keyElem);//返回容器中key与keyElem相等的上下限的两个迭代器。
*/
void test03()
{
	//初始化
	map<int, Person> p;
	p[0] = Person("DD", 15);
	p[4] = Person("EE", 16);
	p[3] = Person("FF", 17);
	p[1] = Person("AA", 18);
	p[2] = Person("BB", 18);
	p[5] = Person("CC", 19);
	myPerinfMap(p);

	//find(key);//查找键key是否存在,若存在，返回该键的元素的迭代器；/若不存在，返回map.end();
	cout << "ID=" << (*p.find(1)).first << "  name="
		<< (*p.find(1)).second.name<<"  age="
		<< (*p.find(1)).second.age << endl;

	//equal_range(keyElem);//返回容器中key与keyElem相等的上下限的两个迭代器。
	pair<map<int, Person>::iterator, map<int, Person>::iterator> dz = p.equal_range(2);
	cout << "ID=\t" << dz.first->first << endl;
	cout << "age=\t" << dz.first->second.age << endl;
	cout << "name=\t"<<dz.first->second.name << endl;
	cout<<endl << endl;
	cout << "ID=\t" << dz.second->first << endl;
	cout << "age=\t" << dz.second->second.age << endl;
	cout << "name=\t" << dz.second->second.name << endl;
}

int main()
{
	test03();
	system("pause");
	return 0;
}