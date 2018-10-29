//searcher ������������:1,����dict�ļ�
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
#include "HashDict.h"
class Searcher
{
	HashDict* dict_;
public:
	void parse(std::string url) {

		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		std::locale loc("chs");//windows��ok

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
			dict_->add(&s);
		}
	}

	bool haveStr(CharString * str) {
		return dict_->search(str);
	}

	Searcher() {
		dict_ = new HashDict(291143);//large prime 
	}
	~Searcher() {
		delete dict_;
	}
};

