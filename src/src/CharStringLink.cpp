#include "CharStringLink.h"



void CharStringLink::add(const std::wstring & str)
{
	CharStringNode * node= new CharStringNode();
	node->str = new CharString(str);

	if (head == NULL) {
		head = end = node;
	}
	else {
		end->next = node;
		node->prior = end;
		end = node;
	}

}

void CharStringLink::add(CharString * node)
{
	if (head == NULL) {
		head = end = new CharStringNode(node);
	}
	else {
		end->next = new CharStringNode(node);
		end->next->prior = end;
		end =  end->next;
	}

}

void CharStringLink::deleteAt(int index)
{
	CharStringNode * node = head;
	for (int i = 0; i < index; i++)
		node = node->next;
	remove(node);
}

void CharStringLink::remove(CharStringNode * node)
{
	//TODO ����iterator��ʽ������ ɾ��ɶ��
	//��bug!!! 
	if(node->prior != NULL)
		node->prior->next = node->next;
	if(node->next != NULL)
		node->next->prior = node->prior;
	delete node;
}

int CharStringLink::search(const CharString & str)
{
	return 0;
}

CharStringLink::CharStringLink():head(NULL), end(NULL)
{

}


CharStringLink::~CharStringLink()
{
	
	while(head != nullptr){
		CharStringNode * node = head;
		head = head->next;
		delete node;
	}

}
