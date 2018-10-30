#pragma once
#include <string>
//String字符串类,暂时用数组存储
//推荐将其当做类似JAVA C#中的引用变量来用(即指针)
class CharString
{
	int maxSize_;
	int size_;
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
	void operator = (const std::string & str);
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
	bool equal (CharString * rstr);
	//

	//CharString():charData_(NULL),size_(0),maxSize_(0){}
	CharString(const std::wstring &);
	CharString(const wchar_t[]);
	CharString(CharString *);
	~CharString();
public:
	const static int UNMATCH = -1;//这种初始化可能不适合c++ 11 以下的版本
};

