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

void CharStringLink::deleteAt(int index)
{
	CharStringNode * node = head;
	for (int i = 0; i < index; i++)
		node = node->next;
	remove(node);
}

void CharStringLink::remove(CharStringNode * node)
{
	//TODO 给个iterator方式来遍历 删除啥的
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
	while (!isEmpty())
		remove(head);
}
