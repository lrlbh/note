#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include <iomanip> // io  man  ip

/*
cout.put() //�򻺳���д�ַ�
cout.write() //��buffer��дnum���ֽڵ���ǰ������С�
*/

void test01()
{
	//cout.put('h').put('e').put('l');

	//char buf[] = "hello world";
	//cout.write(buf, sizeof(buf));

	cout << "hello world" << endl;
}


//ͨ������Ա����
void test02(){

	int number = 99;
	cout.width(10); //���ÿ��
	cout.fill('*'); //���
	cout.setf(ios::left);  //����������ʽ
	cout.unsetf(ios::dec); //ж��ʮ����
	cout.setf(ios::hex);  //����ʮ������   63
	cout.setf(ios::showbase); //��ʾ���� 0x (ʮ������)  0 ���˽��ƣ�
	cout.unsetf(ios::hex); //ж��ʮ������
	cout.setf(ios::oct);  //���ð˽���  
	cout << number << endl;
}


void test03()
{
	//ʹ�ÿ��Ʒ�
	int number = 99;
	cout << setw(20)   //���ÿ��
		 << setfill('~') //���
		 << setiosflags(ios::showbase) //��ʾ����
		<< setiosflags(ios::left)  //�����
		<< hex  //����ʮ������
		<< number
		<< endl;
}


int main(){

	//test01();

	//test02();

	test03();

	system("pause");
	return EXIT_SUCCESS;
}