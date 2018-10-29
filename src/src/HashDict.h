
//二字哈希
//使用((innercode of ch1 * innercode of ch2) / 10) mod N 来实现
//一19968 龟40863
//innercode = (highbyte - 176) * 94 + (lowbyte) * 161//翻译成16进制
//N 最好是质数
#pragma once
#include "CharString.h"
//hash 表中每个节点的具体实现,节点上可以存储不止一个单词,这里先写个接口,后续可以改变节点数据结构提高效率
class HashNode {
public:
	virtual void add(CharString *str) = 0;
	virtual void remove(CharString *str) = 0;
	virtual bool search(CharString *str) = 0;
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
class HashListNode : public HashNode {
	HashListNode_Node * head = nullptr;
	HashListNode_Node * end = nullptr;
public:
	virtual void add(CharString *p_str) {
		HashListNode_Node * node = head;
		if (head == nullptr) {
			head = new HashListNode_Node(p_str);
			end = head;
		}
		else {
			end->next = new HashListNode_Node(p_str);
			end = end->next;
		}
	}
	virtual void remove(CharString *str) {
		HashListNode_Node * p = head;
		HashListNode_Node * q = head;
		if (p == nullptr)
			return;
		else {
			q = p->next;
			if (p->str_ ->equal(str)) {// 第一个就是
				head = q;
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
		HashListNode_Node * p = head;
		while (p != nullptr) {
			if (p->str_ ->equal(str))
				return 1;
			p = p->next;
		}
		return 0;
	}
	~HashListNode()
	{
		HashListNode_Node * p = head;
		while (p != nullptr) {
			HashListNode_Node * q = p;
			p = p->next;
			delete q;
		}
	}
};
//-------以上: 单链表来实现上述的HashNode,查找就直接遍历看一看
//每个hashdict 存储某一个特定长度的hash字典
class HashDict
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
	void add(CharString *str) {
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
		delete map_;
	}
};

