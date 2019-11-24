#include<iostream>
using namespace std;
#include<string>
template<class LR>
class ARR
{
public:
	ARR()
	{
		this->data = NULL;
		this->len = 0;
		this->size = 0;
	}

	//传入参数为数组的大小
	ARR(int a)
	{
		this->len = 0;
		this->size = a;
		this->data = new LR[a];
	}

	//拷贝构造
	ARR(ARR & arr )
	{
		this->len = arr->len;
		this->size = arr->size;
		//delete this[];
		this->data = new LR[arr->size];
		for (int i = 0; i < arr->size; i++)
		{
			this->data[i] = arr->data[i];
		}
	}

	//重载等号
	ARR& operator=(ARR & arr )
	{
		if (this->data!=NULL)
		{
			delete[] this->data;
		}

		this->size = arr.size;
		this->len = arr.len;
		this->data = new ARR[arr.size];
		for (int i = 0; i < arr.len; i++)
		{
			this->data[i] = arr.data[i];
		}

		return *this;
	}

	//重载中括号
	LR& operator[](int pos)
	{
		return this->data[pos];
	}

	//尾插
	void pushBack(const LR &lr)
	{
		if (this->len>=(this->size-1))
		{
			return;
		}

		this->data[len] = lr;
		len++;
	}

	//获取数组大小
	int getSize()
	{
		return this->size;
	}


	//获取数组当前有效长度
	int getLen()
	{
		return this->len;
	}


	void weiShan()
	{
	//	delete this->data[len];
		this->len--;
	}

	//析构
	~ARR()
	{
		delete[] this->data;
	}

private:
	//数据
	LR* data;
	//当前长度
	int len;
	//数组大小
	int size;
};
