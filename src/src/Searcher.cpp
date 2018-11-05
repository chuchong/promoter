#include "Searcher.h"

void Searcher::initDictionary(std::string url) {

	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	std::locale loc("chs");//windowsÏÂok

	std::ifstream in;
	in.imbue(loc);
	in.open(url);

	std::string str;
	std::wstring wstr;
	wstr = conv.from_bytes(str);
	//std::wcout.imbue(loc);
	//std::wcout << wstr;
	while (in >> str) {
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

bool Searcher::haveStr(CharString * str) {
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

Searcher::Searcher() {
	highest_char = wchar_t(0);
	lowest_char = wchar_t(66666);
	dict_2 = new HashDict(291143);//large prime
								  //dict_3 = new HashDict(99907);//large prime 
								  //dict_4 = new HashDict(99907);//large prime 
								  //dict_5plus = new HashDict(1021);//large prime 
}

void Searcher::addWord(CharString * str) {
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

Searcher::~Searcher() {
	delete dict_2;
	//delete dict_3;
	//delete dict_4;
	//delete dict_5plus;
}
