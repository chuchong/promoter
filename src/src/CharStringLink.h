#pragma once
#include "CharString.h"
struct CharStringNode {
	CharStringNode *next;
	CharStringNode *prior;
	CharString *str;
	CharStringNode() {
		prior = next = NULL;
		str = NULL;
	}
	~CharStringNode() {
		delete str;
	}
};
//�洢String ������ ,��ʱ�����洢
class CharStringLink
{
	CharStringNode * head;
	CharStringNode * end;
	//�����ɾ��ĳ�ڵ�ķ���,���ǲ���iterator��ʽ����
	void remove(CharStringNode * str_ptr);
public:
	//���һ���ڵ������
	bool isEmpty() {
		return head == NULL;
	}

	void add(const std::wstring & str);
	//ɾ��λ�ڵ�n����str
	void deleteAt(int index);
	//�����Ƿ����,��������,����-1
	int search(const CharString & str);
	CharStringLink();
	~CharStringLink();
};

