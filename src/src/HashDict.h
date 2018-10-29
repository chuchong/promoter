
//���ֹ�ϣ
//ʹ��((innercode of ch1 * innercode of ch2) / 10) mod N ��ʵ��
//һ19968 ��40863
//innercode = (highbyte - 176) * 94 + (lowbyte) * 161//�����16����
//N ���������
#pragma once
#include "CharString.h"
//hash ����ÿ���ڵ�ľ���ʵ��,�ڵ��Ͽ��Դ洢��ֹһ������,������д���ӿ�,�������Ըı�ڵ����ݽṹ���Ч��
class HashNode {
public:
	virtual void add(CharString *str) = 0;
	virtual void remove(CharString *str) = 0;
	virtual bool search(CharString *str) = 0;
};
//�õ�������ʵ��������HashNode,���Ҿ�ֱ�ӱ�����һ��
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
			if (p->str_ ->equal(str)) {// ��һ������
				head = q;
				delete p;
				return;
			}
			while (q != nullptr) {//��һ������
				if (q->str_->equal(str)) {//qλ����
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
//-------����: ��������ʵ��������HashNode,���Ҿ�ֱ�ӱ�����һ��
//ÿ��hashdict �洢ĳһ���ض����ȵ�hash�ֵ�
class HashDict
{
	int mod_;//��ģ��,ͬʱҲ��hash���С
	HashNode* * map_;//ǰһ�������ñ���,map��һ������
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
		return ((inner1) * (inner2)) % mod_;//ǰ��˻�Ӧ��û����int ���� ����ɾ��
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

