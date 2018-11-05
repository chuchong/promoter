#pragma once
//分词算法 可以不含成员 只要给他一个字典 一个字符串就行
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

