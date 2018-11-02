
//二字哈希
//使用((innercode of ch1 * innercode of ch2) / 10) mod N 来实现
//一19968 龟40863
//innercode = (highbyte - 176) * 94 + (lowbyte) * 161//翻译成16进制
//N 最好是质数
#pragma once
#include <string>
#include <fstream>
#include <ostream>
#include <locale>
#include <codecvt>
#include <assert.h>
#include <sstream>
#include "CharString.h"
#include "Dict.h"
//hash 表中每个节点的具体实现,节点上可以存储不止一个单词,这里先写个接口,后续可以改变节点数据结构提高效率
class HashNodeIterator {
public:
	virtual void operator++ () = 0;
	virtual CharString* operator*() = 0;
	virtual bool operator==(HashNodeIterator&) = 0;
};
class HashNode {
public:
	virtual void add(CharString *str) = 0;
	virtual void remove(CharString *str) = 0;
	virtual bool search(CharString *str) = 0;
	virtual bool isEmpty() = 0;
	virtual HashNodeIterator * begin() = 0;
	virtual HashNodeIterator * end() = 0;
};
//用单链表来实现上述的HashNode,查找就直接遍历看一看

struct HashListNode_Node {
	CharString *str_ = nullptr;
	HashListNode_Node * next = nullptr;
	HashListNode_Node(CharString * str) {
		str_ = new CharString(str);
	}
	~HashListNode_Node() {
		delete str_;
	}
};
class HashListNodeIterator: public HashNodeIterator {
private:
	HashListNode_Node * node;
public:
	virtual void operator++ () {
		node = node->next;
	}
	virtual CharString* operator*() {
		return node->str_;
	}
	virtual bool operator==(HashNodeIterator &other) {
		return (node == dynamic_cast<HashListNodeIterator*>(&other)->node);
	}
	HashListNodeIterator(HashListNode_Node * n):node(n){

	}
};

class HashListNode : public HashNode {
	HashListNode_Node * head_ = nullptr;
	HashListNode_Node * last_ = nullptr;
public:
	virtual HashNodeIterator * begin() {
		return new HashListNodeIterator(head_);
	}
	virtual HashNodeIterator * end() {
		return new HashListNodeIterator(nullptr);
	}
	virtual bool isEmpty() override {
		return head_ == nullptr;
	}
	virtual void add(CharString *p_str) {
		HashListNode_Node * node = head_;
		if (head_ == nullptr) {
			head_ = new HashListNode_Node(p_str);
			last_ = head_;
		}
		else {
			last_->next = new HashListNode_Node(p_str);
			last_ = last_->next;
		}
	}
	virtual void remove(CharString *str) {
		HashListNode_Node * p = head_;
		HashListNode_Node * q = head_;
		if (p == nullptr)
			return;
		else {
			q = p->next;
			if (p->str_ ->equal(str)) {// 第一个就是
				head_ = q;
				delete p;
				return;
			}
			while (q != nullptr) {//第一个不是
				if (q->str_->equal(str)) {//q位置是
					p->next = q->next;
					delete q;
					return;
				}
				p = q;
				q = q->next;
			}
		}
	}
	virtual bool search(CharString *str) override{
		HashListNode_Node * p = head_;
		while (p != nullptr) {
			if (p->str_ ->equal(str))
				return 1;
			p = p->next;
		}
		return 0;
	}
	~HashListNode()
	{
		HashListNode_Node * p = head_;
		while (p != nullptr) {
			HashListNode_Node * q = p;
			p = p->next;
			delete q;
		}
	}
};
//-------以上: 单链表来实现上述的HashNode,查找就直接遍历看一看
//每个hashdict 存储某一个特定长度的hash字典
class HashDict: public Dict
{
	int mod_;//被模数,同时也是hash表大小
	HashNode* * map_;//前一个是引用变量,map是一个数组
	int innercode(wchar_t ch) {
		int hi = ch >> 8;//highbyte;
		int low = ch & 0x00FF;//lowbyte
		int ans =  hi * 94 + low;
		return ans;
	}

	int hash(CharString * str) {
		wchar_t c1 = str->charAt(0);
		wchar_t c2 = str->charAt(1);
		int inner1 = innercode(c1);
		int inner2 = innercode(c2);
		return ((inner1) * (inner2)) % mod_;//前面乘积应该没超过int 上限 不用删除
	}
public:
	HashDict(int mod):mod_(mod) {
		map_ = new HashNode*[mod_];
		for (int i = 0; i < mod_; i++)
			map_[i] = new HashListNode();
	}
	void addCopyOf(CharString *str) {
		int a = hash(str);
		map_[a]->add(str);
	}
	void remove(CharString *str) {
		map_[hash(str)]->remove(str);
	}
	bool search(CharString *str) {
		return map_[hash(str)]->search(str);
	}
	~HashDict() {
		for (int i = 0; i < mod_; i++)
			delete map_[i];
		delete[] map_;
	}
	//自定义格式并没有多快...
	//void generateINI(std::wstring url) {
	//	//std::ofstream o;
	//	std::locale loc("chs");
	//	std::wofstream os;
	//	os.imbue(loc);

	//	//o.open(url);
	//	os.open(url);


	//	for (int i = 0; i < mod_; i++) {
	//		if (!map_[i]->isEmpty()) {
	//			os << std::to_wstring(i);
	//			HashNodeIterator * iter = map_[i]->begin();
	//			HashNodeIterator * end_iter = map_[i]->end();

	//			while (!((*iter) == (*end_iter))) {
	//				os << L" " << (*(*(*iter)));
	//				++(*iter);
	//			}
	//			os << std::endl;
	//			delete iter;
	//			delete end_iter;
	//		}
	//	}
	//}

	//void getINI(std::wstring url) {
	//	std::locale loc("chs");//windows下ok

	//	std::wifstream in;
	//	in.imbue(loc);
	//	in.open(url);

	//	std::wstring wstr;


	//	std::wstringstream ss;

	//	while (getline(in, wstr)) {
	//		ss.clear();
	//		ss.str(wstr);
	//		int num;
	//		ss >> num;
	//		std::wstring value;
	//		while (ss >> value) {
	//			CharString * s = new CharString(value);
	//			map_[num]->add(s);
	//			delete s;
	//		}

	//	}
	//	return;

	//	//while (getline(in, wstr)) {
	//	//	CharString s(wstr);
	//	//	int index = 0;
	//	//	int num_index = s.indexOf(L" ", index);
	//	//	int num = 0;

	//	//	for (int i = index; i < num_index; i++) {
	//	//		if (s.charAt(i) == L',')//烦人的c++ 
	//	//			continue;
	//	//		num = num * 10 + s[i] - L'0';
	//	//	}

	//	//	index = num_index + 1;
	//	//	while(index < s.size()) {
	//	//		int next_index = s.indexOf(L" ", index);
	//	//		if (next_index != -1) {
	//	//			CharString * sub = s.subString(index, next_index);
	//	//			this->map_[num]->add(sub);
	//	//			delete sub;
	//	//			index = next_index;
	//	//		}
	//	//		index ++;
	//	//	} 
	//	//}
	//}
};


