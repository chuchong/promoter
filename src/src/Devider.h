#pragma once
//�ִ��㷨 ���Բ�����Ա ֻҪ����һ���ֵ� һ���ַ�������
#include "CharString.h"
#include "Dict.h"
#include "Searcher.h"
#include "CharStringLink.h"
class Devider
{

public:
	void devide(CharString * string, Searcher * searcher, CharStringLink * link);
	Devider();
	~Devider();
};

