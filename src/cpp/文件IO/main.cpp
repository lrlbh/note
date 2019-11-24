#include<iostream>
using namespace std;
#include<fstream>
#include<string>

//C++文件打开方式
//ios::in	以输入方式打开文件
//ios::out	以输出方式打开文件（这是默认方式），如果已有此名字的文件，则将其原有内容全部清除
//ios::app	以输出方式打开文件，写入的数据添加在文件末尾
//ios::ate	打开一个已有的文件，文件指针指向文件末尾
//ios::trunc	打开一个文件，如果文件已存在，则删除其中全部数据，如文件不存在，则建立新文件。如已指定了 ios::out 方式，而未指定ios : : app，ios::ate，ios : : in，则同时默认此方式
//ios::binary	以二进制方式打开一个文件，如不指定此方式则默认为ASCII方式
//ios::nocreate	打开一个已有的文件，如文件不存在，则打开失败。nocrcate的意思是不建立新文件
//ios::noreplace	如果文件不存在则建立新文件，如果文件已存在则操作失败，replace 的意思是不更新原有文件
//ios::in | ios::out	以输入和输出方式打开文件，文件可读可写
//ios::out | ios::binary	以二进制方式打开一个输出文件
//ios::in | ios::binar	以二进制方式打开一个输入文件

//写文件
void test01()
{
	ofstream ofs("./test.txt", ios::in | ios::trunc);
	if (!ofs.is_open())
	{
		cout << "文件打开失败" << endl;
		return;
	}

	//写入内容
	ofs << "姓名:diaomao\n";
	ofs << "年龄:12" << endl;
	ofs << "性别:男" << endl;

	//关闭对象
	ofs.close();
}

//读文件
void test02()
{
	ifstream ifs;
	ifs.open("./test.txt",ios::in);
	if (!ifs.is_open())
	{
		cout << "文件打开失败" << endl;
		return;
	}

	char buf[1024] = {0};
	//读取文件内容,第一种方式
	//while (ifs>>buf)
	//{
	//	cout << buf << endl;
	//}

	//第二种方式
	//while (ifs.getline(buf,sizeof(buf)))
	//{
	//	cout << buf << endl;
	//}

	//第三种方式
	//string str;
	//while (getline(ifs,str))
	//{
	//	cout << str << endl;
	//}

	//第四种方式,单个字符读取
	char c = 0;
	while ( (c=ifs.get()) != EOF)
	{
		cout << c;
	}

	ifs.close();
}


int main()
{
	test01();
	test02();
	system("pause");
	return 0;
}