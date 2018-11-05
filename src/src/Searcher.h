//searcher ������������:1,����dict�ļ�,����ditionary
//						2,��������ĳ�ַ���
//֮�����Ч�ʲ����Ļ� ���԰����ַ����������費ͬ��dict
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
	Dict* dict_2;//��Ϊ2�Ĺ�ϣ
	//Dict* dict_3;//�ʳ�Ϊ3�Ĺ�ϣ
	//Dict* dict_4;
	//Dict* dict_5plus;//���ڵ���5
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

