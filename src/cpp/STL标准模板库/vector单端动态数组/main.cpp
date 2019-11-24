#include<iostream>
using namespace std;
#include<vector>

/*
拷贝构造:
	vector<T> v; //采用模板实现类实现，默认构造函数
	vector(v.begin(), v.end());//将v[begin(), end())区间中的元素拷贝给本身。
	vector(n, elem);//构造函数将n个elem拷贝给本身。
	vector(const vector &vec);//拷贝构造函数。
*/

void myVectotPrintf(vector<int> &v)
{
	for (int i = 0; i < v.size(); i++)
	{
		cout << v[i]<<" ";
		/*int a = v[i];
		cout << a << endl;*/
	}
	cout <<endl;
}

void test01()
{
	//第一个构造
	//vector<T> v;
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);
	v1.push_back(4);
	myVectotPrintf(v1);

	//第二个构造
	//vector(v.begin(), v.end());
	int arr[] = {1,4,7,2,5,8};
	vector<int> v2(arr, arr+sizeof(arr)/sizeof(int));
	myVectotPrintf(v2);

	//第三个构造
	//vector(n, elem);
	vector<int> v3(10,1);
	myVectotPrintf(v3);

	//第四个构造
	//vector(const vector &vec);
	vector<int> v4(v1);
	myVectotPrintf(v4);
}

/*
赋值操作
	assign(beg, end);//将[beg, end)区间中的数据拷贝赋值给本身。
	assign(n, elem);//将n个elem拷贝赋值给本身。
	vector& operator=(const vector  &vec);//重载等号操作符
	swap(vec);// 将vec与本身的元素互换。
*/
void test02()
{
	int arr[] = { 1,4,7,2,5,8,3,6,9};
	vector<int> v1(arr, arr + sizeof(arr) / sizeof(int));

	//assign(beg, end);//将[beg, end)区间中的数据拷贝赋值给本身
	vector<int> v2;
	v2.assign(v1.begin()+3,v1.begin()+6);
	myVectotPrintf(v2);

	//assign(n, elem);//将n个elem拷贝赋值给本身
	vector<int> v3;
	v3.assign(5,1);
	myVectotPrintf(v3);

	//vector& operator=(const vector  &vec);//重载等号操作符
	vector<int> v4;
	v4 = v2;
	myVectotPrintf(v4);

	//swap(vec);// 将vec与本身的元素互换。
	v1.swap(v3);
	myVectotPrintf(v1);
	myVectotPrintf(v3);
}

/*
vector的大小操作：
	size();//返回容器中元素的个数
	empty();//判断容器是否为空，判断size
	resize(int num);//重新指定容器的长度为num，若容器变长，则以默认值填充新位置。如果容器变短，则末尾超出容器长度的元素被删除。
	resize(int num, elem);//重新指定容器的长度为num，若容器变长，则以elem值填充新位置。如果容器变短，则末尾超出容器长>度的元素被删除。
	capacity();//容器的容量
	reserve(int len);//给容器预留容量，避免重复开辟、释放，提高效率
*/
void test03()
{
	int arr[] = { 1,4,7,2,5,8,3,6,9 };
	vector<int> v1(arr, arr + sizeof(arr) / sizeof(int));

	//resize(int num);重新指定容器的长度
	//指定的是元素的个数。与容量无关
	v1.resize(0);
	myVectotPrintf(v1);

	//empty();//判断容器是否为空
	//判断容器是否为空根据元素的个数决定，与容量无关
	if (v1.empty())
	{
		cout<< "容器为空"<<endl;
	}
	
	//reserve(int len); 给容器预留容量，避免重复开辟、释放，提高效率
	vector<int> v2;
	v2.reserve(10000);
	cout<<v2.capacity()<<endl;
}

/*
vector数据存取操作：
	at(int idx); //返回索引idx所指的数据，如果idx越界，抛出out_of_range异常。
	operator[];//返回索引idx所指的数据，越界时，运行直接报错
	front();//返回容器中第一个数据元素
	back();//返回容器中最后一个数据元素
*/
void test04()
{
	//数据存取用中括号就行
	int arr[] = { 1,4,7,2,5,8,3,6,9 };
	vector<int> v1(arr, arr + sizeof(arr) / sizeof(int));
}

/*
vector插入和删除操作：
	insert(const_iterator pos, int count, ele);//迭代器指向位置pos插入count个元素ele.
	push_back(ele); //尾部插入元素ele
	pop_back();//删除最后一个元素
	erase(const_iterator start, const_iterator end);//删除迭代器从start到end之间的元素
	erase(const_iterator pos);//删除迭代器指向的元素
	clear();//删除容器中所有元素，删除的是元素个数，空间容量不变
*/
void test05()
{
	int arr[] = { 1,4,7,2,5,8,3,6,9 };
	vector<int> v1(arr, arr + sizeof(arr) / sizeof(int));



	//insert(const_iterator pos, int count, ele);
	v1.insert(v1.begin()+5,2,444);
	myVectotPrintf(v1);

	//push_back(ele); //尾部插入元素ele
	v1.push_back(123);
	myVectotPrintf(v1);

	//pop_back();//删除最后一个元素
	v1.pop_back();
	myVectotPrintf(v1);

	//erase(const_iterator start, const_iterator end);
	//删除迭代器从start到end之间的元素
	v1.erase(v1.begin()+5,v1.begin()+7);
	myVectotPrintf(v1);

	//erase(const_iterator pos);//删除迭代器指向的元素
	v1.erase(v1.begin()+4);
	myVectotPrintf(v1);

	//clear();//删除容器中所有元素,删除的是元素个数，空间容量不变
	v1.clear();
	cout << v1.capacity() << endl;
}

//巧用swap，收缩内存空间
void test06()
{
	vector<int> v;
	for (int i = 0; i < 100000; i++) {
		v.push_back(i);
	}

	cout << "capacity:" << v.capacity() << endl;
	cout << "size:" << v.size() << endl;

	//此时 通过resize改变容器的元素个数
	v.resize(10);

	cout << "capacity:" << v.capacity() << endl;
	cout << "size:" << v.size() << endl;

	//收缩内存空间
	//vector<int>(v)  相当于匿名对象，调用了拷贝
	//.swap(v)  把匿名对象和v容器交换
	vector<int>(v).swap(v);

	cout << "capacity:" << v.capacity() << endl;
	cout << "size:" << v.size() << endl;


}

int main()
{
	test06();
	system("pause");
	return 0;
}