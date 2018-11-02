//ע���ҽ�dict�е�bomͷɾ����
//�ڶ�ȡ�������ļ�ʱ,����Ҫ���bomͷ
//dict���ǰ��ձ����С�����ŵ�
//һ19968 ��40863
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
	searcher.parse("../dictionary/�ʿ�.dic");
	CharString s(L"һ��һ��");
	CharString s2(L"��ħ��");
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
	CharString test_word(L"�����������Ļ���������ڴ�һ��Ҫ������Ȼ�������������");
	Searcher s;
	s.parse("../dictionary/�ʿ�.dic");
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
	s.parse("../dictionary/�ʿ�.dic");

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