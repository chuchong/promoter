#pragma once
//本.h中提供存储String的数据结构Collection 以及对Collection操作的Iterator
//已经实现了双向链表,所以名称为CharStringLink
//若实现了其他collection结构,将会把collection移出去
#include <string>
#include <fstream>

#include <ostream>
#include <locale>
#include <codecvt>
#include <assert.h>
#include "CharString.h"
class Iterator {
public:
	virtual Iterator& operator++ () = 0;
	virtual const Iterator & operator++ (int ) = 0;
	virtual CharString* operator*() = 0;
	virtual bool operator==(Iterator&) = 0;
};
class CharStringCollection {
public:
	virtual void add(CharString *str) = 0;
	virtual void remove(CharString *str) = 0;
	virtual bool search(CharString *str) = 0;
	virtual bool isEmpty() = 0;
	virtual Iterator * begin() = 0;
	virtual Iterator * end() = 0;
};
//------------------------下面为双向链表实现
struct ListNode {
	CharString *str_ = nullptr;
	ListNode * next_ = nullptr;
	ListNode * prior_ = nullptr;
	ListNode(CharString * str);
	ListNode() {
	}
	~ListNode() {
		delete str_;
	}
};
class CharStringLink;
//推荐使用Iterator访问StringLink中的变量
class ListIterator : public Iterator {
private:
	CharStringLink * checker;
	ListNode * node;
	friend class CharStringLink;
public:
	virtual const Iterator & operator++ (int) {
		ListIterator tmp = *this;
		node = node->next_;
		return tmp;
	}
	virtual Iterator& operator++ () {
		node = node->next_;
		return *this;
	}
	virtual CharString* operator*() {
		return node->str_;
	}
	virtual bool operator==(Iterator &other) {
		return (node == dynamic_cast<ListIterator*>(&other)->node);
	}
	ListIterator(ListNode * n) :node(n) {

	}
};

class CharStringLink : public CharStringCollection {
	ListNode * head_ = nullptr;
	ListNode * last_ = nullptr;
public:
	void print();
	//delete iterator 指向的 node,注意:!!iter会失效 
	virtual void remove(ListIterator * iter);
	virtual Iterator * begin() {
		return new ListIterator(head_);
	}
	virtual Iterator * end() {
		return new ListIterator(nullptr);
	}
	virtual bool isEmpty() override {
		return head_ == nullptr;
	}
	void add(const std::wstring & str);
	void add(CharString * str);
	//用来删除某个同名字符串的第一个匹配项
	virtual void remove(CharString *str);
	virtual bool search(CharString *str) override;
	void output(CharString * place);
	~CharStringLink();
};

