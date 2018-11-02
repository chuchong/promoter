//searcher 负责两个工作:1,解析dict文件
//						2,搜索有无某字符串
//之后假如效率不够的话 可以按照字符串长度增设不同的dict
//感觉没必要弄那么多
#pragma once
#include<string>
#include<locale>
#include <iostream>
#include <fstream>
#include <ostream>
#include <codecvt>
#include "CharString.h"
#include "HashDict.h"
class Searcher
{
	wchar_t highest_char;
	wchar_t lowest_char;
	Dict* dict_2;//长为2的哈希
	//Dict* dict_3;//词长为3的哈希
	//Dict* dict_4;
	//Dict* dict_5plus;//大于等于5
public:

	void parse(std::string url) {

		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		std::locale loc("chs");//windows下ok

		std::ifstream in;
		in.imbue(loc);
		in.open(url);

		std::string str;
		std::wstring wstr;
		wstr = conv.from_bytes(str);
		//std::wcout.imbue(loc);
		//std::wcout << wstr;
		while (in >> str){
			wstr = conv.from_bytes(str);
			CharString s(wstr);
			highest_char = (highest_char > s[0]) ? highest_char : s[0];
			lowest_char = (lowest_char < s[0]) ? lowest_char : s[0];

			dict_2->addCopyOf(&s);
			//switch (s.size()) {
			//case 2:
			//	dict_2->add(&s);
			//	break;
			//case 3:
			//	dict_3->add(&s);
			//	break;
			//case 4:
			//	dict_4->add(&s);
			//	break;
			//default:
			//	dict_5plus->add(&s);
			//	break;
			//}
		}
	}

	bool haveStr(CharString * str) {
		return dict_2->search(str);
		//switch (str->size()) {
		//case 2:
		//	return dict_2->search(str);
		//	break;
		//case 3:
		//	return dict_3->search(str);
		//	break;
		//case 4:
		//	return dict_4->search(str);
		//	break;
		//default:
		//	return dict_5plus->search(str);
		//	break;
		//}
	}

	Searcher() {
		highest_char = wchar_t(0);
		lowest_char = wchar_t(66666);
		dict_2 = new HashDict(291143);//large prime
		//dict_3 = new HashDict(99907);//large prime 
		//dict_4 = new HashDict(99907);//large prime 
		//dict_5plus = new HashDict(1021);//large prime 
	}

	bool charIn(wchar_t ch){
		return ch <= highest_char && ch >= lowest_char;
	}

	void addWord(CharString * str) {
		dict_2->addCopyOf(str);
		//switch (str->size()) {
		//case 2:
		//	dict_2->add(str);
		//	break;
		//case 3:
		//	dict_3->add(str);
		//	break;
		//case 4:
		//	dict_4->add(str);
		//	break;
		//default:
		//	dict_5plus->add(str);
		//	break;
		//};
	}

	~Searcher() {
		delete dict_2;
		//delete dict_3;
		//delete dict_4;
		//delete dict_5plus;
	}
};

