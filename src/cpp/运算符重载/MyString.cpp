#include "MyString.h"

MyString::MyString()
{	
	this->pString = (char*)malloc(8);
	memset(this->pString,0x0,8);
	this->size = 8;
}

MyString::MyString(const char * buf)
{
	if (this->pString!=NULL )
	{
		free(this->pString);
		this->pString = NULL;
		this->size = 0;
	}

	this->pString = (char*)malloc(strlen(buf)+1);
	memset(pString,0x0,strlen(buf)+1);
	strcpy(this->pString,buf);
	this->size = (strlen(this->pString)+1);

}

MyString::MyString(MyString & str)
{
	this->pString = (char*)malloc(str.size);
	memcpy(this->pString,str.pString,str.size);
	this->size = str.size;
}


MyString::~MyString()
{
	if (this->pString!=NULL)
	{
		free(this->pString);
		this->size = 0;
	}

}







MyString & MyString::operator=(const MyString & str)
{
	if (this->size < str.size)
	{
		free(this->pString);
		this->pString = (char*)malloc(str.size);
	}
	memset(this->pString,0x0,this->size);
	memcpy(this->pString, str.pString, str.size);
	this->size = str.size;
	return *this;

}

MyString & MyString::operator=(const char * str)
{
	if (this->size < strlen(str)+1)
	{
		free(this->pString);
		this->pString = (char*)malloc(strlen(str) + 1);
	}
	memset(this->pString, 0x0, this->size);
	strcpy(this->pString, str);
	this->size = strlen(str) + 1;
	return *this;
}

MyString MyString::operator+(MyString & str)
{
	char * temp = (char*)malloc(str.size+this->size+1);
	memset(temp, 0x0, str.size + this->size + 1);
	strcpy(temp,this->pString);
	strcat(temp,str.pString);
	MyString ms(temp);
	free(temp);
	return ms;	
}

MyString MyString::operator+(const char * str)
{
	char * temp = (char*)malloc((strlen(str) + 1) + this->size );
	memset(temp,0x0, (strlen(str) + 1) + this->size );
	strcpy(temp, this->pString);
	strcat(temp, str);
	MyString ms(temp);
	free(temp);
	return ms;
}

bool MyString::operator==(const char * str)
{
	if (strcmp(this->pString,str)==0)
	{
		return true;
	}
	return false;
}

bool MyString::operator==(MyString & str)
{
	if (strcmp(this->pString, str.pString) == 0)
	{
		return true;
	}
	return false;
}

char & MyString::operator[](int i)
{
	if (i < this->size)
	{
		return this->pString[i];
	}
}

unsigned int MyString::getSize()
{
	return this->size;
}

ostream & operator<<(ostream & cout, MyString & str)
{
	cout << str.pString;
	return cout;
	
}
