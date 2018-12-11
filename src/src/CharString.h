#pragma once
#include <string>
#include <assert.h>
#include <iostream>
#include <locale>
//String�ַ�����,��wchar_t����洢���ַ���
class CharString
{
	int maxSize_;
	int size_ = 0;
	wchar_t* charData_;
	friend class CharString;
public:
	//�����Ӵ�λ��
	int indexOf(const CharString &, int start);
	//��ȡ�Ӵ� ����ҿ�
	CharString* subString(int begin, int end);
	//�����Ӵ�
	void concat(const CharString & str);
	//��ֵ
	void operator = (const CharString & str);
	void operator = (const std::wstring & str);
	//����
	int size() const { return size_; }
	//resize ������������С
	void resize(int maxSize);
	//�±����� 
	const wchar_t & operator [](int index) const;
	wchar_t & operator [](int index);
	const wchar_t & charAt(int index) const;
	wchar_t & charAt(int index);
	//�ȽϺ���
	bool equal (const CharString * rstr);
	void push_back(wchar_t);
	//�ø��ִ���,���͸��Ӷ�
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
	const static int UNMATCH = -1;//���ֳ�ʼ�����ܲ��ʺ�c++ 11 ���µİ汾
};

