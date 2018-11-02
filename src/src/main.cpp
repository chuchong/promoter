//注意我将dict中的bom头删掉了
//在读取其他人文件时,还需要添加bom头
//dict中是按照编码从小到大排的
//一19968 龟40863
#include "Stack.h"
#include "CharString.h"
#include "CharStringLink.h"
#include "HashDict.h"
#include "Searcher.h"
#include "HtmlParser.h"
#include "Devider.h"
#include <string>
#include <iostream>
#include <fstream>
#include <ostream>
#include <locale>
#include <codecvt>

#include <crtdbg.h>


#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

void EnableMemLeakCheck()
{
	int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(tmpFlag);
}

using namespace std;
void testStack() {
	Stack<char> c;
	for (int i = 1; i < 100; i ++)
		c.push('a');
	while (!c.empty()) {
		c.top();
		c.pop();
	}
}

void testString() {
	wstring aaa(L"AAA");
	CharString a(aaa);
	CharString str(L"1123333333333333333333333333333333333333");
	CharString str_1(L"13");
	str_1[1] = L'2';
	str_1.concat(L"33");
	str_1.resize(3);
	CharString * s = str.subString(0, 5);



	cout << s->indexOf(str_1, 0);
}

void testDict() {
	Searcher searcher;
	searcher.parse("../dictionary/词库.dic");
	CharString s(L"一心一意");
	CharString s2(L"红魔馆");
	cout << searcher.haveStr(&s);
	cout << searcher.haveStr(&s2);

}

void testParser() {


	for (int i = 0; i <= 20; i++) {
		HtmlParser parser;
		cout << "---------------------------" << i <<"---------------------" << endl;
		parser.parse(L"../input/" + to_wstring(i) + L".html");
		parser.print();
		cout << "---------------------------" << i << "---------------------" << endl;
	}
	return;
}

void testMemory() {
	//CharString
	while (1) {
		CharString* a = new CharString(L"aaaaaaaaaaaa");
		a->concat(L"aaaa");
		delete a;
	}
}

void testDevider() {
	Devider d;
	CharString test_word(L"做外套来穿的话，里面的内搭一定要修身，不然看起来会很显胖");
	Searcher s;
	s.parse("../dictionary/词库.dic");
	CharStringLink l;
	HtmlParser p;
	p.parse(L"../input/20.html");
	p.getText();
	d.devide(p.getText(), &s, &l);
	l.print();
	return;
}

void testINI() {
	Searcher s;
	s.parse("../dictionary/词库.dic");

	return;
}
int main() {
	//_CrtSetBreakAlloc(1134017);
	//_CrtSetBreakAlloc(1134014);
	//testStack();
	//testString();
	//testDict();
	testParser();
	//testMemory();
	//EnableMemLeakCheck();
	//testDevider();

	return 0;
}