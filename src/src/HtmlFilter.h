#pragma once
//�¸��汾����������Ϣ����,���ڲ��԰汾,���Թ��˱༭
//�����ǽ�������ƥ��,��ƥ��ɹ�,���ض�Ӧ�ı�
//���������ı���Ϣ��ȡ�����Ч

//һЩ�Զ�:��������û����Լ�ѧϰ��ô������Ϣ
#include "CharString.h"
class HtmlNode;


class HtmlFilter
{
private:
	HtmlNode * list_head;
	CharString * value;

	bool equal(HtmlNode * node1, HtmlNode * node2);
public:
	bool filter(HtmlNode * node, HtmlNode * filt);

	void searchTree(HtmlNode * tree);

	HtmlFilter();
	~HtmlFilter();
};

