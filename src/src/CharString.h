#pragma once
#include <string>
#include <assert.h>
#include <iostream>
#include <locale>
//String字符串类,用wchar_t数组存储宽字符串
class CharString
{
	int maxSize_;
	int size_ = 0;
	wchar_t* charData_;
	friend class CharString;
public:
	//查找子串位置
	int indexOf(const CharString &, int start);
	//截取子串 左闭右开
	CharString* subString(int begin, int end);
	//连接子串
	void concat(const CharString & str);
	//赋值
	void operator = (const CharString & str);
	void operator = (const std::wstring & str);
	//长度
	int size() const { return size_; }
	//resize 更改最大数组大小
	void resize(int maxSize);
	//下标引用 
	const wchar_t & operator [](int index) const;
	wchar_t & operator [](int index);
	const wchar_t & charAt(int index) const;
	wchar_t & charAt(int index);
	//比较函数
	bool equal (const CharString * rstr);
	void push_back(wchar_t);
	//用给分词器,降低复杂度
	void pop_back();
	std::wstring to_wstring();

	void print();
	friend std::wostream & operator <<(std::wostream &os, CharString & str);
	//CharString():charData_(NULL),size_(0),maxSize_(0){}
	CharString(const std::wstring &);
	CharString(const wchar_t[]);
	CharString(CharString *);
	~CharString();
public:
	const static int UNMATCH = -1;//这种初始化可能不适合c++ 11 以下的版本
};

