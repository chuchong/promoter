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
#include <string>
#include <iostream>
#include <fstream>
#include <ostream>
#include <locale>
#include <codecvt>
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
	HtmlParser parser;
	parser.parse(L"../input/0.html");
}

int main() {
	//testStack();
	//testString();
	//testDict();
	testParser();

	return 0;
}