//searcher 负责两个工作:1,解析dict文件,产生ditionary
//						2,搜索有无某字符串
//之后假如效率不够的话 可以按照字符串长度增设不同的dict
#pragma once
#include<string>
#include<locale>
#include <iostream>
#include <fstream>
#include <ostream>
#include <codecvt>
#include "CharString.h"
#include "Dict.h"
class Searcher
{
	wchar_t highest_char;
	wchar_t lowest_char;
	Dict* dict_2;//长为2的哈希
	//Dict* dict_3;//词长为3的哈希
	//Dict* dict_4;
	//Dict* dict_5plus;//大于等于5
public:

	void initDictionary(std::string url);

	bool haveStr(CharString * str);

	Searcher();

	bool charIn(wchar_t ch){
		return ch <= highest_char && ch >= lowest_char;
	}

	void addWord(CharString * str);

	~Searcher();
};

