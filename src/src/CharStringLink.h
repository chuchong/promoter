#pragma once
#include <string>
#include <fstream>

#include <ostream>
#include <locale>
#include <codecvt>
#include <assert.h>
#include "CharString.h"
struct CharStringNode {
	CharStringNode *next;
	CharStringNode *prior;
	CharString *str;
	CharStringNode() {
		prior = next = NULL;
		str = NULL;
	}
	CharStringNode(CharString * s) {
		prior = next = NULL;
		str = s;
	}
	~CharStringNode() {
		if(str != nullptr)
		delete str;
	}
};
//存储String 的链表 ,暂时用来存储
class CharStringLink
{
	CharStringNode * head;
	CharStringNode * end;
	//链表的删除某节点的方法,考虑采用iterator方式遍历
	void remove(CharStringNode * str_ptr);
public:
	//添加一个节点于最后
	bool isEmpty() {
		return head == NULL;
	}

	void add(const std::wstring & str);
	void add(CharString * str);
	//删除位于第n个的str
	void deleteAt(int index);
	//dayin
	void print() {
		CharStringNode * node = head;
		while (node != nullptr) {
			node->str->print();
			std::cout << " ";
			node = node->next;
		}
	}
	//查找是否存在,若不存在,返回-1
	int search(const CharString & str);
	void output(CharString * place) {
		std::wstring url = place->to_wstring() + L".txt";
		std::locale loc("chs");
		std::wofstream os;
		os.imbue(loc);
		os.open(url);

		CharStringNode *node = head;
		while (node != nullptr) {
			os << (*(node->str)) << std::endl;
			node = node->next;
		}
	}
	CharStringLink();
	~CharStringLink();
};

